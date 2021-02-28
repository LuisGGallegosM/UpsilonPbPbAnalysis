
#ifndef TREECUTTER
#define TREECUTTER

#include "RtypesCore.h"
#include <iostream>

template<class T>
class TreeCutter
{
    public:

    virtual bool cut(const T* input, Int_t index,Int_t entry)=0;
    virtual bool prescale(Int_t entry) const =0;
    virtual std::string getName() const = 0;

    virtual ~TreeCutter() {};
};

#endif

