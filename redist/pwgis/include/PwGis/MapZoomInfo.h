#ifndef MAPZOOMINFO_H
#define MAPZOOMINFO_H
#include <QObject>
#include "ScalesCollection.h"

class GISCLIENT_EXPORT MapZoomInfo : public QObject
{
	Q_OBJECT
	Q_CLASSINFO("ClassID", "{e86a38ba-87f6-41ff-b661-11021a9f096d}")
	Q_CLASSINFO("InterfaceID", "{c7fb1d2c-3ebb-4508-8f11-aa0ccaa248dc}")

private:
	double _maxScale;
	double _minScale;
	double _maxResolution;
	double _minResolution;
	int _numZoomLevels;

	ScalesCollection* _scales;
	ScalesCollection* _resolutions;

public:
	MapZoomInfo(QObject* parent = 0) :
		QObject(parent),
		_maxScale(0),
		_minScale(0),
		_maxResolution(0),
		_minResolution(0),
		_numZoomLevels(0),
		_scales(new ScalesCollection(parent)),
		_resolutions(new ScalesCollection(parent))
	{
	}
public slots:
	double maxScale() { return _maxScale; }
	double minScale() { return _minScale; }
	double maxResolution() { return _maxResolution; }
	double minResolution() { return _minResolution; }
	int numZoomLevels() { return _numZoomLevels; }

	void setMaxScale (double scale) { _maxScale = scale; }
	void setMinScale (double scale) { _minScale = scale; }
	void setMaxResolution (double res) { _maxResolution = res; }
	void setMinResolution (double res) { _minResolution = res; }
	void setNumZoomLevels (int num) { _numZoomLevels = num; }

	ScalesCollection* scales() { return _scales; }
	ScalesCollection* resolutions() { return _resolutions; }
};

#endif // MAPZOOMINFO_H
