#include <QImage>
#include <QBuffer>
#include <QMessageBox>
#include "PopupFeature.h"

#include <QDebug>

namespace MapFeature {

PopupFeature::PopupFeature(MapWidget* view, const QString& parentId, const QPointF& startPos)
{
	m_view = view;
	m_parentId = parentId;
	m_popupId = createPopupId(parentId);
	m_pos = startPos;
	m_popupText = QString("\" \"");
}

void PopupFeature::update(const QPointF& pos, const QString& text)
{
	m_pos = pos;
	m_popupText = QString("\"") + text + QString("\"");
}

void PopupFeature::show()
{
	m_view->getPwGis()->executeScript(QString("Pw.Gis.Client.SkyHobbitBlaPopup.showPopup(%1)")
								.arg(m_popupId)
								);
}

void PopupFeature::updateMap()
{
	m_view->getPwGis()->executeScript(QString("SkyHobbitAddBlaFeature(%1, %2, %3, %4)")
									.arg(m_popupId)
									.arg(m_pos.x())
									.arg(m_pos.y())
									.arg(m_popupText)
									);
}

void PopupFeature::remove()
{
	m_view->getPwGis()->executeScript(QString("SkyHobbitRemoveBlaFeature(%1)").arg(m_popupId));
}

QString PopupFeature::createPopupId(const QString& id)
{
	QString tmpPopup = QString(id);
	return QString("\"") + tmpPopup + QString("\"");
}

} // namespace MapFeature
