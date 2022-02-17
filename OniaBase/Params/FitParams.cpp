
#include"FitParams.h"

const std::map<std::string,BkgType> bkgNames =
{
    {"none",BkgType::none},
     {"chev",BkgType::chev},
     {"cheb",BkgType::chev},
    {"special",BkgType::special},
    {"exp",BkgType::exponential},
    {"expchev",BkgType::expchev}
};

BkgType getBkgByName(const std::string& name)
{
    for(const auto& bkgname : bkgNames )
    {
        if(name.find(bkgname.first)!=std::string::npos)
        {
            return bkgname.second;
        }
    }

    throw std::invalid_argument("ERROR, background type not supported.");
}