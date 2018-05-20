#ifndef LOGGER_H
#define LOGGER_H

#include <QxtLogger>
#include <QxtBasicFileLoggerEngine>
#include <QxtBasicSTDLoggerEngine>
#include <QFileInfo>

#ifdef Q_OS_WIN
#define __func__ __FUNCTION__
#endif

#ifdef USE_SYSLOG
#include <syslog.h>
#include <QDateTime>
#define log_all(x,s) syslog(LOG_USER | x, "%s", (QString("[%1] %2::%3:%4 %5").arg(QDateTime::currentDateTimeUtc().toString("dd.MM.yyyy hh:mm:ss.zzz")).arg(QFileInfo(__FILE__).fileName()).arg(__func__).arg(__LINE__).arg(s)).trimmed().toLatin1().data())

#define log_trace(s) log_all(LOG_DEBUG,s)
#define log_debug(s) log_all(LOG_DEBUG,s)
#define log_info(s) log_all(LOG_INFO,s)
#define log_warning(s) log_all(LOG_WARNING,s)
#define log_error(s) log_all(LOG_ERR,s)
#define log_critical(s) log_all(LOG_CRIT,s)
#define log_fatal(s) log_all(LOG_ALERT,s)
#define log_write(s) log_all(LOG_EMERG,s)

#else

#define log_all(x,s) qxtLog->x(QString("%1::%2:%3 %4").arg(QFileInfo(__FILE__).fileName()).arg(__func__).arg(__LINE__).arg(s).trimmed())

#define log_trace(s) log_all(trace,s)
#define log_debug(s) log_all(debug,s)
#define log_info(s) log_all(info,s)
#define log_warning(s) log_all(warning,s)
#define log_error(s) log_all(error,s)
#define log_critical(s) log_all(critical,s)
#define log_fatal(s) log_all(fatal,s)
#define log_write(s) log_all(write,s)

#define log_engine(eng,level,s) if (qxtLog->engine(eng) && qxtLog->engine(eng)->isInitialized() && qxtLog->engine(eng)->isLoggingEnabled() && qxtLog->engine(eng)->isLogLevelEnabled(level)) qxtLog->engine(eng)->writeFormatted(level, QString("%1::%2:%3 %4").arg(QFileInfo(__FILE__).fileName()).arg(__func__).arg(__LINE__).arg(s).trimmed())

#define log_eng_trace(eng,s) log_engine(eng,QxtLogger::TraceLevel,s)
#define log_eng_debug(eng,s) log_engine(eng,QxtLogger::DebugLevel,s)
#define log_eng_info(eng,s) log_engine(eng,QxtLogger::InfoLevel,s)
#define log_eng_warning(eng,s) log_engine(eng,QxtLogger::WarningLevel,s)
#define log_eng_error(eng,s) log_engine(eng,QxtLogger::ErrorLevel,s)
#define log_eng_critical(eng,s) log_engine(eng,QxtLogger::CriticalLevel,s)
#define log_eng_fatal(eng,s) log_engine(eng,QxtLogger::FatalLevel,s)
#define log_eng_write(eng,s) log_engine(eng,QxtLogger::WriteLevel,s)

#endif

class RollerFileLoggerEngine : public QxtBasicFileLoggerEngine
{
public:
	RollerFileLoggerEngine(const QString& fileName = QString());

	void setMaxBackup(const uint& index);
	void setMaxFileSize(const uint& size);

private:
	uint m_maxBackup;
	uint m_maxFileSize;

protected:
	virtual void writeToFile(const QString &level, const QVariantList &messages);
	void rollOver();
};

class Logger
{
public:
	static void setupLogger(const QString& logfile = QString(),
							const uint& maxFileSize = 10*1024*1024,
							const uint& maxBackup = 11);
	static void setLoggerLevel(const QxtLogger::LogLevels level);
	static void addLoggerEngine(const QString& engineName,
								const QxtLogger::LogLevel& level,
								const QString& logfile = QString(),
								const uint& maxFileSize = 10*1024*1024,
								const uint& maxBackup = 11);
};

#endif // LOGGER_H
