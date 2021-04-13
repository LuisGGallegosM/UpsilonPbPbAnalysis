
#include<iostream>

#include"../Serialization/Serialization.h"
#include"../Tester/Tester.h"
//#include"../Helpers/Helpers.h"

using std::cout;
using std::string;

void testStorage(Tester* test)
{
    ParameterGroup group("group");
    group.setString("sub1.value1","hello");
    group.setValue("sub1.value2",42.0f);

    test->assert_eq(group.getString("sub1.value1"),"hello","must store string value");
    test->assert_eq(group.getValue("sub1.value2"),42.0f,"must store float value");
}

void testGroupExtraction(Tester* test)
{
    ParameterGroup group("group");
    group.setString("sub1.value1","23");
    group.setString("sub2.trig.cos","cosine");
    group.setValue("sub2.trig.sin",23.0f);

    ParameterGroup subgroup = group.get("sub2.trig","sub2.trig");

    test->assert_eq(subgroup.getString("cos"),"cosine", "must have subgroup string variable");
    test->assert_eq(subgroup.getValue("sin"),23.0f, "must have subgroup float variable");
}

int main()
{
    Tester test("Utils test");
    test.test(testStorage,"variable storage");
    test.test(testGroupExtraction,"group extraction");
    return 0;
}

