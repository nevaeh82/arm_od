// ****************************
// *
// * Auto-generated don't edit
// *
// ****************************
#ifndef PWGISSTYLENUMSTOSTRING_H
#define PWGISSTYLENUMSTOSTRING_H

#include "PwGisStyle.h"

class PwGisStyleEnumToString {
public:
	static QString enumToString( PwGisStyle::Style val)
	{
		switch(val) {
			case PwGisStyle::fillColor: return "fillColor";
			case PwGisStyle::fillOpacity: return "fillOpacity";
			case PwGisStyle::strokeColor: return "strokeColor";
			case PwGisStyle::strokeOpacity: return "strokeOpacity";
			case PwGisStyle::strokeWidth: return "strokeWidth";
			case PwGisStyle::strokeLinecap: return "strokeLinecap";
			case PwGisStyle::strokeDashstyle: return "strokeDashstyle";
			case PwGisStyle::pointRadius: return "pointRadius";
			case PwGisStyle::pointerEvents: return "pointerEvents";
			case PwGisStyle::cursor: return "cursor";
			case PwGisStyle::externalGraphic: return "externalGraphic";
			case PwGisStyle::graphicWidth: return "graphicWidth";
			case PwGisStyle::graphicHeight: return "graphicHeight";
			case PwGisStyle::graphicOpacity: return "graphicOpacity";
			case PwGisStyle::graphicXOffset: return "graphicXOffset";
			case PwGisStyle::graphicYOffset: return "graphicYOffset";
			case PwGisStyle::rotation: return "rotation";
			case PwGisStyle::graphicName: return "graphicName";
			case PwGisStyle::display: return "display";
			case PwGisStyle::mapFontColor: return "mapFontColor";
			case PwGisStyle::mapFontOpacity: return "mapFontOpacity";
			case PwGisStyle::mapFontFamily: return "mapFontFamily";
			case PwGisStyle::mapFontSize: return "mapFontSize";
			case PwGisStyle::mapFontStyle: return "mapFontStyle";
			case PwGisStyle::mapFontWeight: return "mapFontWeight";
			case PwGisStyle::mapLabelOutlineColor: return "mapLabelOutlineColor";
			case PwGisStyle::mapLabelOutlineWidth: return "mapLabelOutlineWidth";
			case PwGisStyle::mapLabelXOffset: return "mapLabelXOffset";
			case PwGisStyle::mapLabelYOffset: return "mapLabelYOffset";
			case PwGisStyle::pwAllowSelect: return "pwAllowSelect";
			case PwGisStyle::pwAllowDrag: return "pwAllowDrag";
			case PwGisStyle::pwAllowReshape: return "pwAllowReshape";
			case PwGisStyle::pwAllowRotate: return "pwAllowRotate";
			case PwGisStyle::pwAllowResize: return "pwAllowResize";
			case PwGisStyle::layer: return "layer";
			case PwGisStyle::pathArrows: return "pathArrows";
			case PwGisStyle::heightArrow: return "heightArrow";
			case PwGisStyle::widthArrows: return "widthArrows";
			case PwGisStyle::minDistance: return "minDistance";
		}
		return QString("undefined (%1)").arg(val);
	} // end of Style
};

#endif
