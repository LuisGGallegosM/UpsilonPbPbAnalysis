
#include"FitParams.h"

const std::map<std::string,BkgType> bkgNames =
{
    {"none",BkgType::none},
     {"chev",BkgType::chev},
    {"special",BkgType::special},
    {"exp",BkgType::exponential},
    {"expchev",BkgType::expchev}
};

BkgType getBkgByName(const std::string& name)
{
    return bkgNames.at(name);
}