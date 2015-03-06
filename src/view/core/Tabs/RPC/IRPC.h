#ifndef IRPC_H
#define IRPC_H

class IRPC
{
public:
    virtual ~IRPC(){};

    virtual int start()     = 0;
    virtual int stop()      = 0;

};

#endif // IRPC_H
