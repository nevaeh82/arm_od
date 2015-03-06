#ifndef IRPCCONFIGREADER_H
#define IRPCCONFIGREADER_H

class IRpcControllerBase;

class IRpcConfigReader
{
public:
	virtual ~IRpcConfigReader(){}

	virtual void setRpcServer(IRpcControllerBase* rpcServer)	= 0;
};

#endif // IRPCCONFIGREADER_H
