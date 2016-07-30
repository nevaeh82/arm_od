#include <QByteArray>

#include "SolverPacket1.pb.h"

inline void createCommandSolverClear( SolverProtocol::Packet& pkt )
{
	pkt.mutable_datafromclient()->set_commandtosolver( SolverProtocol::Packet_DataFromClient_CommandToSolver_clearSolver );
}

inline void createCommandSolverStopServer( SolverProtocol::Packet& pkt )
{
	pkt.mutable_datafromclient()->set_commandtosolver( SolverProtocol::Packet_DataFromClient_CommandToSolver_stopServer );
}

inline void createCommandSolverRestartServer( SolverProtocol::Packet& pkt )
{
	pkt.mutable_datafromclient()->set_commandtosolver( SolverProtocol::Packet_DataFromClient_CommandToSolver_restartServer );
}

inline void createCommandSolverStartCalcCorrections( SolverProtocol::Packet& pkt )
{
	pkt.mutable_datafromclient()->set_commandtosolver( SolverProtocol::Packet_DataFromClient_CommandToSolver_startCalculationOfCorrections );
}

inline void createCommandSolverStopCalcCorrections( SolverProtocol::Packet& pkt )
{
	pkt.mutable_datafromclient()->set_commandtosolver( SolverProtocol::Packet_DataFromClient_CommandToSolver_stopCalculationOfCorrections );
}

inline void createCommandSolverResetCalcCorrections( SolverProtocol::Packet& pkt )
{
	pkt.mutable_datafromclient()->set_commandtosolver( SolverProtocol::Packet_DataFromClient_CommandToSolver_resetCalculatedCorrections );
}

inline void createCommandSolverSaveCalcCorrections( SolverProtocol::Packet& pkt )
{
	pkt.mutable_datafromclient()->set_commandtosolver( SolverProtocol::Packet_DataFromClient_CommandToSolver_saveCalculatedCorrections );
}

inline void createCommandSolverSetUser( SolverProtocol::Packet& pkt, SolverProtocol::Packet_DataFromClient_SetRequest_SetClientType& data )
{
	pkt.mutable_datafromclient()->mutable_setrequest()->mutable_setclienttype()->CopyFrom( data );
}

inline void createCommandSolverGetAll( SolverProtocol::Packet& pkt )
{
	pkt.mutable_datafromclient()->add_getrequest( SolverProtocol::Packet_DataFromClient_GetRequest_getAllSettingsFromSolver );
}

//Check has commands
inline bool isSolverMessageCommandSolver( SolverProtocol::Packet& pkt )
{
	if( !pkt.has_datafromsolver() ) {
		return false;
	}

	if( !pkt.datafromsolver().has_message() ) {
		return false;
	}

	return true;
}

inline bool isSolverMessageSolverResponse( SolverProtocol::Packet& pkt )
{
	if( !pkt.has_datafromsolver() ) {
		return false;
	}

	if( !pkt.datafromsolver().has_solverresponse() ) {
		return false;
	}

	return true;
}

inline bool isSolverMessageHasSingleMarksManual( SolverProtocol::Packet& pkt )
{
    if( pkt.has_datafromsolver() && pkt.datafromsolver().has_solution_manual_altitude() &&
        pkt.datafromsolver().solution_manual_altitude().has_singlemarks() ) {
        return true;
    }

    return false;
}

inline bool isSolverMessageHasTrajectoryManual( SolverProtocol::Packet& pkt )
{
    if( pkt.has_datafromsolver() && pkt.datafromsolver().has_solution_manual_altitude() &&
        (pkt.datafromsolver().solution_manual_altitude().trajectory_size() > 0) ) {
        return true;
    }

    return false;
}
