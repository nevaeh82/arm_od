#ifndef BIMAP_H
#define BIMAP_H

#include <QMap>

/// Bidirectional map based on QMap
template <typename X, typename Y>
class BiMap
{
protected:
	QMap<X, Y> m_left;
	QMap<Y, X> m_right;

public:
	BiMap() {}
	BiMap(const BiMap<X, Y> &other) { operator =(other);}

	/**
	 * Returns the number of (left, right) pairs in the map.
	 * @see isEmpty(), count()
	 */
	int size();

	/**
	 * Removes all items from the map.
	 * @see removeByLeft(), removeByRight()
	 */
	void clear();

	/**
	 * Returns true if the map contains no items; otherwise returns false.
	 * @see size()
	 */
	bool isEmpty();

	/**
	 * Inserts a new item with left and right arguments.
	 *
	 * If there is already an item with the left key or with the right key,
	 * that new item was not inserted
	 *
	 * @return true if item inserted
	 */
	bool insert(const X& left, const Y& right);

	/**
	 * Removes the item that have left key from the map.
	 *
	 * @return true, if left key exists in map and map item was removed
	 * @see removeByRight()
	 */
	bool removeByLeft(const X& left);

	/**
	 * Removes the item that have right key from the map.
	 *
	 * @return true, if right key exists in map and map item was removed
	 * @see removeByLeft()
	 */
	bool removeByRight(const Y& right);

	/**
	 * Returns true if the map contains an item with left key; otherwise returns false.
	 * @see containsRight(), removeByLeft(), count()
	 */
	bool containsLeft(const X& left);

	/**
	 * Returns true if the map contains an item with right key; otherwise returns false.
	 * @see containsLeft(), removeByRight(), count()
	 */
	bool containsRight(const Y& right);

	/**
	 * Returns the left key associated with the right key.
	 *
	 * If the map contains no item with right key,
	 * the function returns a default-constructed value.
	 *
	 * @see right(), rightKeys(), containsRight()
	 */
	const X left(const Y& right);

	/**
	 * Returns the right key associated with the left key.
	 *
	 * If the map contains no item with left key,
	 * the function returns a default-constructed value.
	 *
	 * @see left(), leftKeys(), containsLeft()
	 */
	const Y right(const X& left);

	/**
	 * Returns a list containing all the left keys in the map.
	 *
	 * @see rightKeys(), left(), right()
	 */
	QList<X> leftKeys();

	/**
	 * Returns a list containing all the right keys in the map.
	 *
	 * @see leftKeys(), left(), right()
	 */
	QList<Y> rightKeys();

	/**
	 * Returns true if other is not equal to this map; otherwise returns false.
	 *
	 * Two maps are considered equal if they contain the same (left, right) pairs.
	 *
	 * This function requires the left and right types to implement operator==().
	 *
	 * @see operator==()
	 */
	bool operator!=(const BiMap<X, Y>& other);

	/**
	 * Returns true if other is equal to this map; otherwise returns false.
	 *
	 * Two maps are considered equal if they contain the same (left, right) pairs.
	 *
	 * This function requires the left and right types to implement operator==().
	 *
	 * @see operator!=()
	 */
	bool operator==(const BiMap<X, Y>& other);

	/**
	 * Assigns other to this map and returns a reference to this map.
	 */
	BiMap<X, Y>& operator=(const BiMap<X, Y>& other);

	// method aliases

	/**
	 * This is an overloaded function. Same as size().
	 */
	inline int count() { return size(); }

	/**
	 * This function is provided for STL compatibility. It is equivalent to isEmpty().
	 * @return returning true if the map is empty; otherwise returning false.
	 */
	inline bool empty() { return isEmpty(); }
};

template <typename X, typename Y>
int BiMap<X, Y>::size()
{
	Q_ASSERT( m_left.size() == m_right.size() );

	return m_left.size();
}

template <typename X, typename Y>
void BiMap<X, Y>::clear()
{
	m_left.clear();
	m_right.clear();
}

template <typename X, typename Y>
bool BiMap<X, Y>::isEmpty()
{
	Q_ASSERT( m_left.isEmpty() == m_right.isEmpty() );

	return m_left.isEmpty();
}

template <typename X, typename Y>
bool BiMap<X, Y>::insert(const X& left, const Y& right)
{
	if (containsLeft( left ) || containsRight( right )) {
		return false;
	}

	m_left.insert( left, right );
	m_right.insert( right, left );

	return true;
}

template <typename X, typename Y>
bool BiMap<X, Y>::removeByLeft(const X& left)
{
	if (!containsLeft( left )) {
		return false;
	}

	const Y& right = m_left.value( left );

	Q_ASSERT( m_right.contains( right ) );

	m_left.remove( left );
	m_right.remove( right );

	return true;
}

template <typename X, typename Y>
bool BiMap<X, Y>::removeByRight(const Y& right)
{
	if (!containsRight( right )) {
		return false;
	}

	const X& left = m_right.value( right );

	Q_ASSERT( m_left.contains( left ) );

	m_left.remove( left );
	m_right.remove( right );

	return true;
}

template <typename X, typename Y>
bool BiMap<X, Y>::containsLeft(const X& left)
{
	return m_left.contains( left );
}

template <typename X, typename Y>
bool BiMap<X, Y>::containsRight(const Y& right)
{
	return m_right.contains( right );
}

template <typename X, typename Y>
const X BiMap<X, Y>::left(const Y& right)
{
	return m_right.value( right );
}

template <typename X, typename Y>
const Y BiMap<X, Y>::right(const X& left)
{
	return m_left.value( left );
}

template <typename X, typename Y>
QList<X> BiMap<X, Y>::leftKeys()
{
	return m_left.keys();
}

template <typename X, typename Y>
QList<Y> BiMap<X, Y>::rightKeys()
{
	return m_right.keys();
}

template <typename X, typename Y>
bool BiMap<X, Y>::operator!=(const BiMap<X, Y>& other)
{
	return m_left != other.m_left;
}

template <typename X, typename Y>
bool BiMap<X, Y>::operator==(const BiMap<X, Y>& other)
{
	return m_left == other.m_left;
}

template <typename X, typename Y>
BiMap<X, Y>& BiMap<X, Y>::operator=(const BiMap<X, Y>& other)
{
	m_left = other.m_left;
	m_right = other.m_right;

	return *this;
}

#endif // BIMAP_H
