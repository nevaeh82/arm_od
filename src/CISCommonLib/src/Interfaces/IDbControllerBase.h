#ifndef IDBCONTROLLERBASE_H
#define IDBCONTROLLERBASE_H

struct DBConnectionStruct;

class IDbControllerBase
{
	public:
		virtual ~IDbControllerBase(){}

		virtual bool connectToDB(const DBConnectionStruct&) = 0;
		virtual void disconnectFromDb() = 0;
};

#endif // IDBCONTROLLERBASE_H
