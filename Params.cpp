
#include "Params.h"

using namespace std;

void serialize(ofstream& file,const char* name,float value)
{
    file << name << " = " << value << "\n";
}

void serialize(ofstream& file,const char* name,bool value)
{
    file << name << " = " <<  (value ? "true" : "false") << "\n";
}

void serialize(ofstream& file,const char* name,int value)
{
    file << name << " = " << value<< "\n";
}

void serialize(ofstream& file,const char* name,ULong64_t value)
{
    file << name << " = " << value<< "\n";
}