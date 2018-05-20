#ifndef MAPOBJECTSTYLE_H
#define MAPOBJECTSTYLE_H

#include <QObject>
#include "PwGisClient.h"
#include "IdGenerator.h"
#include "PwGisStyle.h"

class ConverterToCss;
class FillStyle;
class LineStyle;
class UserInteractionStyle;
class IconStyle;
class TextStyle;
class PwGisStyle;
class PwGisWidget;
class IStyleFactory;
class IdGenerator;


/**
 * @brief The MapObjectStyle class can be created in any time.
 * PwGisStyle->apply() will be called automaticaly on MapReady
 */
class GISCLIENT_EXPORT MapObjectStyle : public QObject
{
	Q_OBJECT

	Q_CLASSINFO("ClassID", "{e27162f6-5377-405e-a699-feeb81ab3d1d}")
	Q_CLASSINFO("InterfaceID", "{39e79841-a91f-4c4a-8243-350258bf3d38}")

	Q_PROPERTY (FillStyle* fill READ fill )
	Q_PROPERTY (LineStyle* line READ line )
	Q_PROPERTY (UserInteractionStyle* userInteraction READ userInteraction)
	Q_PROPERTY (IconStyle* icon READ icon )
	Q_PROPERTY (TextStyle* text READ text )
	Q_PROPERTY (QString id READ id)


	FillStyle* _fillStyle;

	LineStyle* _lineStyle;

	UserInteractionStyle* _userInteractionStyle;

	IconStyle* _iconStyle;

	TextStyle* _textStyle;

	PwGisStyle* _pwGisStyle;

	IdGenerator* _idGenerator;

public:
	MapObjectStyle(IStyleFactory* styleFactory, QObject *parent);
	MapObjectStyle(QObject *parent);

	FillStyle* fill()
	{
		return _fillStyle;
	}

	LineStyle* line()
	{
		return _lineStyle;
	}

	UserInteractionStyle* userInteraction()
	{
		return _userInteractionStyle;
	}

	IconStyle* icon()
	{
		return _iconStyle;
	}

	TextStyle* text()
	{
		return _textStyle;
	}

	QString id() const
	{
		return _pwGisStyle->getName();
	}

};

#endif // MAPOBJECTSTYLE_H
