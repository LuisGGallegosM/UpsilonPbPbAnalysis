
#include<iostream>

#include"../Serialization/Serialization.h"
//#include"../Helpers/Helpers.h"

using std::cout;

void Assert(bool predicate,const char* behavor, const char* name)
{
    if(predicate)
    {
        cout << "test :"<< name <<" : " << behavor << " \t: passed\n";
    }
    else
    {
        cout << "test :"<< name <<" : " << behavor << " \t: FAILED\n";
    }
}

void testStorage(const char* name)
{
    ParameterGroup group("group");
    group.setString("sub1.value1","hello");
    group.setValue("sub1.value2",42);

    Assert(group.getString("sub1.value1")=="hello","must store string value",name);
    Assert(group.getValue("sub1.value2")==42,"must store float value",name);
}

void testGroupExtraction(const char* name)
{
    ParameterGroup group("group");
    group.setString("sub1.value1","23");
    group.setString("sub2.trig.cos","cosine");
    group.setValue("sub2.trig.sin",23);

    ParameterGroup subgroup = group.get("sub2.trig","sub2.trig");

    Assert(subgroup.getString("cos")=="cosine", "must have subgroup string variable",name);
    Assert(subgroup.getValue("sin")==23, "must have subgroup float variable",name);
}

int main()
{
    std::cout << "start testing\n";

    testStorage("variable storage");
    testGroupExtraction("group extraction");

    std::cout << "end testing\n";

    return 0;
}

