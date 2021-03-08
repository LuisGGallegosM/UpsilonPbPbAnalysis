#ifndef SKIMMER
#define SKIMMER

class Skimmer
{
    public:
    virtual void Write() = 0;
    virtual void Skim() = 0;

    virtual ~Skimmer() = default;
};

template<typename Reader>
class Cutter
{
    public:
    virtual bool cut(const Reader* input, Int_t index,Int_t entry) = 0;
    virtual ~Cutter() = default;
};

#endif


