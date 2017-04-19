#pragma once

#include <QObject>
#include <QXmlReader>
#include <QDateTime>

#include "BaseParserWidget.h"

class BaseParser : public QObject
{

Q_OBJECT

public:
	BaseParser(QObject* parent = 0);
	~BaseParser(){}

	BaseParserWidget *getView() const;

private:
	QXmlReader* m_reader;

	BaseParserWidget* m_view;

private slots:
	void readFromPath(QString);
};

