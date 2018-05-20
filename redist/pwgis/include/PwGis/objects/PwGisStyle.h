#ifndef PWGISSTYLE_H
#define PWGISSTYLE_H
#include <QObject>
#include <QString>
#include <QMap>

#include "PwGisClient.h"


class IMapProvider;

class GISCLIENT_EXPORT PwGisStyle : public QObject
{
	Q_OBJECT
	Q_ENUMS(Style)
	Q_CLASSINFO("ClassID", "{19DC49F5-889F-4189-8C44-0FA2C3CC99F4}")
	Q_CLASSINFO("InterfaceID", "{30AC18E9-65BA-4D39-940F-DFC51097BCBC}")
	//Q_CLASSINFO("RegisterObject", "yes")

public:
	enum Style {
		fillColor, // Default is #ee9900. This is the color used for filling in Polygons. It is also used in the center of marks for points: the interior color of circles or other shapes. It is not used if an externalGraphic is applied to a point.
		fillOpacity, // Default is 0.4. This is the opacity used for filling in Polygons. It is also used in the center of marks for points: the interior color of circles or other shapes. It is not used if an externalGraphic is applied to a point.
		strokeColor, // Default is #ee9900. This is color of the line on features. On polygons and point marks, it is used as an outline to the feature. On lines, this is the representation of the feature.
		strokeOpacity, // Default is 1 This is opacity of the line on features. On polygons and point marks, it is used as an outline to the feature. On lines, this is the representation of the feature.
		strokeWidth, // Default is 1 This is width of the line on features. On polygons and point marks, it is used as an outline to the feature. On lines, this is the representation of the feature.
		strokeLinecap, // Default is round. Options are butt, round, square. This property is similar to the SVG stroke-linecap property. It determines what the end of lines should look like. See the SVG link for image examples.
		strokeDashstyle, // Default is solid. Options are: dot, dash, dashdot, longdash, longdashdot, solid
		pointRadius, // Default is 6.
		pointerEvents, // Default is visiblePainted. Only used by the SVG Renderer. See SVG pointer-events definition for more. (http://www.w3.org/TR/SVG11/interact.html#PointerEventsProperty)
		cursor, // Cursor used when mouse is over the feature. Default is an empty string, which inherits from parent elements.
		externalGraphic, // An external image to be used to represent a point.
		graphicWidth,
		graphicHeight, // These properties define the height and width of an externalGraphic. This is an alternative to the pointRadius symbolizer property to be used when your graphic has different sizes in the X and Y direction.
		graphicOpacity, // Opacity of an external graphic.
		graphicXOffset,
		graphicYOffset, // Where the ‘center’ of an externalGraphic should be.
		rotation, // The rotation angle in degrees clockwise for a point symbolizer.
		graphicName, // Name of a type of symbol to be used for a point mark.
		display, // Can be set to ‘none’ to hide features from rendering.
		mapFontColor,	//The font color for the label, to be provided like CSS.
		mapFontOpacity,	// Opacity (0-1) for the label
		mapFontFamily,	//The font family for the label, to be provided like in CSS.
		mapFontSize,	// The font size for the label, to be provided like in CSS.
		mapFontStyle,	// The font style for the label, to be provided like in CSS.
		mapFontWeight,	// The font weight for the label, to be provided like in CSS.
		mapLabelOutlineColor, //The halo color for the halo label
		mapLabelOutlineWidth, //The halo width for the halo label
		mapLabelXOffset, //The x offset for the halo label
		mapLabelYOffset, //The y offset for the halo label
		pwAllowSelect, // Object can be selected by the mouse. JS notifies 2 layers
		pwAllowDrag, // Object is dragable. JS notifies vector layer
		pwAllowReshape, // Object is reshapable. JS notifies vector layer
		pwAllowRotate, // Object is rotatable. JS notifies vector layer
		pwAllowResize, // Object is resizable. JS notifies vector layer
		layer, // layer object located on
		pathArrows, // allow to draw arrows for path
		heightArrow, // height of path arrow in px
		widthArrows, // width of path arrow in px
		minDistance // the minimum distance between the arrows on the screen in px
	};

	PwGisStyle(QObject *parent);

	PwGisStyle(const PwGisStyle& pwgisstyle);

	PwGisStyle(IMapProvider* mapProvider, const QString& styleName,  QObject* parent);

	PwGisStyle& operator=(const PwGisStyle &another);

	QString getName();

private:
	IMapProvider* _mapProvider;
	QString _styleName;
	QMap<Style, QString> _properties;

public slots:

	PwGisStyle* setProperty( Style propertyName, const QString& value);

	QString getProperty( Style propertyName) const;

	PwGisStyle* apply();

};



#endif // PWGISSTYLE_H
