#ifndef DEBUGUTILS_H
#define DEBUGUTILS_H

#include <QString>
#include <QFile>
#include <QByteArray>
#include <QRegExp>
#include <QStringList>

class DebugUtils
{
public:
	static QString printByteArray(const QByteArray& data, bool showAscii = false)
	{
		QString s;
		uchar c;

		for ( int i=0 ; i < data.size() ; i++ ){
			c = data[i];
			if ( showAscii &&  c >= 0x20 && c <= 126 ) {
				s.append(c);
			} else {
				s.append(QString("<%1>").arg(c, 2, 16, QChar('0')));
			}
		}

		return s;
	}

	static int randInt(int low, int high)
	{
		// Random number between low and high
		return qrand() % ((high + 1) - low) + low;
	}

	static QByteArray readComPortData(QFile& skyhobbitLogFile)
	{
		if (!skyhobbitLogFile.exists()){
			return QByteArray();
		}

		skyhobbitLogFile.open(QFile::ReadOnly);
		QRegExp rx("DEBUG ComServer\\s+-\\s+(.{0,5}[\\<].{2,2}[\\>].+)[\\r\\n]+$");
		QRegExp splitRx("[\\<](.{2,2})[\\>]");

		splitRx.setMinimal(true);
		rx.setMinimal(true);

		QByteArray comPortData;
		QByteArray log;

		int pos = 0;

		do {
			log = skyhobbitLogFile.readLine();
			QString str(log);
			if (rx.indexIn(str, 0) != -1){
				QString data = rx.cap(1);
				data.remove('\r');
				data.remove('\n');

				while ((pos = splitRx.indexIn(data, 0)) != -1){
					bool ok = false;
					data.replace(pos, 4, splitRx.cap(1).toUInt(&ok,16));
				}
				comPortData.append(data.toAscii());
			}
		} while (log.count());

		return comPortData;
	}
};

#endif // DEBUGUTILS_H
