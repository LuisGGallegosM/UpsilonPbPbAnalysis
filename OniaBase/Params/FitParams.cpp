
#include"FitParams.h"

const std::map<std::string,BkgType> bkgNames =
{
    {"none",BkgType::none},
     {"chev",BkgType::chev},
    {"special",BkgType::special},
    {"exponential",BkgType::exponential},
    {"expChev2",BkgType::expChev2}
};

BkgType getBkgByName(const std::string& name)
{
    return bkgNames.at(name);
}