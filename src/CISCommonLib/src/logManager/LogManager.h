#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <QFile>

class LogManager
{
public:
	LogManager(const QString& fileName = QString(), const int& maxSize = 10*1024*1024, const int& maxBackup = 11);
	~LogManager();

	void writeToFile(QString message);
	bool isFileOpened();
	bool isFileExist();

private:
	int m_maxSize;
	int m_maxBackup;
	QString m_fileName;

	QFile logFile;
	bool openFile();
	void rollOver();
};

#endif // LOGMANAGER_H
