#ifndef USERINTERACTIONSTYLE_H
#define USERINTERACTIONSTYLE_H

#include <QObject>
#include "BaseStyle.h"

class GISCLIENT_EXPORT UserInteractionStyle : public BaseStyle
{
	Q_OBJECT

	Q_CLASSINFO("ClassID", "{256ff7ac-4260-4f53-98c5-a6fe45de17cf}")
	Q_CLASSINFO("InterfaceID", "{778b8ee4-d4c9-4ffd-8157-51d3bb2ee5a5}")

	Q_PROPERTY (bool allowSelect READ allowSelect WRITE setAllowSelect NOTIFY allowSelectChanged)
	Q_PROPERTY (bool allowDrag READ allowDrag WRITE setAllowDrag NOTIFY allowDragChanged)
	Q_PROPERTY (bool allowReshape READ allowReshape WRITE setAllowReshape NOTIFY allowReshapeChanged)
	Q_PROPERTY (bool allowRotate READ allowRotate WRITE setAllowRotate NOTIFY allowRotateChanged)
	Q_PROPERTY (bool allowResize READ allowResize WRITE setAllowResize NOTIFY allowResizeChanged)

	bool _allowSelect;

	bool _allowDrag;
	bool _allowReshape;
	bool _allowRotate;
	bool _allowResize;

public:
	explicit UserInteractionStyle( PwGisStyle* style, QObject *parent );
	explicit UserInteractionStyle( QObject *parent );

	bool allowSelect() const;
	bool allowDrag() const;
	bool allowReshape() const;
	bool allowRotate() const;
	bool allowResize() const;

	void setAllowSelect( bool allowSelect);
	void setAllowDrag( bool allowDrag);
	void setAllowReshape( bool allowReshape);
	void setAllowRotate( bool allowRotate);
	void setAllowResize( bool allowResize);

signals:

	void allowSelectChanged(bool allowSelect);
	void allowDragChanged(bool allowDrag);
	void allowReshapeChanged(bool allowReshape);
	void allowRotateChanged(bool allowRotate);
	void allowResizeChanged(bool allowResize);
};



#endif // USERINTERACTIONSTYLE_H
