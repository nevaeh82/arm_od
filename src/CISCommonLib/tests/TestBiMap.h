#ifndef TESTBIMAP_H
#define TESTBIMAP_H

#include <cxxtest/TestSuite.h>
#include "Templates/BiMap.h"

class TestBiMap: public CxxTest::TestSuite
{

public:
	// WHEN Inserting a new item to clean BiMap
	// THEN Item should inserted and map size increase by one
	void testInsertAndSize()
	{
		BiMap<int, int> map;

		TS_ASSERT_EQUALS( 0, map.size() );
		TS_ASSERT_EQUALS( 0, map.count() );

		bool result = map.insert( 1, 2 );

		TS_ASSERT( result );
		TS_ASSERT_EQUALS( 1, map.size() );
		TS_ASSERT_EQUALS( 1, map.count() );
	}

	// WHEN Inserting a new item to map that already contains left or right part of item
	// THEN Item should not inserted
	void testInsertFailed()
	{
		BiMap<int, int> map;
		bool result;

		map.insert( 1, 2 );
		TS_ASSERT_EQUALS( 1, map.size() );

		result = map.insert( 1, 3 );
		TS_ASSERT_EQUALS( false, result );
		TS_ASSERT_EQUALS( 1, map.size() );

		result = map.insert( 3, 2 );
		TS_ASSERT_EQUALS( false, result );
		TS_ASSERT_EQUALS( 1, map.size() );

		result = map.insert( 2, 1 );
		TS_ASSERT_EQUALS( true, result );
		TS_ASSERT_EQUALS( 2, map.size() );

		result = map.insert( 3, 4 );
		TS_ASSERT_EQUALS( true, result );
		TS_ASSERT_EQUALS( 3, map.size() );
	}

	// WHEN Clear map
	// THEN Size should be equals zero
	void testClear()
	{
		BiMap<int, int> map;

		map.insert( 1, 2 );
		map.insert( 2, 3 );
		map.insert( 3, 4 );
		TS_ASSERT_EQUALS( 3, map.size() );

		map.clear();
		TS_ASSERT_EQUALS( 0, map.size() );
	}

	// WHEN Map items count equals zero
	// THEN isEmpty should return true
	void testIsEmpty()
	{
		BiMap<int, int> map;

		TS_ASSERT_EQUALS( 0, map.size() );
		TS_ASSERT_EQUALS( true, map.isEmpty() );
		TS_ASSERT_EQUALS( true, map.empty() );
	}

	// WHEN Map items count not equals zero
	// THEN isEmpty should return false
	void testIsNotEmpty()
	{
		BiMap<int, int> map;

		map.insert( 1, 2 );
		map.insert( 2, 3 );
		map.insert( 3, 4 );

		TS_ASSERT_EQUALS( 3, map.size() );
		TS_ASSERT_EQUALS( false, map.isEmpty() );
		TS_ASSERT_EQUALS( false, map.empty() );
	}

	// WHEN Item inserted
	// THEN Item should exists in a map
	void testContains()
	{
		BiMap<int, int> map;

		map.insert( 1, 2 );
		map.insert( 2, 3 );
		map.insert( 3, 4 );

		TS_ASSERT_EQUALS( false, map.containsLeft( 0 ) );
		TS_ASSERT_EQUALS( true, map.containsLeft( 1 ) );
		TS_ASSERT_EQUALS( true, map.containsLeft( 2 ) );
		TS_ASSERT_EQUALS( true, map.containsLeft( 3 ) );
		TS_ASSERT_EQUALS( false, map.containsLeft( 4 ) );

		TS_ASSERT_EQUALS( false, map.containsRight( 1 ) );
		TS_ASSERT_EQUALS( true, map.containsRight( 2 ) );
		TS_ASSERT_EQUALS( true, map.containsRight( 3 ) );
		TS_ASSERT_EQUALS( true, map.containsRight( 4 ) );
		TS_ASSERT_EQUALS( false, map.containsRight( 5 ) );
	}

	// WHEN Item removed by left key and siutable item exists in a map
	// THEN Map should remove it
	void testRemoveByLeft()
	{
		BiMap<int, int> map;

		map.insert( 1, 2 );
		map.insert( 2, 3 );
		map.insert( 3, 4 );
		TS_ASSERT_EQUALS( 3, map.size() );

		map.removeByLeft( 2 );
		TS_ASSERT_EQUALS( 2, map.size() );
		TS_ASSERT_EQUALS( false, map.containsLeft( 2 ) );
	}

	// WHEN Item removed by right key and siutable item exists in a map
	// THEN Map should remove it
	void testRemoveByRight()
	{
		BiMap<int, int> map;

		map.insert( 1, 2 );
		map.insert( 2, 3 );
		map.insert( 3, 4 );
		TS_ASSERT_EQUALS( 3, map.size() );

		map.removeByRight( 3 );
		TS_ASSERT_EQUALS( 2, map.size() );
		TS_ASSERT_EQUALS( false, map.containsRight( 3 ) );
	}

	// WHEN Nonexistent item removed from map
	// THEN Map should not remove any items
	void testRemoveNonexistent()
	{
		BiMap<int, int> map;

		map.insert( 1, 2 );
		map.insert( 2, 3 );
		map.insert( 3, 4 );
		TS_ASSERT_EQUALS( 3, map.size() );

		map.removeByLeft( 0 );
		map.removeByLeft( 4 );
		map.removeByRight( 1 );
		map.removeByRight( 5 );

		TS_ASSERT_EQUALS( 3, map.size() );
	}

	// WHEN Requested right key exists in a map and user requested left value by right
	// THEN Map should return left value
	void testLeft()
	{
		BiMap<int, int> map;

		map.insert( 1, 2 );
		map.insert( 2, 3 );
		map.insert( 3, 4 );

		TS_ASSERT_EQUALS( 1, map.left( 2 ) );
		TS_ASSERT_EQUALS( 2, map.left( 3 ) );
		TS_ASSERT_EQUALS( 3, map.left( 4 ) );
	}

	// WHEN Requested left key exists in a map and user requested right value by left
	// THEN Map should return right value
	void testRight()
	{
		BiMap<int, int> map;

		map.insert( 1, 2 );
		map.insert( 2, 3 );
		map.insert( 3, 4 );

		TS_ASSERT_EQUALS( 2, map.right( 1 ) );
		TS_ASSERT_EQUALS( 3, map.right( 2 ) );
		TS_ASSERT_EQUALS( 4, map.right( 3 ) );
	}

	// WHEN Requested item by key and item not exists
	// THEN Map should return default contructed value
	void testGetNotExistsValue()
	{
		BiMap<int, QString> map;

		map.insert( 1, "2" );

		TS_ASSERT_EQUALS( 0, map.left( "3" ) );
		TS_ASSERT_EQUALS( "", map.right( 2 ) );
	}

	// WHEN Requested list of keys
	// THEN Map should return list of requested keys
	void testKeys()
	{
		BiMap<int, int> map;

		map.insert( 1, 2 );
		map.insert( 2, 3 );
		map.insert( 3, 4 );

		QList<int> left = map.leftKeys();
		QList<int> right = map.rightKeys();

		TS_ASSERT_EQUALS( 3, left.size() );
		TS_ASSERT_EQUALS( 1, left[0] );
		TS_ASSERT_EQUALS( 2, left[1] );
		TS_ASSERT_EQUALS( 3, left[2] );

		TS_ASSERT_EQUALS( 3, right.size() );
		TS_ASSERT_EQUALS( 2, right[0] );
		TS_ASSERT_EQUALS( 3, right[1] );
		TS_ASSERT_EQUALS( 4, right[2] );
	}

	// WHEN Compares two maps
	// THEN Operation should return true if maps are equals, otherwise - false
	void testOperationEquals()
	{
		BiMap<int, int> map1;

		map1.insert( 1, 2 );
		map1.insert( 2, 3 );
		map1.insert( 3, 4 );

		BiMap<int, int> map2;

		map2.insert( 1, 2 );
		map2.insert( 2, 3 );
		map2.insert( 3, 4 );

		BiMap<int, int> map3;

		map3.insert( 1, 2 );
		map3.insert( 2, 3 );

		TS_ASSERT_EQUALS( true, map1 == map2 );
		TS_ASSERT_EQUALS( false, map1 == map3 );

		TS_ASSERT_EQUALS( true, map1 != map3 );
		TS_ASSERT_EQUALS( false, map1 != map2 );
	}

	// WHEN Copy one map into other
	// THEN Maps should be equals
	void testOperationCopy()
	{
		BiMap<int, int> map1;

		map1.insert( 1, 2 );
		map1.insert( 2, 3 );
		map1.insert( 3, 4 );

		BiMap<int, int> map2;
		map2 = map1;

		TS_ASSERT_EQUALS( true, map1 == map2 );
	}

	// WHEN Creates a map by copy constructor
	// THEN Maps should be equals
	void testCopyConstructor()
	{
		BiMap<int, int> map1;

		map1.insert( 1, 2 );
		map1.insert( 2, 3 );
		map1.insert( 3, 4 );

		BiMap<int, int> map2 = map1;

		TS_ASSERT_EQUALS( true, map1 == map2 );
	}
};

#endif // TESTBIMAP_H
