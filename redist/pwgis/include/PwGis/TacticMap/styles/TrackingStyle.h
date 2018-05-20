#ifndef TRACKINGSTYLE_H
#define TRACKINGSTYLE_H

#include <QObject>
#include "../TacticMap_global.h"

class TACTICMAP_EXPORT TrackingStyle : public QObject
{
	Q_OBJECT

	Q_CLASSINFO("ClassID", "{bbf9eed4-7c22-4b9b-9ba5-57b62a2a20be}")
	Q_CLASSINFO("InterfaceID", "{b8ca9bb8-ee6b-4e9b-9732-6f44be185c00}")

	Q_PROPERTY (uint positionsLimit READ positionsLimit WRITE setPositionsLimit)
	Q_PROPERTY (double traceLength READ traceLength WRITE setTraceLength)
	Q_PROPERTY (double traceLengthSecond READ traceLifetime WRITE setTraceLifetime)

private:

	uint _latestPositionCount;
	double _traceLengthMeters;
	double _traceLengthSeconds;

public:
	explicit TrackingStyle(QObject* parent = NULL);

public:
	/**
	 * @brief Gets trace length in points.
	 * @return count of maximum trace points.
	 */
	uint positionsLimit() const;

	/**
	 * @brief Sets maximum trace points.
	 * @param count of points which can be presented in trace.
	 */
	void setPositionsLimit(uint positionsCount);

	/**
	 * @brief Indicates whether trace positions defined.
	 * @return true if trace positions defined.
	 */
	bool isTracePositionsLimitDefined() const
	{
		return _latestPositionCount != 0;
	}


	/**
	 * @brief Gets trace maximum leghth in meters.
	 * @return given trace length.
	 */
	double traceLength() const;

	/**
	 * @brief Sets trace maximum legth in metres. When trece will be longer then given length then older points will be removed.
	 * @param meters trace length in meters.
	 */
	void setTraceLength(double meters);

	/**
	 * @brief Indicates whether trace length defined.
	 * @return true if length defined.
	 */
	bool isTraceLengthDefined() const
	{
		return _traceLengthMeters != 0;
	}


	/**
	 * @brief Gets trace lifetime in seconds
	 * @return trace lifetime in seconds.
	 */
	double traceLifetime() const;

	/**
	 * @brief Sets trace lifetime. When trace will be longer then given time then older points will be removed.
	 * @param seconds  lifetime in seconds.
	 */
	void setTraceLifetime(double seconds);


	/**
	 * @brief Indicates whether trace lifetime defined.
	 * @return true if lifetime defined.
	 */
	bool isTraceLifetimeDefined() const
	{
		return _traceLengthSeconds != 0;
	}
};

#endif // TRACKINGSTYLE_H
