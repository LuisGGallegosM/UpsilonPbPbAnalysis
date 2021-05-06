#ifndef TESTER
#define TESTER

#include<iostream>

enum class textColor { red=31, green, orange, blue, purple,cyan };

std::string colorText(const std::string& text, textColor color);

class Tester
{
    std::string suitName;
    int numOfTestSuits;
    int numOfTestSuitsSuccess;
    int numOfTest;
    int numOfTestSuccess;
    int numOfTotalTest;
    int numOfTotalTestSuccess;
    
    public:
    Tester(const char* name);

    void test(void (*func)(Tester*), const char* testName);

    bool assert_true(bool predicate,const std::string& behavor);

    void assert_eq(bool value, bool expected, const std::string& behavor);
    void assert_eq(int value, int expected, const std::string& behavor);
    void assert_eq(float value, float expected, const std::string& behavor);
    void assert_eq(const std::string& value, const std::string& expected, const std::string& behavor);

    void showPassed(const std::string& behavor);
    void showFailed(const std::string& behavor, const std::string& info);
    ~Tester();
};

#endif