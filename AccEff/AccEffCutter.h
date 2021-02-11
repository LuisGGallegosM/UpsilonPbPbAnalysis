
#ifndef ACCEFFCUTTER
#define ACCEFFCUTTER

#include "AccEffInput.h"
#include "../TreeProcessor/TreeCutter.h"
#include "../Utils/SkimParams.h"

class AccEffCutter : public TreeCutter<AccEffInput>
{
    private:
    cutParams kineCut;

    public:
    bool cut(AccEffInput* input, Int_t index,Int_t entry) override;
    bool prescale(Int_t entry) const override { return false; }

    std::string getName() const override {return "Acceptancy";};
    
    AccEffCutter(const cutParams* cut);
};

#if defined(__CLING__)
#include "OniaCutter.cpp"
#endif

#endif