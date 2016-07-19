#ifndef POPUPFEATURE_H
#define POPUPFEATURE_H

#include <Map/MapWidget.h>

namespace MapFeature {

class FeaturesFactory;

/// POPUP feature representation on map
class PopupFeature
{
public:
	PopupFeature(MapWidget* view, const QString& parentId, const QPointF& startPos);
	virtual ~PopupFeature() {}
	void update(const QPointF& pos, const QString &text);
	void show();

	void updateMap();

	void remove();

private:
	QString m_parentId;
	QString m_popupId;
	QPointF m_pos;
	QString m_popupText;

	MapWidget* m_view;

	QString createPopupId(const QString& id);
};

} // namespace MapFeature

#endif // POPUPFEATURE_H
