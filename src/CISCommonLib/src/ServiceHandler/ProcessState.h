#ifndef PROCESSSTATE_H
#define PROCESSSTATE_H


namespace SkyHobbit
{
namespace Common
{
namespace ServiceControl
{

class ProcessState
{
public:
	enum Enum {
		NotRunning = 0,
		IsRunnung = 1,
		Error = 2
	};
};

} // namespace ServiceControl
} // namespace Common
} // namespace Pw

#endif // PROCESSSTATE_H
