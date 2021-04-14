
#include<iostream>

#include"../Serialization/Serialization.h"
#include"../Tester/Tester.h"
//#include"../Helpers/Helpers.h"

using std::cout;
using std::string;

void testStorage(Tester* test)
{
    ParameterGroup group;

    int i=0;
    for (const std::string& p : { "", ".sub1",".sub2", ".sub3" })
    {
        group.setString("string1"+p,"hello"+std::to_string(i));
        group.setFloat("value1"+p,42.0f+i);
        group.setInt("int1"+p,11+i);
        group.setBool("bool1"+p,true);
        i++;
    }

    i=0;
    for(const std::string& p : { "", ".sub1",".sub2" })
    {
        test->assert_eq(group.getString("string1"+p),"hello"+std::to_string(i),"must store string value "+std::to_string(i));
        test->assert_eq(group.getFloat("value1"+p),42.0f+i,"must store float value "+std::to_string(i));
        test->assert_eq(group.getInt("int1"+p),11+i,"must store int value "+std::to_string(i));
        test->assert_eq(group.getBool("bool1"+p),true,"must store bool value "+std::to_string(i));
        i++;
    }
}

void testGroupExtraction(Tester* test)
{
    ParameterGroup group;
    group.setString("sub1.value1","23");
    group.setString("sub2.trig.cos","cosine");
    group.setFloat("sub2.trig.sin",23.0f);
    group.setInt("sub2.value4",42);
    group.setString("value3","12");

    ParameterGroup* subgroup = group.get("sub2.trig");

    test->assert_eq(subgroup->getString("cos"),"cosine", "must have subgroup string variable");
    test->assert_eq(subgroup->getFloat("sin"),23.0f, "must have subgroup float variable");

    ParameterGroup* subgroup2 = group.get("sub2");
    test->assert_eq(subgroup2->getString("trig.cos"),"cosine", "must have subgroup string variable");
    test->assert_eq(subgroup2->getFloat("trig.sin"),23.0f, "must have subgroup float variable");
    test->assert_eq(subgroup2->getInt("value4"),42,"must have subgroup int variable");
}

int main()
{
    Tester test("Utils test");
    test.test(testStorage,"variable storage");
    test.test(testGroupExtraction,"group extraction");
    return 0;
}

