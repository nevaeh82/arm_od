#ifndef DEMOTRANSACTION_H
#define DEMOTRANSACTION_H

#include <QWidget>
#include <QFile>
#include <QTextStream>

namespace Ui {
class DemoTransaction;
}

class DemoTransaction : public QWidget
{
	Q_OBJECT

private:
	Ui::DemoTransaction *ui;
	QFile _logFile;
	QTextStream _logStream;
	int _counterLine;

	QString getRuText( const QString &text );
	void executeLogLine( const QString &logLine );
	QString getTextBetweenQuotes( const QString &text );

	void addMarkers( const bool useTransaction = true );
	void addMarkerFromLog( const QString &logLine );

	void addCircles( const bool useTransaction = true );
	void addPathFromLog( const QString &logLine );

	void addPoints();

	void removeMarkerFromLog( const QString &logLine );
	void removeFeatureFromLog( const QString &logLine );
	void removeAllObjectsFromLog();
	void commitTransactionFromLog();

	inline QString createMarkerName( const int id ) { return QString( "marker #%1" ).arg( id ); }
	inline QString createObjectName( const int id ) { return QString( "item #%1" ).arg( id ); }


public:
	explicit DemoTransaction(QWidget *parent = 0);
	~DemoTransaction();

public slots:
	void onBtnOpenMap();
	void onBtnCloseMap();
	void onBtnAddMarkersWithoutTrans();
	void onBtnAddMarkersWithTrans();
	void onBtnAddPathWithoutTrans();
	void onBtnAddPathWithTrans();
	void onBtnAddPoints();
	void onBtnRemoveAllFeatures();
	void onBenchmark();
};

#endif // DEMOTRANSACTION_H
