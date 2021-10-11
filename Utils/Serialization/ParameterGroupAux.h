#ifndef PARAMETERGROUPAUX
#define PARAMETERGROUPAUX

#include<vector>

#include"RooRealVar.h"

#include"ParameterGroup.h"

float getParamHigh(const ParameterGroup& p, const std::string& name);
float getParamLow(const ParameterGroup& p, const std::string& name);
float getParamVal(const ParameterGroup& p, const std::string& name);

std::vector<float> getFloatVector(const ParameterGroup& p, const std::string& name);
std::vector<std::string> getStringVector(const ParameterGroup& p, const std::string& name);
std::vector<int> getIntVector(const ParameterGroup& p, const std::string& name);
std::vector<ParameterGroup*> getGroupVector(ParameterGroup& p, const std::string& name);

void setFloatVector(ParameterGroup& p, const std::string& name, const std::vector<float>& vec);
void setStringVector(ParameterGroup& p, const std::string& name,const std::vector<std::string>& vec);
void setIntVector(ParameterGroup& p, const std::string& name,const std::vector<int>& vec);

float getFloatIndex(const ParameterGroup& p, const std::string& name,int index);
std::string getStringIndex(const ParameterGroup& p, const std::string& name,int index);
int getIntIndex(const ParameterGroup& p, const std::string& name,int index);

void setFloatIndex(ParameterGroup& p, const std::string& name,float value,int index);
void setStringIndex(ParameterGroup& p, const std::string& name,const std::string& value,int index);
void setIntIndex(ParameterGroup& p, const std::string& name,int value,int index);

void ParameterWrite( ParameterGroup& p, const RooRealVar& var, const std::string& name);

#endif