#ifndef TESTRPCROUTEDCLIENT_H
#define TESTRPCROUTEDCLIENT_H

#include <cxxtest/TestSuite.h>
#include <cxxmock/cxxmock.h>

#include <Rpc/RpcRoutedClient.h>
#include <Rpc/RpcPeerAbstract.h>

#include "Mocks.generated.h"

class TestRpcRoutedClient: public CxxTest::TestSuite
{
public:
	CxxMock::Repository* mocks;
	RpcPeerAbstract *peer;
	RpcRoutedClient *client;

	void setUp()
	{
		mocks = new CxxMock::Repository();
		peer = mocks->create<RpcPeerAbstract>();

		client = new RpcRoutedClient();
		client->m_clientPeer = peer;
	}

	void tearDown()
	{
		delete client;
		delete peer;
		delete mocks;
	}

	// WHEN Send register route command first time
	// THEN Method should execte successfull
	void testRegisterRoute()
	{
		TS_EXPECT_CALL( client->m_clientPeer->isClient() ).returns( true );
		TS_EXPECT_CALL_VOID( client->m_clientPeer->call( RPC_METHOD_REGISTER_CLIENT,
														 QVariant( 10 ) ) );

		mocks->replay();

		bool result = client->registerRoute( 10 );
		TS_ASSERT( result );

		mocks->verify();
	}

	// WHEN Called register route command with zero route id
	// THEN Method should not executed
	void testRegisterRouteZero()
	{
		mocks->replay();

		bool result = client->registerRoute( 0 );
		TS_ASSERT( !result );

		mocks->verify();
	}

	// WHEN Called register route command but client not connected yet
	// THEN Method should not executed
	void testRegisterRouteNotConnected()
	{
		TS_EXPECT_CALL( client->m_clientPeer->isClient() ).returns( false );

		mocks->replay();

		bool result = client->registerRoute( 10 );
		TS_ASSERT( !result );

		mocks->verify();
	}

	// WHEN Send deregister route command
	// THEN Method should use method call
	void testDeregisterRoute()
	{
		TS_EXPECT_CALL( client->m_clientPeer->isClient() ).returns( true );
		TS_EXPECT_CALL( client->m_clientPeer->isClient() ).returns( true );
		TS_EXPECT_CALL( client->m_clientPeer->isClient() ).returns( true );

		TS_EXPECT_CALL_VOID( client->m_clientPeer->call( RPC_METHOD_REGISTER_CLIENT,
														 QVariant( 20 ) ) );
		TS_EXPECT_CALL_VOID( client->m_clientPeer->call( RPC_METHOD_DEREGISTER_CLIENT ) );
		TS_EXPECT_CALL_VOID( client->m_clientPeer->call( RPC_METHOD_REGISTER_CLIENT,
														 QVariant( 20 ) ) );

		mocks->replay();

		bool result;
		client->registerRoute(20);
		client->deregisterRoute();
		result = client->registerRoute(20);
		TS_ASSERT( result );

		mocks->verify();
	}

	// WHEN Called deregister route with zero route
	// THEN Method not should use method call
	void testDeregisterRouteZero()
	{
		mocks->replay();
		client->deregisterRoute();
		mocks->verify();
	}

	// WHEN Called deregister route with offline client
	// THEN Method not should use method call
	void testDeregisterRouteNotConnected()
	{
		TS_EXPECT_CALL( client->m_clientPeer->isClient() ).returns( true );
		TS_EXPECT_CALL( client->m_clientPeer->isClient() ).returns( false );

		TS_EXPECT_CALL_VOID( client->m_clientPeer->call( RPC_METHOD_REGISTER_CLIENT,
														 QVariant( 10 ) ) );

		mocks->replay();
		client->registerRoute(10);
		client->deregisterRoute();
		mocks->verify();
	}

	// WHEN Send register route command second time
	// THEN Method should execte deregister command first
	void testRegisterRouteTwoTimes()
	{
		TS_EXPECT_CALL( client->m_clientPeer->isClient() ).returns( true );
		TS_EXPECT_CALL( client->m_clientPeer->isClient() ).returns( true );
		TS_EXPECT_CALL( client->m_clientPeer->isClient() ).returns( true );

		TS_EXPECT_CALL_VOID( client->m_clientPeer->call( RPC_METHOD_REGISTER_CLIENT,
														 QVariant( 10 ) ) );
		TS_EXPECT_CALL_VOID( client->m_clientPeer->call( RPC_METHOD_DEREGISTER_CLIENT ) );
		TS_EXPECT_CALL_VOID( client->m_clientPeer->call( RPC_METHOD_REGISTER_CLIENT,
														 QVariant( 10 ) ) );

		mocks->replay();

		bool result;
		result = client->registerRoute( 10 );
		result = client->registerRoute( 10 );
		TS_ASSERT( result );

		mocks->verify();
	}
};

#endif // TESTRPCROUTEDCLIENT_H
