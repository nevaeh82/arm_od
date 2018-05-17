#ifndef CONVERTERTOCSS_H
#define CONVERTERTOCSS_H

#include <QObject>
#include <QtGui/QColor>
#include "PwGisClient.h"

class GISCLIENT_EXPORT ConverterToCss: public QObject {

	Q_OBJECT

public:
	enum  EStrokeDashstyle //Default is solid. Options are: dot, dash, dashdot, longdash, longdashdot, solid
	{
		solid, dot, dash, dashdot, longdash, longdashdot
	};
	enum ECursor
	{
		autoCursor,
		crosshair,
		defaultCursor,
		e_resize,
		help,
		move,
		n_resize,
		ne_resize,
		nw_resize,
		pointer,
		progress,
		s_resize,
		se_resize,
		sw_resize,
		text,
		w_resize,
		wait
	};
	Q_ENUMS(EStrokeDashstyle)
	Q_ENUMS(ECursor)

	static QString intToCssColor(QColor aColor);
	static QString strokeDashstyleToCss(EStrokeDashstyle style);
	static QString cursorToCss(ECursor cursor);
};

#endif // CONVERTERTOCSS_H
