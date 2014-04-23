#ifndef IUAVHISTORYLISTENER_H
#define IUAVHISTORYLISTENER_H

#include "Interfaces/IUavDbChangedListener.h"

class IUavHistoryListener : public IUavDbChangedListener
{
	public:
		enum Status {
			NotReady,
			Ready,
			Playing,
			Loading
		};

		virtual ~IUavHistoryListener() {}
		virtual void onStatusChanged(Status status) = 0;
};

#endif // IUAVHISTORYLISTENER_H
