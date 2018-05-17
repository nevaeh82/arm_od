#ifndef TESTRPCROUTEDSERVER_H
#define TESTRPCROUTEDSERVER_H

#include <cxxtest/TestSuite.h>
#include <cxxmock/cxxmock.h>

#include <Rpc/RpcRoutedServer.h>
#include <Rpc/RpcPeerAbstract.h>

#include "Mocks.generated.h"

class TestRpcRoutedServer: public CxxTest::TestSuite
{
public:
	CxxMock::Repository* mocks;
	RpcPeerAbstract *peer;
	RpcRoutedServer *server;

	void setUp()
	{
		mocks = new CxxMock::Repository();
		peer = mocks->create<RpcPeerAbstract>();

		server = new RpcRoutedServer();
		server->m_serverPeer = peer;
	}

	void tearDown()
	{
		delete server;
		delete peer;
		delete mocks;
	}

	// WHEN User try to register first receiver
	// THEN Reciever should be registered
	void testRegisterReceiver()
	{
		bool result;
		IRpcListener *receiver = mocks->create<IRpcListener>();

		result = server->registerReceiver( receiver, 1 );
		TS_ASSERT( result );

		delete receiver;
	}

	// WHEN User try to register NULL pointer
	// THEN Reciever should not be registered
	void testRegisterReceiverNULL()
	{
		bool result = server->registerReceiver( NULL, 1 );
		TS_ASSERT( !result );
	}

	// WHEN User try to register two different receivers on two different routes
	// THEN Recievers should be registered
	void testRegisterReceiverTwoDifferentOnDifferentRoutes()
	{
		bool result;
		IRpcListener *receiver1 = mocks->create<IRpcListener>();
		IRpcListener *receiver2 = mocks->create<IRpcListener>();

		result = server->registerReceiver( receiver1, 1 );
		TS_ASSERT( result );

		result = server->registerReceiver( receiver2, 2 );
		TS_ASSERT( result );

		delete receiver1;
		delete receiver2;
	}

	// WHEN User try to register two different receivers on same route
	// THEN Second reciever should not be registered
	void testRegisterReceiverTwoDifferentOnSameRoute()
	{
		bool result;
		IRpcListener *receiver1 = mocks->create<IRpcListener>();
		IRpcListener *receiver2 = mocks->create<IRpcListener>();

		result = server->registerReceiver( receiver1, 1 );
		TS_ASSERT( result );

		result = server->registerReceiver( receiver2, 1 );
		TS_ASSERT( !result );

		delete receiver1;
		delete receiver2;
	}

	// WHEN User try to register first receiver again
	// THEN Reciever should not be registered
	void testRegisterReceiverTwoTimes()
	{
		bool result;
		IRpcListener *receiver = mocks->create<IRpcListener>();

		result = server->registerReceiver( receiver, 1 );
		TS_ASSERT( result );

		result = server->registerReceiver( receiver, 2 );
		TS_ASSERT( !result );

		delete receiver;
	}

	// WHEN User try to register first client
	// THEN Client should be registered
	void testRegisterClient()
	{
		bool result = server->registerClient( 100, 1 );
		TS_ASSERT( result );
	}

	// WHEN User try to register client with zero id
	// THEN Client should not be registered
	void testRegisterCleintZero()
	{
		bool result = server->registerClient( 0, 1 );
		TS_ASSERT( !result );
	}

	// WHEN User try to register two different clients on two different routes
	// THEN Clients should be registered
	void testRegisterClientTwoDifferentOnDifferentRoutes()
	{
		bool result;

		result = server->registerClient( 100, 1 );
		TS_ASSERT( result );

		result = server->registerClient( 101, 2 );
		TS_ASSERT( result );
	}

	// WHEN User try to register two different clients on same route
	// THEN Second client should not be registered
	void testRegisterClientTwoDifferentOnSameRoute()
	{
		bool result;

		result = server->registerClient( 100, 1 );
		TS_ASSERT( result );

		result = server->registerClient( 101, 1 );
		TS_ASSERT( !result );
	}

	// WHEN User try to register first client again
	// THEN Client should not be registered
	void testRegisterClientTwoTimes()
	{
		bool result;

		result = server->registerClient( 100, 1 );
		TS_ASSERT( result );

		result = server->registerClient( 100, 2 );
		TS_ASSERT( !result );
	}

	// WHEN User dispatch RPC method with client id
	// THEN Method should dispathed only to receiver that registered for this route
	void testDispatch()
	{
		IRpcListener *receiver1 = mocks->create<IRpcListener>();
		IRpcListener *receiver2 = mocks->create<IRpcListener>();

		TS_EXPECT_CALL_VOID( receiver1->onMethodCalled( "method1", QVariant(200)) );
		TS_EXPECT_CALL_VOID( receiver2->onMethodCalled( "method2", QVariant("300")) );
		mocks->replay();

		server->registerReceiver( receiver1, 1 );
		server->registerReceiver( receiver2, 2 );
		server->registerClient( 100, 1 );
		server->registerClient( 101, 2 );
		server->dispatch( "method1", QVariant(200), 100 );
		server->dispatch( "method2", QVariant("300"), 101 );

		mocks->verify();

		delete receiver1;
		delete receiver2;
	}

	// WHEN User dispatch RPC method with client id and there is no receivers for this client
	// THEN Method should not be dispatched
	void testDispatchToNotRegisteredRoute()
	{
		mocks->replay();

		server->registerClient( 100, 1 );
		server->dispatch( "method1", QVariant(200), 100 );

		mocks->verify();
	}

	// WHEN User dispatch RPC method without client id
	// THEN Method should be dispatched to all listeners
	void testDispatchWithoutClientId()
	{
		IRpcListener *receiver1 = mocks->create<IRpcListener>();
		IRpcListener *receiver2 = mocks->create<IRpcListener>();

		TS_EXPECT_CALL_VOID( receiver1->onMethodCalled( "method1", QVariant(200)) );
		TS_EXPECT_CALL_VOID( receiver2->onMethodCalled( "method1", QVariant(200)) );
		mocks->replay();

		server->registerReceiver( receiver1, 1 );
		server->registerReceiver( receiver2, 2 );
		server->registerClient( 100, 1 );
		server->registerClient( 101, 2 );
		server->dispatch( "method1", QVariant(200) );

		mocks->verify();

		delete receiver1;
		delete receiver2;
	}

	// WHEN User deregister existing receiver
	// THEN Method should not be dispatched to this receiver from client, linked with it
	void testDeregisterReceiver()
	{
		IRpcListener *receiver1 = mocks->create<IRpcListener>();
		IRpcListener *receiver2 = mocks->create<IRpcListener>();

		TS_EXPECT_CALL_VOID( receiver1->onMethodCalled( "method1", QVariant(200)) );
		mocks->replay();

		server->registerReceiver( receiver1, 1 );
		server->registerReceiver( receiver2, 2 );
		server->registerClient( 100, 1 );
		server->registerClient( 101, 2 );
		server->deregisterReceiver( receiver2 );
		server->dispatch( "method1", QVariant(200) );
		server->dispatch( "method2", QVariant(300), 101 );

		mocks->verify();

		delete receiver1;
		delete receiver2;
	}

	// WHEN User deregister existing receiver
	// THEN Server should not create a link for this receiver and client after that
	void testDeregisterReceiverAndRegisterClientAfter()
	{
		IRpcListener *receiver = mocks->create<IRpcListener>();

		mocks->replay();

		server->registerReceiver( receiver, 1 );
		server->deregisterReceiver( receiver );
		server->registerClient( 100, 1 );
		server->dispatch( "method1", QVariant(200), 100 );

		mocks->verify();

		delete receiver;
	}

	// WHEN User deregister receiver NULL pointer
	// THEN Nothis happends
	void testDeregisterReceiverNULL()
	{
		server->deregisterReceiver( NULL );
	}



	// WHEN User call RPC method with sender
	// THEN Method should call mathod only for client that registered for this route
	void testCall()
	{
		IRpcListener *receiver1 = mocks->create<IRpcListener>();
		IRpcListener *receiver2 = mocks->create<IRpcListener>();

		TS_EXPECT_CALL_VOID( peer->call( 100, "method1", QVariant(200)) );
		TS_EXPECT_CALL_VOID( peer->call( 101, "method2", QVariant("300")) );
		mocks->replay();

		server->registerReceiver( receiver1, 1 );
		server->registerReceiver( receiver2, 2 );
		server->registerClient( 100, 1 );
		server->registerClient( 101, 2 );
		server->call( "method1", QVariant(200), receiver1 );
		server->call( "method2", QVariant("300"), receiver2 );

		mocks->verify();

		delete receiver1;
		delete receiver2;
	}

	// WHEN User call RPC method with sender and there is no clients registered for this sender
	// THEN Method should not be called
	void testCallToNotRegisteredRoute()
	{
		IRpcListener *receiver = mocks->create<IRpcListener>();

		mocks->replay();

		server->registerReceiver( receiver, 1 );
		server->call( "method1", QVariant(200), receiver );

		mocks->verify();
	}

	// WHEN User call RPC method without sender
	// THEN Method should be called for all clients
	void testCallWithoutSender()
	{
		IRpcListener *receiver1 = mocks->create<IRpcListener>();
		IRpcListener *receiver2 = mocks->create<IRpcListener>();

		TS_EXPECT_CALL_VOID( peer->call( "method1", QVariant(200)) );
		mocks->replay();

		server->registerReceiver( receiver1, 1 );
		server->registerReceiver( receiver2, 2 );
		server->registerClient( 100, 1 );
		server->registerClient( 101, 2 );
		server->call( "method1", QVariant(200) );

		mocks->verify();

		delete receiver1;
		delete receiver2;
	}

	// WHEN User deregister existing client
	// THEN Method should not be called for this receiver from sender, linked with it
	void testDeregisterClient()
	{
		IRpcListener *receiver1 = mocks->create<IRpcListener>();
		IRpcListener *receiver2 = mocks->create<IRpcListener>();

		mocks->replay();

		server->registerReceiver( receiver1, 1 );
		server->registerReceiver( receiver2, 2 );
		server->registerClient( 100, 1 );
		server->registerClient( 101, 2 );
		server->deregisterClient( 101 );
		server->call( "method2", QVariant(300), receiver2 );

		mocks->verify();

		delete receiver1;
		delete receiver2;
	}

	// WHEN User deregister existing client
	// THEN Server should not create a link for this client and receiver after that
	void testDeregisterClientAndRegisterReceiverAfter()
	{
		IRpcListener *receiver = mocks->create<IRpcListener>();

		mocks->replay();

		server->registerClient( 100, 1 );
		server->deregisterClient( 100 );
		server->registerReceiver( receiver, 1 );
		server->dispatch( "method1", QVariant(200), 100 );

		mocks->verify();

		delete receiver;
	}

	// WHEN User deregister client with zero id
	// THEN Nothis happends
	void testDeregisterClientNULL()
	{
		server->deregisterClient( 0 );
	}

	// WHEN Called registerReceiver without route id
	// THEN Reciever should be registered
	void testRegisterReceiverWithoutId()
	{
		bool result;
		IRpcListener *receiver = mocks->create<IRpcListener>();

		TS_EXPECT_CALL_VOID( receiver->onMethodCalled( "method1", QVariant( 100 )) );
		mocks->replay();

		result = server->registerReceiver( receiver );
		server->dispatch( "method1", QVariant( 100 ) );
		TS_ASSERT( result );

		mocks->verify();

		delete receiver;
	}
};

#endif // TESTRPCROUTEDSERVER_H
