#include "DbBlaController.h"

DbBlaController::DbBlaController(QObject *parent) :
	DbControllerBase("DBBLACONNECTION", "QMYSQL", parent)
{

}

bool DbBlaController::addBla(const Bla&)
{
	return false;
}

bool DbBlaController::addBla(const Bla&, const BlaInfo&)
{
	return false;
}

bool DbBlaController::addBlaInfo(const BlaInfo&)
{
	return false;
}

bool DbBlaController::addDevice(const Device&)
{
	return false;
}

bool DbBlaController::addBlaMission(const BlaMission&)
{
	return false;
}

bool DbBlaController::addTarget(const Target&)
{
	return false;
}
