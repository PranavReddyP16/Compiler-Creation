#include "Lexer.h"
using namespace std;
#define acc 10000

//function declarations
void initialiseParseTable();
void initialiseRules();
vector<pair<string, int>> lexer();

//this map handles Action
//if shift operation then it is a positive number
//if reduce operation then it is a negative number
map<pair<int, string>, int> action;
//this map handles Goto
map<pair<int, string>, int> gotoState;
//this map handles the rules
//(RHS -> vector of LHS parts), (rule number)
map<int, pair<string, vector<string>>> rules;

int main(int argc, char *argv[])
{
    //cout << add(2, 3);
    initialiseParseTable();
    initialiseRules();
    //data structures required
    //vector<string> input;
    stack<pair<string, int>> s; //stack can have string as well as integers for states so if one of them is present as an element the other will be empty string or INT_MAX

    vector<std::pair<std::string, int>> inputFromLexer = lexerCall(argc, argv);
    //vector<pair<string, int>> inputFromLexer = lexer();
    vector<string> input;

    int inputCount = inputFromLexer.size();

    for (int i = 0; i < inputCount; ++i)
    {
        input.push_back(inputFromLexer[i].first);
        // cout << input[i] << endl;
    }

    //mandatory conditions for parsing
    ++inputCount;
    s.push({"$", INT_MAX});
    s.push({"", 0});

    input.push_back("$");
    //cout << inputCount << endl;
    int inputMonitor = 0;

    while (inputMonitor < inputCount)
    {
        string currentInput = input[inputMonitor];
        pair<string, int> stackTop = s.top();

        //if the current transition is not present in the parse table
        if (action.find({stackTop.second, currentInput}) == action.end())
        {
            cout << "The input is incorrect!!!!" << endl;
            break;
        }
        //if present
        else
        {
            int actionToTake = action[{stackTop.second, currentInput}];

            if (actionToTake == acc) //accepted state
            {
                cout << "The input is correct!!!!" << endl;
                break;
            }
            else if (actionToTake > 0) //shift operation
            {
                s.push({currentInput, INT_MAX});
                s.push({"", actionToTake});
                ++inputMonitor;
            }
            else //reduce operation
            {
                actionToTake = abs(actionToTake);
                vector<string> currentRule = rules[actionToTake].second;
                stack<string> stackPoppedElements;
                while (s.size() > 0 && stackPoppedElements.size() < currentRule.size())
                {
                    //removing the string and the state number
                    s.pop();
                    stackPoppedElements.push(s.top().first);
                    s.pop();
                }
                if (stackPoppedElements.size() != currentRule.size())
                {
                    cout << "The input is incorrect!!!!" << endl;
                    break;
                }
                else
                {
                    int invalid = 0;
                    for (auto it : currentRule)
                    {
                        if (it != stackPoppedElements.top())
                        {
                            invalid = 1;
                        }
                        stackPoppedElements.pop();
                    }
                    if (invalid == 1)
                    {
                        cout << "The input is incorrect!!!!" << endl;
                        break;
                    }
                    else
                    {
                        string gotoColumn = rules[actionToTake].first;
                        int gotoStateVal = s.top().second;
                        s.push({gotoColumn, INT_MAX});
                        s.push({"", gotoState[{gotoStateVal, gotoColumn}]});
                    }
                }
            }
        }
        //stack printer
        // stack<pair<string, int>> s1 = s;
        // while (!s1.empty())
        // {
        //     if (s1.top().first != "")
        //     {
        //         cout << s1.top().first;
        //     }
        //     else
        //     {
        //         cout << s1.top().second;
        //     }
        //     s1.pop();
        // }
        // cout << endl;
    }
}

//initialising the rules numbered from 0
void initialiseRules()
{
    rules[0] = {"PROGRAM", {"main()", "{", "STATEMENTS", "}"}};
    rules[1] = {"STATEMENTS", {"STATEMENT", "STATEMENTS"}};
    rules[2] = {"STATEMENTS", {}};
    rules[3] = {"STATEMENT", {"CONDITIONAL"}};
    rules[4] = {"STATEMENT", {"LOOP"}};
    rules[5] = {"STATEMENT", {"datatype", "id", ";"}};
    rules[6] = {"STATEMENT", {"ASSIGNMENT", ";"}};
    rules[7] = {"CONDITIONAL", {"if", "(", "CONDITIONS", ")", "{", "STATEMENTS", "}"}};
    rules[8] = {"CONDITIONAL", {"if", "(", "CONDITIONS", ")", "{", "STATEMENTS", "}", "else", "{", "STATEMENTS", "}"}};
    rules[9] = {"CONDITIONS", {"CONDITION", "logical_operator", "CONDITION"}};
    rules[10] = {"CONDITIONS", {"CONDITION"}};
    rules[11] = {"CONDITION", {"id", "comparator", "TERM"}};
    rules[12] = {"LOOP", {"for", "(", "ASSIGNMENT", ";", "CONDITIONS", ";", "ASSIGNMENT", ")", "{", "STATEMENTS", "}"}};
    rules[13] = {"ASSIGNMENT", {"id", "=", "EXPRESSION"}};
    rules[14] = {"ASSIGNMENT", {"datatype", "id", "=", "EXPRESSION"}};
    rules[15] = {"EXPRESSION", {"(", "EXPRESSION", "arithmetic_operator", "EXPRESSION", ")"}};
    rules[16] = {"EXPRESSION", {"TERM"}};
    rules[17] = {"TERM", {"id"}};
    rules[18] = {"TERM", {"literal"}};
}

//generating the parse table
void initialiseParseTable()
{
    action[{0, "main()"}] = 1;

    action[{1, "{"}] = 2;
    action[{32, "{"}] = 38;
    action[{49, "{"}] = 51;
    action[{50, "{"}] = 52;

    action[{2, "}"}] = -2;
    action[{3, "}"}] = 12;
    action[{4, "}"}] = -2;
    action[{5, "}"}] = -3;
    action[{6, "}"}] = -4;
    action[{13, "}"}] = -1;
    action[{15, "}"}] = -6;
    action[{19, "}"}] = -5;
    action[{38, "}"}] = -2;
    action[{43, "}"}] = 46;
    action[{46, "}"}] = -7;
    action[{49, "}"}] = 51;
    action[{51, "}"}] = -2;
    action[{52, "}"}] = -2;
    action[{53, "}"}] = 55;
    action[{54, "}"}] = 56;
    action[{55, "}"}] = -8;
    action[{56, "}"}] = -12;

    action[{2, "datatype"}] = 7;
    action[{4, "datatype"}] = 7;
    action[{5, "datatype"}] = -3;
    action[{6, "datatype"}] = -4;
    action[{15, "datatype"}] = -6;
    action[{17, "datatype"}] = 25;
    action[{19, "datatype"}] = -5;
    action[{38, "datatype"}] = 7;
    action[{44, "datatype"}] = 25;
    action[{46, "datatype"}] = -7;
    action[{51, "datatype"}] = 7;
    action[{52, "datatype"}] = 7;
    action[{55, "datatype"}] = -8;
    action[{56, "datatype"}] = -12;

    action[{2, "id"}] = 11;
    action[{4, "id"}] = 11;
    action[{5, "id"}] = -3;
    action[{6, "id"}] = -4;
    action[{7, "id"}] = 14;
    action[{15, "id"}] = -6;
    action[{16, "id"}] = 23;
    action[{17, "id"}] = 11;
    action[{18, "id"}] = 29;
    action[{19, "id"}] = -5;
    action[{20, "id"}] = 29;
    action[{25, "id"}] = 36;
    action[{27, "id"}] = 29;
    action[{33, "id"}] = 23;
    action[{34, "id"}] = 29;
    action[{35, "id"}] = 23;
    action[{38, "id"}] = 11;
    action[{42, "id"}] = 29;
    action[{44, "id"}] = 11;
    action[{46, "id"}] = -7;
    action[{51, "id"}] = 11;
    action[{52, "id"}] = 11;
    action[{55, "id"}] = -8;
    action[{56, "id"}] = -12;

    action[{8, ";"}] = 15;
    action[{14, ";"}] = 19;
    action[{22, ";"}] = -10;
    action[{24, ";"}] = 35;
    action[{26, ";"}] = -13;
    action[{28, ";"}] = -16;
    action[{29, ";"}] = -17;
    action[{30, ";"}] = -18;
    action[{31, ";"}] = -14;
    action[{39, ";"}] = -9;
    action[{40, ";"}] = -11;
    action[{41, ";"}] = 44;
    action[{48, ";"}] = -15;

    action[{2, "if"}] = 9;
    action[{4, "if"}] = 9;
    action[{5, "if"}] = -3;
    action[{6, "if"}] = -4;
    action[{15, "if"}] = -6;
    action[{19, "if"}] = -5;
    action[{38, "if"}] = 9;
    action[{46, "if"}] = -7;
    action[{51, "if"}] = 9;
    action[{52, "if"}] = 9;
    action[{55, "if"}] = -8;
    action[{56, "if"}] = -12;

    action[{9, "("}] = 16;
    action[{10, "("}] = 17;
    action[{18, "("}] = 27;
    action[{20, "("}] = 27;
    action[{27, "("}] = 27;
    action[{42, "("}] = 27;

    action[{21, ")"}] = 32;
    action[{22, ")"}] = -10;
    action[{26, ")"}] = -13;
    action[{28, ")"}] = -16;
    action[{29, ")"}] = -17;
    action[{30, ")"}] = -18;
    action[{31, ")"}] = -14;
    action[{39, ")"}] = -9;
    action[{40, ")"}] = -11;
    action[{45, ")"}] = 48;
    action[{47, ")"}] = 50;
    action[{48, ")"}] = -15;

    action[{46, "else"}] = 49;

    action[{22, "logical_operator"}] = 33;
    action[{29, "logical_operator"}] = -17;
    action[{30, "logical_operator"}] = -18;
    action[{40, "logical_operator"}] = -11;

    action[{23, "comparator"}] = 34;

    action[{2, "for"}] = 10;
    action[{4, "for"}] = 10;
    action[{5, "for"}] = -3;
    action[{6, "for"}] = -4;
    action[{15, "for"}] = -6;
    action[{19, "for"}] = -5;
    action[{38, "for"}] = 10;
    action[{46, "for"}] = -7;
    action[{51, "for"}] = 10;
    action[{52, "for"}] = 10;
    action[{55, "for"}] = -8;
    action[{56, "for"}] = -12;

    action[{11, "="}] = 18;
    action[{14, "="}] = 20;
    action[{36, "="}] = 20;

    action[{28, "arithmetic_operator"}] = -16;
    action[{29, "arithmetic_operator"}] = -17;
    action[{30, "arithmetic_operator"}] = -18;
    action[{37, "arithmetic_operator"}] = 42;
    action[{48, "arithmetic_operator"}] = -15;

    action[{18, "literal"}] = 30;
    action[{20, "literal"}] = 30;
    action[{27, "literal"}] = 30;
    action[{34, "literal"}] = 30;
    action[{42, "literal"}] = 30;

    action[{12, "$"}] = acc;

    gotoState[{2, "STATEMENTS"}] = 3;
    gotoState[{4, "STATEMENTS"}] = 13;
    gotoState[{38, "STATEMENTS"}] = 43;
    gotoState[{51, "STATEMENTS"}] = 53;
    gotoState[{52, "STATEMENTS"}] = 54;

    gotoState[{2, "STATEMENT"}] = 4;
    gotoState[{4, "STATEMENT"}] = 4;
    gotoState[{38, "STATEMENT"}] = 4;
    gotoState[{52, "STATEMENT"}] = 4;
    gotoState[{55, "STATEMENT"}] = 4;

    gotoState[{2, "CONDITIONAL"}] = 5;
    gotoState[{4, "CONDITIONAL"}] = 5;
    gotoState[{38, "CONDITIONAL"}] = 5;
    gotoState[{51, "CONDITIONAL"}] = 5;
    gotoState[{52, "CONDITIONAL"}] = 5;

    gotoState[{16, "CONDITIONS"}] = 21;
    gotoState[{35, "CONDITIONS"}] = 41;

    gotoState[{16, "CONDITION"}] = 22;
    gotoState[{33, "CONDITION"}] = 39;
    gotoState[{35, "CONDITION"}] = 22;

    gotoState[{2, "LOOP"}] = 6;
    gotoState[{4, "LOOP"}] = 6;
    gotoState[{38, "LOOP"}] = 6;
    gotoState[{51, "LOOP"}] = 6;
    gotoState[{52, "LOOP"}] = 6;

    gotoState[{2, "ASSIGNMENT"}] = 8;
    gotoState[{4, "ASSIGNMENT"}] = 8;
    gotoState[{17, "ASSIGNMENT"}] = 24;
    gotoState[{38, "ASSIGNMENT"}] = 8;
    gotoState[{44, "ASSIGNMENT"}] = 47;
    gotoState[{51, "ASSIGNMENT"}] = 8;
    gotoState[{52, "ASSIGNMENT"}] = 8;

    gotoState[{18, "EXPRESSION"}] = 26;
    gotoState[{20, "EXPRESSION"}] = 31;
    gotoState[{27, "EXPRESSION"}] = 37;
    gotoState[{42, "EXPRESSION"}] = 45;

    gotoState[{18, "TERM"}] = 28;
    gotoState[{20, "TERM"}] = 28;
    gotoState[{27, "TERM"}] = 28;
    gotoState[{34, "TERM"}] = 40;
    gotoState[{42, "TERM"}] = 28;
}
