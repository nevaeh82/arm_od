#ifndef __SHOWLOG_H
#define __SHOWLOG_H
#include <QMutex>
#include <QPlainTextEdit>
#include <QThread>

QMutex mutex;
QPlainTextEdit* g_textEdit;

#if QT_VERSION < 0x050000
void myMessageOutput(QtMsgType type, const char *msg)
#else
#define qInstallMsgHandler qInstallMessageHandler

void myMessageOutput(QtMsgType type, const QMessageLogContext &, const QString &msg)
#endif
{
		QMutexLocker locker( &mutex);

		if ( !g_textEdit ) {
				return;
		}

		switch (type) {
		case QtDebugMsg:
				g_textEdit->appendPlainText(QString("[%1] Debug:  %2").arg( (int)QThread::currentThreadId()).arg( msg));
				break;
		case QtWarningMsg:
				g_textEdit->appendPlainText(QString("[%1] Warning: %2").arg((int)QThread::currentThreadId()).arg( msg));
				break;
		case QtCriticalMsg:
				g_textEdit->appendPlainText(QString("[%1] Critical: %2").arg( (int)QThread::currentThreadId()).arg( msg));
				break;
		case QtFatalMsg:
				g_textEdit->appendPlainText(QString("[%1] Fatal: %2").arg( (int)QThread::currentThreadId()).arg( msg));
				abort();
		}
}
void installLogHandler(QPlainTextEdit* widget)
{
	g_textEdit = widget;
	qInstallMsgHandler(myMessageOutput);
}

#endif
