
#include"Tester.h"

void Tester::test(void (*func)(Tester*), const char* testName)
{
    std::cout << "testing :"<< testName << "\n";
    try
    {
        func(this);
    }
    catch(const std::exception& e)
    {
        std::cout << redText(std::string("\tFAILED - exception: ")+e.what()+"\n");
    }
}

void Tester::assert(bool predicate,const std::string& behavor)
{
    if(predicate)
    {
        std::cout << "\ttest :"<< behavor << " \t: "<< greenText("passed") <<"\n";
    }
    else
    {
        std::cout << "\ttest :"<< behavor << " \t: "<< redText("FAILED") << " - result FALSE";
    }
}

std::string redText(const std::string& text)
{
    return "\033[1;31m"+text+"\033[0m";
}

std::string greenText(const std::string& text)
{
    return "\033[1;32m"+text+"\033[0m";
}