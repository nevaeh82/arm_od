#ifndef POST_H
#define POST_H
#include <QDateTime>
#include "TracedObjectAbstract.h"
#include "TacticMap_global.h"

class PostStyle;

/**
 * @brief The Post class holds presentation for Scanning Post
 */
class TACTICMAP_EXPORT Post: public TracedObjectAbstract
{
	Q_OBJECT
	Q_CLASSINFO("ClassID", "{1224a607-77ce-427c-93e4-bf27d91efde5}")
	Q_CLASSINFO("InterfaceID", "{7b6db617-f800-4918-b4cd-96a665610e39}")

	//Q_PROPERTY(PostStyle* postStyle READ postStyle WRITE setStyle)
private:
	PostStyle* _style;

public:
	explicit Post(IObjectsFactory* factory, QString id, QString name = "", QObject *parent = 0);
	explicit Post(QObject* parent=0);

	virtual ~Post();

public slots:
	//PostStyle* postStyle() const;
	void setStyle(PostStyle* postStyle);

};

#endif // POST_H
