#include "BaseParser.h"

#include <QDir>
#include <QTime>
#include <QXmlStreamReader>


#include "Logger/Logger.h"


BaseParser::BaseParser(QObject *parent)
{
	m_view = new BaseParserWidget(0);

	connect(m_view, SIGNAL(signalPathSelected(QString)), this, SLOT(readFromPath(QString)));
}

BaseParserWidget *BaseParser::getView() const
{
	return m_view;
}

void BaseParser::readFromPath(QString path)
{
	QList<MobileSession> retList;

	QDir m_directory(path);
	QStringList filters;
	filters << "*.pwx";
	QStringList files = m_directory.entryList(filters, QDir::Files, QDir::Unsorted);

	if(files.isEmpty()) {
		return;
	}

	foreach (QString filePath, files) {

		QFile *file = new QFile(path + QDir::separator() + filePath);
		if (!file->open(QIODevice::ReadOnly))
		{
			continue;
		}

		MobileSession oneData;

		QXmlStreamReader xml(file);
		while (!xml.atEnd() && !xml.hasError()) {
			QXmlStreamReader::TokenType token = xml.readNext();

			if (token == QXmlStreamReader::StartDocument) continue;

			if (token == QXmlStreamReader::StartElement) {
				if( xml.name() == "Exported_Data" ) {
					continue;
				}

				if( xml.name() == "s_standard" ) {
					xml.readNext();
					oneData.standart = QString(QByteArray::fromBase64(xml.text().toUtf8())) + "\r\n" + filePath;
					continue;
				}

				if( xml.name() == "s_datetime" ) {
					xml.readNext();
					QDateTime date;
					QTime time;

					QStringList lst = xml.text().toString().split("_");
					log_debug(date.toString());

					//date =  date.fromString(QString("%1_%2_%3").arg(lst.at(0).arg(lst.at(1).arg(lst.at(2)))), "yyyy_MM_dd");
					oneData.datetime =  xml.text().toString();
					continue;
				}

				if( xml.name() == "s_network" ) {
					xml.readNext();
					oneData.network = QString(QByteArray::fromBase64(xml.text().toUtf8()));
					continue;
				}

				if( xml.name() == "s_basestation" ) {
					xml.readNext();
					oneData.basestation = QString(QByteArray::fromBase64(xml.text().toUtf8()));
					continue;
				}

				if( xml.name() == "s_lac" ) {
					xml.readNext();
					QString txt = xml.text().toString();
					oneData.lac = QString(QByteArray::fromBase64(xml.text().toUtf8()));
					continue;
				}

				if( xml.name() == "s_cid" ) {
					xml.readNext();
					oneData.cid = QString(QByteArray::fromBase64(xml.text().toUtf8()));
					continue;
				}

				if( xml.name() == "s_frequency" ) {
					xml.readNext();
					oneData.frequency = QString(QByteArray::fromBase64(xml.text().toUtf8()));
					continue;
				}

				if( xml.name() == "s_duration" ) {
					xml.readNext();
					oneData.duration = xml.text().toString();
					continue;
				}
//==================================================================================================
				if( xml.name() == "s_latitude" ) {
					xml.readNext();
					oneData.lat = xml.text().toString().toDouble();
					continue;
				}

				if( xml.name() == "s_longitude" ) {
					xml.readNext();
					oneData.lon = xml.text().toString().toDouble();
					continue;
				}

				if( xml.name() == "s_object" ) {
					xml.readNext();
					oneData.source = QString(QByteArray::fromBase64(xml.text().toUtf8()));
					continue;
				}
			}


		}//while

		retList.append(oneData);

		file->close();
		delete file;

	}


	m_view->setTableData(retList);
}
