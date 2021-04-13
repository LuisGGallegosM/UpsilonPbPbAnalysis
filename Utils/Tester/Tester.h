#ifndef TESTER
#define TESTER

#include<iostream>

std::string redText(const std::string& text);
std::string greenText(const std::string& text);

class Tester
{
    std::string suitName;
    public:
    Tester(const char* name) : suitName(name) {}

    void test(void (*func)(Tester*), const char* testName);

    void assert(bool predicate,const std::string& behavor);
    template<typename T1,typename T2>
    void assert_eq(T1 value,T2 expected,std::string behavor)
    {
        if(value==expected)
        {
            std::cout << "\ttest :"<< behavor << " \t: "<< greenText("passed") <<"\n";
        }
        else
        {
            std::cout << "\ttest :"<< behavor << " \t: "<< redText("FAILED");
            std::cout << " - Expected: '"<< expected << "' Result: '" << value <<"'\n";
        }
    }

};

#endif