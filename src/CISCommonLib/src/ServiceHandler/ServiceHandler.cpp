#include <QCoreApplication>
#include <QDir>
#include <QDateTime>
#include <QMutexLocker>

#include "ProcessStateEnumsToString.h"
#include "CommonCodesEnumsToString.h"
#include "ServiceTerminateCauseEnumsToString.h"

#include "ServiceHandler.h"

#include "Sleeper.h"

#ifndef	__linux__
#define SUFFIX  ".exe"
#else
#define SUFFIX
#endif

#ifdef Q_OS_WIN

#ifdef WINVER
#undef WINVER
#define WINVER 0x0501
#else
#define WINVER 0x0501
#endif

#include <windows.h>
#endif

typedef SkyHobbit::Common::ServiceControl::ServiceErrorCodes ServiceError;

namespace SkyHobbit
{
namespace Common
{
namespace ServiceControl
{

ServiceHandler::ServiceHandler(const QString& servicePath, const QStringList& args, ITerminateRpcController* rpcController, QObject *parent):
	QObject(parent),
	c_terminateTimeout(1000),
	_process(NULL),
	_state(ProcessState::NotRunning),
	_lastServiceError( ServiceError::Success ),
	_syncRoot(QMutex::Recursive)
{

#ifdef Q_OS_WIN
	_childCloseJob = CreateJobObject(NULL, NULL);

#endif

	QMutexLocker locker(&_syncRoot);

	m_agentRpcController = rpcController;

	_servicePath = servicePath;
	_args = args;

	_cmdline = getCommandLine();

	if ( _servicePath.isEmpty()) {
		return;
	}

	_cmdline += " " + _args.join(" ");
}

ServiceHandler::~ServiceHandler()
{
	QMutexLocker locker(&_syncRoot);

	if ( !_process ){
		return;
	}

	_process->disconnect(this);

	_process->kill();
	_process->waitForFinished(c_terminateTimeout);

	delete _process;
	_process = NULL;

#ifdef Q_OS_WIN
	CloseHandle(_childCloseJob);
#endif
}

Q_PID ServiceHandler::getProcessId() const
{
	return _process->pid();
}

QString ServiceHandler::getCommandLine()
{
#ifdef Q_OS_WIN
	return QString("\"%1\"").arg(getServicePath());
#else
	return getServicePath();
#endif
}

void ServiceHandler::errorCodesProcessing(int code)
{
	_lastServiceError = (ServiceError::Enum) code;

	log_error( QString("Service %2: %1 (Process State: %3)")
				.arg(ServiceErrorEnumsToString::enumToString(_lastServiceError))
				.arg(serviceName())
				.arg(ProcessStateEnumsToString::enumToString(_state)) );
}

void ServiceHandler::onProcessStarted()
{
	_state = ProcessState::IsRunnung;
#ifdef Q_OS_WIN

	if(!_childCloseJob) return;

	JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli;
	memset(&jeli, 0, sizeof(jeli));
	jeli.BasicLimitInformation.LimitFlags = 0x00002000;//JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE Causes all processes associated with the job to terminate when the last handle to the job is closed.

	if(!SetInformationJobObject( _childCloseJob, JobObjectExtendedLimitInformation, &jeli, sizeof(jeli))) return;

	AssignProcessToJobObject(_childCloseJob, _process->pid()->hProcess);

#endif

	emit processStartedSignal();
}

void ServiceHandler::onProcessStartFailed(QProcess::ProcessError error)
{
	if (QProcess::FailedToStart != error) {
		return;
	}

	emit processStartFailedSignal();
}

void ServiceHandler::onProcessDestroyed(int code, QProcess::ExitStatus status)
{
	QMutexLocker locker(&_syncRoot);

	emit onProcessDead();

	log_critical(QString("Service %4 destroyed: rc:%1 status:%2 isexpected:%3")
			  .arg(code)
			  .arg((status == QProcess::CrashExit ? "crash" : "normal" ))
			  .arg((_state != ProcessState::IsRunnung ? "expected" : "not expected" ))
			  .arg(serviceName()));

	errorCodesProcessing(code);

	if ( _state == ProcessState::IsRunnung ) {
		Sleeper::sleep(2);
		startService(true);
	}
}

#define service_log(s) qxtLog->write(s)

void ServiceHandler::onReadProcessOutput()
{
	service_log(QString("%1").arg(QString(_process->readAllStandardOutput())).trimmed());
}

void ServiceHandler::onReadProcessError()
{
	service_log(QString("%1").arg(QString(_process->readAllStandardError())).trimmed());
}

void ServiceHandler::startService(const bool isRestart, const bool isCatchLogs)
{
	QMutexLocker locker(&_syncRoot);
	//_process->startDetached(_cmdline);
	if (_process == NULL) {
		_process = new QProcess(this);

	}

	_process->disconnect(this);

	connect(_process, SIGNAL(finished(int,QProcess::ExitStatus)), SLOT(onProcessDestroyed(int,QProcess::ExitStatus)));

	if (isCatchLogs) {
		connect(_process, SIGNAL(readyReadStandardOutput()), SLOT(onReadProcessOutput()));
		connect(_process, SIGNAL(readyReadStandardError()), SLOT(onReadProcessError()));
	}
	connect(_process, SIGNAL(started()), this, SLOT(onProcessStarted()));
	connect(_process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(onProcessStartFailed(QProcess::ProcessError)));

	///BYCYCLE
#ifdef Q_OS_UNIX
	QString cmdLine = _cmdline;
	if (isRestart) {
		cmdLine.append(" restart");
	}

	log_debug(QString("Cmd line: %1").arg(cmdLine));
	_process->start( cmdLine, QProcess::ReadOnly);
#else
	QStringList argsList;

	if (isRestart) {
		argsList.append("restart");
	}

	if (argsList.isEmpty()) {
		_process->start( _cmdline, QProcess::ReadOnly);
	} else {
		_process->start( _cmdline, argsList, QProcess::ReadOnly);
	}

#endif

}

QString ServiceHandler::getServicePath()
{
	if ( !_servicePath.isEmpty()) {
		return _servicePath;
	}
	QStringList names;

	names.append( serviceName() + "" SUFFIX);

	QStringList paths;
	paths.append(QDir::currentPath() );
	paths.append(QCoreApplication::applicationDirPath());

	foreach( QString path, paths ){
		foreach( QString name, names ) {
			QString fullname = QDir::toNativeSeparators(path + QDir::separator() + name);
			log_trace(QString("Checking %1").arg(fullname));

			if ( QFile(fullname ).exists()) {
				_servicePath = fullname ;
				return _servicePath;
			}
		}
	}

	return _servicePath;
}

int ServiceHandler::serviceHandle()
{
	QMutexLocker locker(&_syncRoot);
	if ( !_process ) {
		return 0;
	}
	return (int)_process->pid();
}

bool ServiceHandler::isServiceStarted()
{
	QMutexLocker locker(&_syncRoot);
	if ( !_process ) {
		return false;
	}
	return _process->state() == QProcess::Running;
}

QString ServiceHandler::serviceName()
{
	return _servicePath;
}

int ServiceHandler::terminate()
{
	log_info(" terminate >>>");
	QMutexLocker locker(&_syncRoot);

	if ( !_process ) {
		return ServiceTerminateCause::NotRunned;
	}

	_process->disconnect(this);

	//Try shutdown correctly.
	if (m_agentRpcController == NULL) {
		kill();
		return ServiceTerminateCause::ByKill;
	}

	m_agentRpcController->sendTerminate(getServicePath()+ "" SUFFIX);
	qApp->processEvents();

	if ( _process->waitForFinished(c_terminateTimeout)) {
		log_debug(" waitForFinished = true");
		//_process->deleteLater();
		//_process = NULL;
		return ServiceTerminateCause::ByCommand;
	}
	/*
	_process->terminate();
	if ( _process->waitForFinished(c_terminateTimeout) ) {
		_process->deleteLater();
		_process = NULL;
		return ServiceTerminateCause::ByTerminate;
	}*/

	kill();
	return ServiceTerminateCause::ByKill;
}
void ServiceHandler::kill()
{
	log_info(" kill >>");
	QMutexLocker locker(&_syncRoot);

	_state = ProcessState::NotRunning;
	if ( !_process ) {
		return;
	}
	_process->disconnect(this);

#ifdef Q_OS_WIN

	//TerminateProcess(_process->pid()->hProcess, 42);
	//_process->waitForFinished(c_terminateTimeout);
	_process->kill();
	_process->close();
	delete _process;
	_process = NULL;
#else
	_process->terminate();
	_process->waitForFinished(c_terminateTimeout);
	_process->close();
	delete _process;
	_process = NULL;
#endif
}

bool ServiceHandler::isServiceAvailable()
{
	return _process ? _process->isOpen() : false;
}

void ServiceHandler::start(const bool isRestart, const bool isCatchLogs)
{
	startService(isRestart, isCatchLogs);
}

bool ServiceHandler::waitService(int timeout)
{
	QMutexLocker locker(&_syncRoot);

	QDateTime expireTime = QDateTime::currentDateTime().addSecs(timeout);

	while( !isServiceAvailable() && QDateTime::currentDateTime() < expireTime && _state == ProcessState::IsRunnung )
	{
		QCoreApplication::processEvents();
		SharedThread::msleep(100);
	}
	return isServiceAvailable();
}

void ServiceHandler::waitForFinished()
{
	if (_process == NULL){
		return;
	}

	_process->waitForFinished(c_terminateTimeout);

}

void ServiceHandler::waitForStarted(int msecs)
{
	_process->waitForStarted(msecs);
}

void ServiceHandler::addArgument(QString arg)
{
	_args.append(arg);
}

} // namespace ServiceControl
} // namespace Common
} // namespace Pw
