#ifndef IBLADBCHANGEDLISTENER_H
#define IBLADBCHANGEDLISTENER_H

struct Bla;
struct BlaInfo;

class IBlaDbChangedListener{

public:
	virtual ~IBlaDbChangedListener(){}

	virtual void onBlaAdded(const Bla& bla) = 0;
	virtual void onBlaRemoved(const Bla& bla) = 0;
	virtual void onBlaInfoChanged(const BlaInfo& blaInfo) = 0;
};

#endif // IBLADBCHANGEDLISTENER_H
