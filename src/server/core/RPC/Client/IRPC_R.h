#ifndef IRPC_R_H
#define IRPC_R_H

class IRPC_R
{
public:
    virtual ~IRPC_R(){};

    virtual int start()     = 0;
    virtual int stop()      = 0;

};

#endif // IRPC_H
