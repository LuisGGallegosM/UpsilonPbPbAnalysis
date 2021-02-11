
#ifndef CUTTER
#define CUTTER

#include "RtypesCore.h"

template<class T>
class TreeCutter
{
    public:

    virtual bool cut(T* input, Int_t index,Int_t entry)=0;
    virtual bool prescale(Int_t entry) const =0;

    virtual ~TreeCutter() {};
};

#endif

