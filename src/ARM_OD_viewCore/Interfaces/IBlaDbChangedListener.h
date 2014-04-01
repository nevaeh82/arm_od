#ifndef IBLADBCHANGEDLISTENER_H
#define IBLADBCHANGEDLISTENER_H

struct Bla;
struct BlaInfo;

class IBlaDbChangedListener{

	virtual ~IBlaDbChangedListener(){}

	virtual void onBlaAdded(const Bla& bla) = 0;
	virtual void onBlaRemoved(const Bla& bla) = 0;

	virtual void onBlaInfoChanged(const BlaInfo& blaInfo);
};

#endif // IBLADBCHANGEDLISTENER_H