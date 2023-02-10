
#include"FitParams.h"

const std::map<std::string,BkgType> bkgNames =
{
    {"none",BkgType::none},
    {"expchev",BkgType::expchev},
    {"expcheb",BkgType::expchev},
     {"chev",BkgType::chev},
     {"cheb",BkgType::chev},
    {"special",BkgType::special},
    {"exponential",BkgType::exponential}
};

BkgType getBkgByName(const std::string& name)
{
    for(const auto& bkgname : bkgNames )
    {
        if(name.substr(0,bkgname.first.size())==bkgname.first)
        {
            return bkgname.second;
        }
    }

    throw std::invalid_argument("ERROR, background type not supported.");
}