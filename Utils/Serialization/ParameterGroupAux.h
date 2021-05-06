#ifndef PARAMETERGROUPAUX
#define PARAMETERGROUPAUX

#include<vector>

#include"ParameterGroup.h"

std::vector<float> getFloatVector(const ParameterGroup& p, const std::string& name);
std::vector<std::string> getStringVector(const ParameterGroup& p, const std::string& name);
std::vector<int> getIntVector(const ParameterGroup& p, const std::string& name);

#endif