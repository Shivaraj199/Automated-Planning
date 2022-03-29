#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include "parser.hpp"

void Parser::replaceAll(std::string &str, const std::string &from, const std::string &to)
{
    if (from.empty())
        return;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

void Parser::mapInit()
{
    keyword_map["domain"] = domain;
    keyword_map[":requirement"] = requirement;
    keyword_map[":predicates"] = predicates;
    keyword_map[":constants"] = constants;
    keyword_map[":action"] = action;
    keyword_map[":parameters"] = parameters;
    keyword_map[":precondition"] = precondition;
    keyword_map[":effect"] = effect;
    keyword_map[":objects"] = object;
    keyword_map[":init"] = init;
    keyword_map[":goal"] = goal;
}

void Parser::stack_pop(std::vector<std::string> &stack)
{
    if (stack.size() > 0)
        stack.pop_back();
    else
        throw std::runtime_error("Paranthesis not balanced");
}

void Parser::check_stack(std::vector<std::string> &stack)
{
    if (stack.size())
        throw std::runtime_error("Paranthesis not balanced");
}

void Parser::parsed_init(std::string &actionname, std::vector<std::string> &paramNames)
{

    actionname = tStack[0];
    tStack.erase(tStack.begin());
    for (auto token : tStack)
    {
        paramNames.push_back(token);
    };
    tStack.clear();
}

void Parser::parsed_goal(std::string &actionname, std::vector<std::string> &paramNames)
{
    actionname = tStack[0];
    tStack.erase(tStack.begin());
    for (auto token : tStack)
    {
        paramNames.push_back(token);
    };
    tStack.clear();
}

void Parser::parsed_domainname(std::string &domainname)
{
    domainname = tStack[0];
    tStack.erase(tStack.begin());
}

void Parser::parsed_predicate(std::string &predicatename, int &no)
{
    int count = 0;
    predicatename = tStack[0];
    tStack.erase(tStack.begin());
    for (auto token : tStack)
    {
        if (token[0] == '?')
            count++;
    };
    no = count;
    tStack.clear();
    return;
}

void Parser::parsed_actionname(std::string &actionname)
{
    actionname = tStack[0];
    tStack.clear();
}

void Parser::parsed_parameter(std::string &actionname, std::string &parameter)
{
    actionname = currentAction;
    parameter = tStack[0].substr(1);
    tStack.clear();
}

void Parser::parsed_precod(std::string &actionname, std::string &precondname, bool &boolValue, std::vector<std::string> &paramnames)
{
    if (tStack[0] == "not")
    {
        precondVarState = false;
        tStack.erase(tStack.begin());
    }
    actionname = currentAction;
    precondname = tStack[0];
    boolValue = !(precondState == precondVarState);
    tStack.erase(tStack.begin());
    for (auto token : tStack)
    {
        paramnames.push_back(token.substr(1));
    };
    precondVarState = true;
    tStack.clear();
}

void Parser::parsed_effect(std::string &actionname, std::string &effectname, bool &boolValue, std::vector<std::string> &paramnames)
{
    if (tStack[0] == "not")
    {
        effectVarState = false;
        tStack.erase(tStack.begin());
    }
    actionname = currentAction;
    effectname = tStack[0];
    boolValue = !(effectState == effectVarState);
    tStack.erase(tStack.begin());
    for (auto token : tStack)
    {
        if (token[0] == '?')
            paramnames.push_back(token.substr(1));
    };
    effectVarState = true;
    tStack.clear();
}

void Parser::parsed_when_precod(std::string &precondname, bool &boolValue, std::vector<std::string> &paramnames)
{
    if (tStack[0] == "not")
    {
        effectVarState = false;
        tStack.erase(tStack.begin());
    }
    actionname = currentAction;
    effectname = tStack[0];
    boolValue = !(effectState == effectVarState);
    tStack.erase(tStack.begin());
    for (auto token : tStack)
    {
        if (token[0] == '?')
            paramnames.push_back(token.substr(1));
    };
    effectVarState = true;
    tStack.clear();
}

void Parser::parsed_when_effect(std::string &effectname, bool &boolValue, std::vector<std::string> &paramnames)
{
    if (tStack[0] == "not")
    {
        effectVarState = false;
        tStack.erase(tStack.begin());
    }
    actionname = currentAction;
    effectname = tStack[0];
    boolValue = !(effectState == effectVarState);
    tStack.erase(tStack.begin());
    for (auto token : tStack)
    {
        if (token[0] == '?')
            paramnames.push_back(token.substr(1));
    };
    effectVarState = true;
    tStack.clear();
}

Parser::Parser(std::ifstream &inFile)
{
    try
    {
        mapInit();
        std::string fileTokenStream;
        while (std::getline(inFile, line))
        {
            std::string tokenstream = line.substr(0, line.find(";"));
            replaceAll(tokenstream, "(", " ( ");
            replaceAll(tokenstream, ")", " ) ");
            std::stringstream input(tokenstream);
            std::string token;
            while (input >> token)
            {
                fileTokenStream = fileTokenStream + " " + token;
                if (token == "(")
                {
                    fpStack.push_back(token);
                }
                if (token == ")")
                    stack_pop(fpStack);
            }
        };
        check_stack(fpStack);
        std::stringstream input(fileTokenStream);
        // parse the file token stream created above
        while (input >> token)
        {
            // std::cout << token << std::endl;
            switch (keyword_map[token])
            {

           /**
           **PARSING PROBLEM FILES
           **/

            /*parsing Domain file_name */
            case domain:
                input >> currentToken;
                tStack.push_back(currentToken);
                parsed_domainname(domainName);
                break;


            /*Parsing  Domain Predicates*/
            case predicates:
                input >> currentToken;
                fpStack.push_back("(");
                while (currentToken != ")")
                {
                    vpStack.push_back(currentToken);
                    while (currentToken != ")")
                    {
                        input >> currentToken;
                        (currentToken == ")") ? stack_pop(vpStack) : tStack.push_back(currentToken);
                    }
                    parsed_predicate(predicateName, no);
                    predicateList.push_back(std::make_pair(predicateName, no));
                    check_stack(vpStack);
                    input >> currentToken;
                };
                fpStack.pop_back();
                check_stack(fpStack);
                break;

            /*Parsing Domain Constants*/
            case constants:
                input >> currentToken;
                fpStack.push_back("(");
                while (currentToken != ")")
                {
                    std::vector<std::string> temp;
                    while (currentToken != "-")
                    {
                        temp.push_back(currentToken);
                        input >> currentToken;
                    }
                    if (temp.size() != 0){
                        constList.push_back(temp);
                    }
                    input >> currentToken;
                    input >> currentToken;
                }
                fpStack.pop_back();
                check_stack(fpStack);
                break;

            /*Parsing every action_name From Domain file*/
            case action:
                input >> currentToken;
                tStack.push_back(currentToken);
                parsed_actionname(currentAction);
                actionList.push_back(currentAction);
                break;


            /*Parsing the Parameters of domain File*/
            case parameters:
                input >> currentToken;
                fpStack.push_back("(");
                while (currentToken != ")")
                {
                    if (currentToken[0] == '?')
                    {

                        tStack.push_back(currentToken);
                        parsed_parameter(actionname, parameter);
                        parameterList.push_back(std::make_pair(actionname, parameter));
                    }

                    input >> currentToken;
                }
                stack_pop(fpStack);
                check_stack(fpStack);
                break;

            /*Parsing precondition of domain File*/
            case precondition:
                insideP = false;
                precondString = "";
                input >> currentToken;
                fpStack.push_back("(");
                while (fpStack.size())
                {
                    input >> currentToken;
                    if (currentToken == "(")
                    {
                        insideP = true;
                        fpStack.push_back("(");
                    }

                    if (currentToken == ")")
                        stack_pop(fpStack);
                    precondString = precondString + " " + currentToken;
                }
                precondString.pop_back(); // remove the last )
                if (!insideP)             //check if there is only one preconsition
                {
                    std::stringstream precondStream(precondString);
                    while (precondStream >> currentToken)
                    {
                        tStack.push_back(currentToken);
                    }
                    std::vector<std::string> paramnames;
                    parsed_precod(actionname, preconditionValue, boolValue, paramnames);
                    precondList.push_back(std::make_tuple(actionname, preconditionValue, boolValue, paramnames));
                    paramnames.clear();
                }
                else
                {
                    std::stringstream precondStream(precondString);
                    precondStream >> currentToken;
                    if (currentToken == "and")
                        precondState = true;
                    if (currentToken == "not")
                        precondState = false;
                    while (precondStream >> currentToken)
                    {
                        vpStack.push_back("(");
                        while (vpStack.size())
                        {
                            precondStream >> currentToken;
                            if (currentToken == "(")
                                vpStack.push_back("(");
                            else if (currentToken == ")")
                                vpStack.pop_back();
                            else
                                tStack.push_back(currentToken);
                        }
                        std::vector<std::string> paramnames;
                        parsed_precod(actionname, preconditionValue, boolValue, paramnames);
                        precondList.push_back(std::make_tuple(actionname, preconditionValue, boolValue, paramnames));
                        paramnames.clear();
                    }
                }
                check_stack(fpStack);
                break;


            /*Parsing Effect of domain File*/
            case effect:
                insideP = false;
                whenEffectList.clear();
                whenPredList.clear();
                effectString = "";
                input >> currentToken;
                fpStack.push_back("(");
                while (fpStack.size())
                {
                    input >> currentToken;
                    if (currentToken == "(")
                    {
                        insideP = true;
                        fpStack.push_back("(");
                    }

                    if (currentToken == ")")
                        stack_pop(fpStack);
                    effectString = effectString + " " + currentToken;
                }
                effectString.pop_back(); // remove the last )
                if (!insideP)            // check if there is only one effect
                {
                    std::stringstream effectStream(effectString);
                    while (effectStream >> currentToken)
                    {
                        tStack.push_back(currentToken);
                    }
                    parsed_precod(actionname, effectname, boolValue, paramnames);

                    //std::cout << "std:" << actionname << " " << effectname << " " << boolValue << std::endl;

                    effectList.push_back(std::make_tuple(actionname, effectname, boolValue, paramnames));
                    paramnames.clear();
                }
                else
                {
                    std::stringstream effectStream(effectString);
                    effectStream >> currentToken;
                    if (currentToken == "and")
                        effectState = true;
                    else if (currentToken == "not")
                        effectState = false;
                    else
                    {
                        std::streamsize pos = effectStream.tellp();
                        effectStream.str(currentToken + effectStream.str());
                        effectStream.seekp(pos + currentToken.length());
                    }

                    while (effectStream >> currentToken)
                    {
                        vpStack.push_back("(");
                        bool increase_found = false;
                        bool forall_found = false;
                        bool when_found = false;
                        int increaseParamCount = 0;

                        while (vpStack.size())
                        {
                            effectStream >> currentToken;
                            if (currentToken == "(")
                                vpStack.push_back("(");
                            else if (currentToken == ")")
                                vpStack.pop_back();
                            else if (currentToken == "increase")
                            {
                                increase_found = true;
                            }
                            else if (currentToken == "forall")
                            {
                                forall_found = true;
                                while (currentToken != "when")
                                {
                                    effectStream >> currentToken;
                                }
                                when_found = true;
                            }
                            else if (currentToken == "when")
                            {
                                when_found = true;
                            }

                            else
                            {
                                if (!increase_found)
                                {

                                    tStack.push_back(currentToken);
                                }
                                else
                                {
                                    increaseParamCount++;
                                    if (increaseParamCount == 2)
                                    {
                                        std::pair<std::string, int> temp_p;
                                        temp_p.first = actionname;
                                        std::stringstream numb(currentToken);
                                        int x;
                                        numb >> x;
                                        temp_p.second = x;
                                        //std::cout << "hello:" << temp_p.first << " " << temp_p.second << std::endl;
                                        actionCostList.push_back(temp_p);
                                    }
                                }
                            }

                            if (when_found)
                            {

                                std::vector<std::string> whenStack;
                                whenStack.push_back("(");
                                std::string whenEffectString;
                                while (whenStack.size())
                                {
                                    effectStream >> currentToken;
                                    if (currentToken == "(")
                                        whenStack.push_back("(");
                                    if (currentToken == ")")
                                        whenStack.pop_back();
                                    whenEffectString = whenEffectString + " " + currentToken;
                                }
                                whenEffectString.pop_back(); // remove the last )
                                std::stringstream whenEffectStream(whenEffectString);
                                std::vector<std::string> innerpStack;
                                std::vector<std::string> innerAndStack;
                                std::string innerString;
                                bool and_found = false;
                                int pred = 0;
                                while (whenEffectStream >> currentToken)
                                {
                                    innerpStack.push_back("(");
                                    innerString = "";

                                    while (innerpStack.size())
                                    {
                                        whenEffectStream >> currentToken;
                                        if (currentToken == "(")
                                            innerpStack.push_back("(");
                                        if (currentToken == ")")
                                            innerpStack.pop_back();
                                        if (currentToken == "and")
                                            and_found = true;
                                        innerString = innerString + " " + currentToken;
                                    }
                                    pred++;
                                    innerString.pop_back(); //remove the last )

                                    if (and_found)
                                    {

                                        std::stringstream innerStream(innerString);
                                        innerStream >> currentToken; //get the and keyword
                                        while (innerStream >> currentToken)
                                        {
                                            innerAndStack.push_back("(");
                                            while (innerAndStack.size())
                                            {
                                                innerStream >> currentToken;
                                                if (currentToken == "(")
                                                    innerAndStack.push_back("(");
                                                else if (currentToken == ")")
                                                    innerAndStack.pop_back();
                                                else
                                                    tStack.push_back(currentToken);
                                            }
                                            if (pred == 1)
                                            {
                                                parsed_when_precod(effectname, boolValue, paramnames);
                                                whenPredList.push_back(std::make_tuple(effectname, boolValue, paramnames));
                                            }
                                            else
                                            {
                                                parsed_when_effect(effectname, boolValue, paramnames);
                                                whenEffectList.push_back(std::make_tuple(effectname, boolValue, paramnames));
                                            }

                                            paramnames.clear();
                                        }
                                    }
                                    else
                                    {

                                        std::stringstream innerStream(innerString);
                                        tStack.clear();
                                        while (innerStream >> currentToken)
                                        {
                                            tStack.push_back(currentToken);
                                        }
                                        if (pred == 1)
                                        {
                                            parsed_when_precod(effectname, boolValue, paramnames);
                                            whenPredList.push_back(std::make_tuple(effectname, boolValue, paramnames));
                                        }
                                        else
                                        {
                                            parsed_when_effect(effectname, boolValue, paramnames);
                                            whenEffectList.push_back(std::make_tuple(effectname, boolValue, paramnames));
                                        }
                                        paramnames.clear();
                                    }
                                }
                                if (forall_found)
                                {
                                    effectStream >> currentToken; //remove the ) from the forall statement
                                }
                            }
                        }

                        if ((!increase_found) && (!forall_found) && (!when_found))
                        {

                            parsed_effect(actionname, effectname, boolValue, paramnames);
                            effectList.push_back(std::make_tuple(actionname, effectname, boolValue, paramnames));
                        }
                        paramnames.clear();
                        tStack.clear();
                    }
                    condiList.push_back(std::make_tuple(currentAction, whenPredList, whenEffectList));
                }
                check_stack(fpStack);
                break;





            /**
            **PARSING PROBLEM FILES
            **/
            case object:
                fpStack.push_back("(");
                input >> currentToken;
                while (currentToken != ")")
                {
                    std::vector<std::string> obj_temp;
                    while (currentToken != "-")
                    {
                        obj_temp.push_back(currentToken);
                        input >> currentToken;
                    }
                    if (obj_temp.size() != 0){
                        objectList.push_back(obj_temp);
                    }
                    input >> currentToken;
                    input >> currentToken;
                }
                fpStack.pop_back();
                check_stack(fpStack);
                break;

            case init:
                initString = "";
                fpStack.push_back("(");
                while (fpStack.size())
                {
                    input >> currentToken;
                    if (currentToken == "(")
                    {

                        fpStack.push_back("(");
                    }

                    if (currentToken == ")")
                        stack_pop(fpStack);
                    initString = initString + " " + currentToken;
                }
                initString.pop_back(); // remove the last )

                if (initString.length())
                {
                    std::stringstream initStream(initString);
                    while (initStream >> currentToken)
                    {
                        vpStack.push_back("(");
                        while (vpStack.size())
                        {
                            initStream >> currentToken;
                            if (currentToken == "(")
                                vpStack.push_back("(");
                            else if (currentToken == ")")
                                vpStack.pop_back();
                            else
                                tStack.push_back(currentToken);
                        }

                        parsed_init(actionname, paramnames);
                        initList.push_back(std::make_tuple(actionname, paramnames));
                        paramnames.clear();
                    }
                }
                check_stack(fpStack);
                break;
            case goal:
                insideAnd = false;
                goalString = "";

                fpStack.push_back("(");
                while (fpStack.size())
                {
                    input >> currentToken;
                    if (currentToken == "(")
                    {
                        fpStack.push_back("(");
                    }
                    if (currentToken == "and")
                        insideAnd = true;
                    if (currentToken == ")")
                        stack_pop(fpStack);
                    goalString = goalString + " " + currentToken;
                }
                goalString.pop_back(); // remove the last )

                if (insideAnd)
                {
                    goalString.pop_back(); //space
                    goalString.pop_back(); //last )
                    std::stringstream goalStream(goalString);
                    goalStream >> currentToken;
                    goalStream >> currentToken;
                    while (goalStream >> currentToken)
                    {
                        vpStack.push_back("(");
                        while (vpStack.size())
                        {
                            goalStream >> currentToken;
                            if (currentToken == "(")
                                vpStack.push_back("(");
                            else if (currentToken == ")")
                                vpStack.pop_back();
                            else
                                tStack.push_back(currentToken);
                        }

                        parsed_goal(actionname, paramnames);
                        goalList.push_back(std::make_tuple(actionname, paramnames));
                        paramnames.clear();
                    }
                }

                else
                {
                    std::stringstream goalStream(goalString);
                    while (goalStream >> currentToken)
                    {
                        if (currentToken != "(" && currentToken != ")")
                            tStack.push_back(currentToken);
                    }

                    parsed_goal(actionname, paramnames);
                    goalList.push_back(std::make_tuple(actionname, paramnames));
                    paramnames.clear();
                }

                check_stack(fpStack);
                break;
            default:
                break;
            }
        }
    }
    catch (const std::exception &ex)
    {
        std::cerr << "*** Error: " << ex.what() << "\n";
    }
}
