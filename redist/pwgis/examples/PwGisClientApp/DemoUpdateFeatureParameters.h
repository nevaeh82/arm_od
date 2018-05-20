#ifndef DEMOUPDATEFEATUREPARAMETERS_H
#define DEMOUPDATEFEATUREPARAMETERS_H

#include <QWidget>
#include <QFile>
#include <QTextStream>

namespace Ui {
class DemoUpdateFeatureParameters;
}

class DemoUpdateFeatureParameters : public QWidget
{
	Q_OBJECT

private:
	Ui::DemoUpdateFeatureParameters *ui;
	QFile _logFile;
	QTextStream _logStream;
	int _counterLine;

	QString getRuText( const QString &text );
	void executeLogLine( const QString &logLine );
	QString getTextBetweenQuotes( const QString &text );
	void addMarkerFromLog( const QString &logLine );
	void addPolygonFromLog( const QString &logLine );
	void removeMarkerFromLog( const QString &logLine );
	void removeFeatureFromLog( const QString &logLine );
	void removeAllObjectsFromLog();
	void commitTransactionFromLog();

public:
	explicit DemoUpdateFeatureParameters(QWidget *parent = 0);
	~DemoUpdateFeatureParameters();

public slots:
	void onBtnOpenMap();
	void onBtnCloseMap();
	void onBtnAddPolygone();
	void onBtnAddStylePolygone();
	void onBtnOpenFile();
	void onBtnReadLine();
};

#endif // DEMOUPDATEFEATUREPARAMETERS_H
