#include<iostream>

#include"../../Utils/Serialization/Serialization.h"
#include"../../Utils/Tester/Tester.h"

void testFit(Tester* t);

int main()
{
    Tester test("Fitting test");
    testFit(&test);
    return 0;
}

