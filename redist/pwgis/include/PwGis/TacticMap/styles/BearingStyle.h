#ifndef BEARINGSTYLE_H
#define BEARINGSTYLE_H
#include <QObject>
#include "../TacticMap_global.h"

class MapObjectStyle;
class LineStyle;
class PwGisWidget;
class TextStyle;
class IStyleFactory;
class PwGisStyle;
class BearingBehaviourStyle;

class TACTICMAP_EXPORT BearingStyle: public QObject
{
	Q_OBJECT

	Q_CLASSINFO("ClassID", "{68b9b4cf-ccf9-45c6-8e9b-32d9615d3e51}")
	Q_CLASSINFO("InterfaceID", "{5f7196ef-549b-46d6-b53f-99ca65b6668b}")

	Q_PROPERTY (LineStyle* line READ line)
	Q_PROPERTY (TextStyle* text READ text)

private:
	MapObjectStyle* _underlyingStyle;
	BearingBehaviourStyle* _behaviour;

public:
	explicit BearingStyle(IStyleFactory* styleFactory, QObject* parent);
	explicit BearingStyle(QObject* parent = NULL);

	LineStyle* line() const;
	TextStyle* text() const;

public slots:
	BearingBehaviourStyle* behaviour() const;
	MapObjectStyle* underlyingStyle() const;

};

#endif // BEARINGSTYLE_H
