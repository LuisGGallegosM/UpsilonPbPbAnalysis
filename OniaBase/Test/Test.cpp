
#include"../Params/Params.h"
#include"../../Utils/Serialization/Serialization.h"
#include"../../Utils/Tester/Tester.h"

void CutParamTest(Tester* tester)
{
    CutParams cut;
    cut.deserialize("./OniaBase/Test/Files/sampleCut.cutconf");
    
    tester->assert_eq( cut.isMC,false,"isMC deserialized");
    tester->assert_eq( cut.checkSign, true,"checkSign deserialized");
    tester->assert_eq( cut.ptLow,0.0,"ptLow deserialized");
    tester->assert_eq( cut.ptHigh, 100.0f,"ptHigh deserialized");
    tester->assert_eq( cut.yLow, 0.0f,"yLow deserialized");
    tester->assert_eq( cut.yHigh, 2.4f, "yHigh deserialized");
}

void ParamsTest()
{
    Tester test("Params");
    test.test(CutParamTest,"CutParams");
}

int main()
{
    ParamsTest();

}