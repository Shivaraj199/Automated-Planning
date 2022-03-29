#ifndef PARSER_HP
#define PARSER_HP

#include <string>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
class Parser
{
private:
    enum keywords
    {
        domain = 1,
        requirement,
        predicates,
        constants,
        action,
        parameters,
        precondition,
        effect,
        object,
        init,
        goal
    };

    std::map<std::string, keywords> keyword_map;
    std::string currentToken;
    std::string currentAction;
    std::vector<std::string> tStack;  //token stack
    std::vector<std::string> fpStack; //function paranthesis stack
    std::vector<std::string> vpStack; //variable paranthesis stack
    bool insideP = false;             //inside paranthesis
    bool insideAnd;

    std::string precondString;
    bool precondState = true;
    bool precondVarState = true;
    std::string effectString;
    bool effectState = true;
    bool effectVarState = true;
    std::string objectString;
    std::string initString;
    std::string goalString;

    std::string line;
    std::string token;

    //temp varaibles used in the pass by reference functions
    std::string predicateName;
    int no;
    std::string objectName;
    std::string actionname;
    std::string parameter;
    std::string preconditionValue;
    std::string effectname;
    bool boolValue;
    std::vector<std::string> paramnames;

    void replaceAll(std::string &str, const std::string &from, const std::string &to);

    void stack_pop(std::vector<std::string> &stack);

    void check_stack(std::vector<std::string> &stack);

    void mapInit();



   /*Problem*/
    void parsed_init(std::string &actionName, std::vector<std::string> &paramNames);

    void parsed_goal(std::string &actionName, std::vector<std::string> &paramNames);


   /*Domain*/
    void parsed_domainname(std::string &domainname);

    void parsed_predicate(std::string &predicatename, int &no);

    void parsed_actionname(std::string &actionname);

    void parsed_parameter(std::string &actionname, std::string &parameter);

    void parsed_precod(std::string &actionname, std::string &precondname, bool &boolValue, std::vector<std::string> &paramnames);

    void parsed_effect(std::string &actionname, std::string &effectname, bool &boolValue, std::vector<std::string> &paramnames);

    void parsed_when_precod(std::string &precondname, bool &boolValue, std::vector<std::string> &paramnames);

    void parsed_when_effect(std::string &effectname, bool &boolValue, std::vector<std::string> &paramnames);

public:
    std::string domainName;
    std::vector<std::pair<std::string, int>> predicateList;
    std::vector<std::string> actionList;
    std::vector< std::vector<std::string>> constList;
    std::vector<std::pair<std::string, int>> actionCostList;
    std::vector<std::pair<std::string, std::string>> parameterList;
    std::vector<std::tuple<std::string, std::string, bool, std::vector<std::string>>> precondList;
    std::vector<std::tuple<std::string, std::string, bool, std::vector<std::string>>> effectList;
    std::vector<std::tuple<std::string, bool, std::vector<std::string>>> whenPredList;
    std::vector<std::tuple<std::string, bool, std::vector<std::string>>> whenEffectList;
    std::vector<std::tuple<std::string, std::vector<std::tuple<std::string, bool, std::vector<std::string>>>,
    std::vector<std::tuple<std::string, bool, std::vector<std::string>>>>> condiList;

    std::vector<std::vector<std::string>> objectList;
    std::vector<std::tuple<std::string, std::vector<std::string>>> initList;
    std::vector<std::tuple<std::string, std::vector<std::string>>> goalList;



    Parser(std::ifstream &inFile);
};

#endif
