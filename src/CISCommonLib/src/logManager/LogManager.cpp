#include "LogManager.h"

LogManager::LogManager(const QString& fileName, const int& maxSize, const int& maxBackup)
{
	m_maxSize = maxSize;
	m_maxBackup = maxBackup;
	m_fileName = fileName;

	logFile.setFileName( fileName );
	openFile();
}

LogManager::~LogManager()
{
	logFile.close();
}

bool LogManager::isFileExist()
{
	return logFile.exists();
}

bool LogManager::openFile()
{
	return logFile.open(QIODevice::WriteOnly);
}

bool LogManager::isFileOpened()
{
	return logFile.isOpen();
}

void LogManager::writeToFile(QString message)
{
	logFile.write(message.toStdString().c_str());
	if( logFile.size() > m_maxSize ) {
		rollOver();
	}
}

void LogManager::rollOver()
{
	logFile.close();

	QFile f;
	f.setFileName( m_fileName + QLatin1Char('.') + QString::number(m_maxBackup) );
	if (f.exists() && !f.remove()) {
		openFile();
		return;
	}

	QString target_file_name;
	for (int i = m_maxBackup - 1; i >=1; i--)
	{
		f.setFileName(m_fileName + QLatin1Char('.') + QString::number(i));
		if (f.exists())
		{
			target_file_name = m_fileName + QLatin1Char('.') + QString::number(i + 1);
			if (!f.rename(target_file_name)) {
				openFile();
				return;
			}
		}
	}

	f.setFileName(m_fileName);
	target_file_name = m_fileName + QLatin1String(".1");
	if (!f.rename(target_file_name)) {
		openFile();
		return;
	}

	openFile();
	return;
}

