#ifndef IDBBLACONTROLLER_H
#define IDBBLACONTROLLER_H

struct Bla;
struct BlaInfo;
struct Device;
struct BlaMission;
struct Target;
struct DBConnectionStruct;

class IDbBlaController
{
	public:
		virtual ~IDbBlaController(){}

		virtual bool addBla(const Bla&) = 0;
		virtual bool addBla(const Bla&, const BlaInfo&) = 0;

		virtual bool addBlaInfo(const BlaInfo&) = 0;

		virtual bool addDevice(const Device&) = 0;

		virtual bool addBlaMission(const BlaMission&) = 0;
		virtual bool addTarget(const Target&) = 0;
};

#endif // IDBBLACONTROLLER_H
