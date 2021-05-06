
#include "Tester.h"
#include<iomanip>

Tester::Tester(const char* name) : suitName(name)
{
    numOfTestSuits=0;
    numOfTestSuitsSuccess=0;
    numOfTest = 0;
    numOfTestSuccess = 0;
    numOfTotalTest=0;
    numOfTotalTestSuccess=0;
    std::cout << "\tTesting: "<< name << "\n";
}

void Tester::test(void (*func)(Tester*), const char* testName)
{
    std::cout << "test suit : "<< testName << "\n";
    ++numOfTestSuits;
    try
    {
        numOfTestSuccess=0;
        numOfTest=0;
        func(this);
        textColor c = (numOfTestSuccess==numOfTest) ? textColor::green : textColor::red;
        std::string str=std::to_string(numOfTestSuccess)+" / " + std::to_string(numOfTest);
        std::cout << "\tTests ran " <<  colorText(str,c) << " Passed\n" ;
        ++numOfTestSuitsSuccess;
    }
    catch(const std::exception& e)
    {
        std::cout << colorText(std::string("\tFAILED - exception: ")+e.what()+"\n",textColor::red);
    }
}

void Tester::showPassed(const std::string& behavor)
{
    std::cout << "\ttest :"<< behavor << std::setw(60-behavor.length())<< colorText("passed",textColor::green) <<"\n";
    ++numOfTestSuccess;
    ++numOfTest;
    ++numOfTotalTest;
    ++numOfTotalTestSuccess;
}

void Tester::showFailed(const std::string& behavor, const std::string& info)
{
    std::cout << "\ttest :"<< behavor << std::setw(60-behavor.length()) << colorText("FAILED",textColor::red) << " - " << info << "\n";
    ++numOfTestSuits;
    ++numOfTotalTest;
}

bool Tester::assert_true(bool predicate,const std::string& behavor)
{
    if(predicate)
        showPassed(behavor);
    else
        showFailed(behavor,"result FALSE");
    return predicate;
}

template<typename T>
std::string expectString(T value,T expected,const std::string& type)
{
    return " type:"+colorText(type,textColor::blue)+" Expected: "+
    colorText(std::to_string(expected),textColor::cyan) + 
    " Result: " +colorText(std::to_string(value),textColor::red);
}

std::string expectString(const std::string& value,const std::string& expected,const std::string& type)
{
    return " type:"+colorText(type,textColor::blue)+" Expected: \""+
    colorText(expected,textColor::cyan) + "\" Result: \"" +
    colorText(value,textColor::red)+"\"";
}

std::string expectString(bool value, bool expected,const std::string& type)
{
    return " type:"+colorText(type,textColor::blue)+" Expected: "+
    colorText((value? "true" : "false"),textColor::blue) + 
    " Result: " +colorText((expected? "true" : "false"),textColor::red);
}

void Tester::assert_eq(bool value,bool expected,const std::string& behavor)
{
    if(value==expected)
        showPassed(behavor);
    else
        showFailed(behavor,expectString(value,expected,"boolean"));
}

void Tester::assert_eq(int value,int expected,const std::string& behavor)
{
    if(value==expected)
        showPassed(behavor);
    else
        showFailed(behavor,expectString(value,expected,"int"));
}

void Tester::assert_eq(float value,float expected,const std::string& behavor)
{
    if(value==expected)
        showPassed(behavor);
    else
        showFailed(behavor,expectString(value,expected,"float"));
}

void Tester::assert_eq(const std::string& value,const std::string& expected,const std::string& behavor)
{
    if(value==expected)
        showPassed(behavor);
    else
        showFailed(behavor,expectString(value,expected,"string"));
}

Tester::~Tester()
{
    textColor c = (numOfTestSuitsSuccess==numOfTestSuits) ? textColor::green : textColor::red;
    std::string str=std::to_string(numOfTotalTestSuccess)+" / " + std::to_string(numOfTotalTest);
    std::string str2=std::to_string(numOfTestSuitsSuccess)+" / " + std::to_string(numOfTestSuits);

    std::cout << "Total tests ran : "  <<  colorText(str,c) << " Passed\n" ;
    std::cout << "Test suites ran : " <<  colorText(str2,c) << " Passed\n\n" ;
}

std::string colorText(const std::string& text, textColor color)
{
    return "\033[1;"+ std::to_string(int(color))+"m"+text+"\033[0m";
}