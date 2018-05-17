#include "Logger/Logger.h"
#include <QDir>

RollerFileLoggerEngine::RollerFileLoggerEngine(const QString &fileName) : QxtBasicFileLoggerEngine(fileName)
{
}

void RollerFileLoggerEngine::setMaxBackup(const uint &index) {
	m_maxBackup = index;
}

void RollerFileLoggerEngine::setMaxFileSize(const uint &size) {
	m_maxFileSize = size;
}

void RollerFileLoggerEngine::writeToFile(const QString &level, const QVariantList &messages)
{
	QxtBasicFileLoggerEngine::writeToFile(level, messages);

	if (device()->size() > m_maxFileSize)
		rollOver();
}

void RollerFileLoggerEngine::rollOver()
{
	QIODevice::OpenMode savedOpenMode = device()->openMode();
	device()->close();
	QFile f;
	f.setFileName(logFileName() + QLatin1Char('.') + QString::number(m_maxBackup));
	if (f.exists() && !f.remove())
		return;

	QString target_file_name;
	for (int i = m_maxBackup - 1; i >=1; i--)
	{
		f.setFileName(logFileName() + QLatin1Char('.') + QString::number(i));
		if (f.exists())
		{
			target_file_name = logFileName() + QLatin1Char('.') + QString::number(i + 1);
			if (!f.rename(target_file_name))
				return;
		}
	}

	f.setFileName(logFileName());
	target_file_name = logFileName() + QLatin1String(".1");
	if (!f.rename(target_file_name))
		return;

	device()->open(savedOpenMode);
}


void Logger::setupLogger(const QString &logfile, const uint &maxFileSize, const uint &maxBackup)
{
	qxtLog->removeLoggerEngine("DEFAULT");

	if (!logfile.isEmpty()) {
		addLoggerEngine("fileroller", QxtLogger::AllLevels, logfile, maxFileSize, maxBackup);
#ifdef Q_OS_WIN
		addLoggerEngine("stdwin", QxtLogger::AllLevels);
#endif
	} else {
		addLoggerEngine("basestd", QxtLogger::AllLevels);
	}
}

void Logger::setLoggerLevel(const QxtLogger::LogLevels level)
{
	if( qxtLog->isLoggerEngine("fileroller") ) {
		qxtLog->disableAllLogLevels("fileroller");
		qxtLog->enableLogLevels("fileroller", level);
	}

	if( qxtLog->isLoggerEngine("stdwin") ) {
		qxtLog->disableAllLogLevels("stdwin");
		qxtLog->enableLogLevels("stdwin", level);
	}

	if( qxtLog->isLoggerEngine("basestd") ) {
		qxtLog->disableAllLogLevels("basestd");
		qxtLog->enableLogLevels("basestd", level);
	}
}

void Logger::addLoggerEngine(const QString &engineName, const QxtLogger::LogLevel& level, const QString &logfile, const uint &maxFileSize, const uint &maxBackup)
{
	if (!logfile.isEmpty()) {
		QFileInfo f(logfile);
		QDir d = f.absoluteDir();
		if (!d.exists()) {
			d.mkpath(d.absolutePath());
		}
		RollerFileLoggerEngine *file  = new RollerFileLoggerEngine(logfile);
		file->setMaxBackup(maxBackup);
		file->setMaxFileSize(maxFileSize);
		file->setDateFormat("dd.MM.yyyy hh:mm:ss.zzz");
		qxtLog->addLoggerEngine(engineName, file);
		qxtLog->disableAllLogLevels(engineName);
		qxtLog->enableLogLevels(engineName, level);
	} else {
		QxtBasicSTDLoggerEngine *std = new QxtBasicSTDLoggerEngine();
		qxtLog->addLoggerEngine(engineName, std);
		qxtLog->disableAllLogLevels(engineName);
		qxtLog->enableLogLevels(engineName, level);
	}
}
