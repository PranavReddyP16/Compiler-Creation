#include <bits/stdc++.h>
using namespace std;
#define acc 10000

//function declarations
void initialiseParseTable();
void initialiseRules();

//this map handles Action
//if shift operation then it is a positive number
//if reduce operation then it is a negative number
map<pair<int, string>, int> action;
//this map handles Goto
map<pair<int, string>, int> gotoState;
//this map handles the rules
//(RHS -> vector of LHS parts), (rule number)
map<int, pair<string, vector<string>>> rules;

int main()
{
    initialiseParseTable();
    initialiseRules();

    //data structures required
    vector<string> input;
    stack<pair<string, int>> s; //stack can have string as well as integers for states so if one of them is present as an element the other will be empty string or INT_MAX

    //getting user input tokens
    cout << "Enter the number of input elements: ";
    int inputCount;
    cin >> inputCount;

    cout << "Enter the input elements, one on each line" << endl;
    for (int i = 0; i < inputCount; ++i)
    {
        string temp;
        cin >> temp;
        input.push_back(temp);
    }

    //mandatory conditions for parsing
    ++inputCount;
    s.push({"$", INT_MAX});
    s.push({"", 0});

    input.push_back("$");

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
    rules[7] = {"CONDITIONAL", {"if", "(", "{", "CONDITIONS", "}", ")", "{", "STATEMENTS", "}", "else", "{", "STATEMENTS", "}"}};
    rules[8] = {"CONDITIONS", {"CONDITION", "logical_operator", "CONDITION"}};
    rules[9] = {"CONDITIONS", {"CONDITION"}};
    rules[10] = {"CONDITION", {"id", "comparator", "TERM"}};
    rules[11] = {"LOOP", {"for", "(", "ASSIGNMENT", ";", "CONDITIONS", ";", "ASSIGNMENT", ")", "{", "STATEMENTS", "}"}};
    rules[12] = {"ASSIGNMENT", {"id", "=", "EXPRESSION"}};
    rules[13] = {"ASSIGNMENT", {"datatype", "id", "=", "EXPRESSION"}};
    rules[14] = {"EXPRESSION", {"(", "EXPRESSION", "arithmetic_operator", "EXPRESSION", ")"}};
    rules[15] = {"EXPRESSION", {"TERM"}};
    rules[16] = {"TERM", {"id"}};
    rules[17] = {"TERM", {"literal"}};
}

//generating the parse table
void initialiseParseTable()
{
    action[{0, "main()"}] = 1;

    action[{1, "{"}] = 2;
    action[{16, "{"}] = 21;
    action[{41, "{"}] = 46;
    action[{50, "{"}] = 52;
    action[{53, "{"}] = 55;

    action[{2, "}"}] = -2;
    action[{3, "}"}] = 12;
    action[{4, "}"}] = -2;
    action[{5, "}"}] = -3;
    action[{6, "}"}] = -4;
    action[{13, "}"}] = -1;
    action[{15, "}"}] = -6;
    action[{19, "}"}] = -5;
    action[{27, "}"}] = -17;
    action[{28, "}"}] = -18;
    action[{30, "}"}] = 36;
    action[{31, "}"}] = -10;
    action[{42, "}"}] = -9;
    action[{43, "}"}] = -11;
    action[{46, "}"}] = -2;
    action[{49, "}"}] = 51;
    action[{51, "}"}] = -7;
    action[{52, "}"}] = -2;
    action[{54, "}"}] = 56;
    action[{55, "}"}] = -2;
    action[{56, "}"}] = -12;
    action[{57, "}"}] = 58;
    action[{58, "}"}] = -8;

    action[{2, "datatype"}] = 7;
    action[{4, "datatype"}] = 7;
    action[{5, "datatype"}] = -3;
    action[{6, "datatype"}] = -4;
    action[{13, "datatype"}] = -1;
    action[{15, "datatype"}] = -6;
    action[{17, "datatype"}] = 23;
    action[{19, "datatype"}] = -5;
    action[{44, "datatype"}] = 23;
    action[{46, "datatype"}] = 7;
    action[{51, "datatype"}] = -7;
    action[{52, "datatype"}] = 7;
    action[{55, "datatype"}] = 7;
    action[{56, "datatype"}] = -12;
    action[{58, "datatype"}] = -8;

    action[{2, "id"}] = 11;
    action[{4, "id"}] = 11;
    action[{5, "id"}] = -3;
    action[{6, "id"}] = -4;
    action[{7, "id"}] = 14;
    action[{15, "id"}] = -6;
    action[{17, "id"}] = 11;
    action[{18, "id"}] = 27;
    action[{19, "id"}] = -5;
    action[{20, "id"}] = 27;
    action[{21, "id"}] = 32;
    action[{23, "id"}] = 34;
    action[{25, "id"}] = 27;
    action[{33, "id"}] = 32;
    action[{37, "id"}] = 32;
    action[{38, "id"}] = 27;
    action[{40, "id"}] = 27;
    action[{44, "id"}] = 11;
    action[{46, "id"}] = 11;
    action[{51, "id"}] = -7;
    action[{52, "id"}] = 11;
    action[{55, "id"}] = 11;
    action[{56, "id"}] = -12;
    action[{58, "id"}] = -8;

    action[{8, ";"}] = 15;
    action[{14, ";"}] = 19;
    action[{22, ";"}] = 33;
    action[{24, ";"}] = -13;
    action[{26, ";"}] = -16;
    action[{27, ";"}] = -17;
    action[{28, ";"}] = -18;
    action[{29, ";"}] = -14;
    action[{31, ";"}] = -10;
    action[{39, ";"}] = 44;
    action[{42, ";"}] = -9;
    action[{43, ";"}] = -11;
    action[{48, ";"}] = -15;

    action[{2, "if"}] = 9;
    action[{4, "if"}] = 9;
    action[{5, "if"}] = -3;
    action[{6, "if"}] = -4;
    action[{15, "if"}] = -6;
    action[{19, "if"}] = -5;
    action[{46, "if"}] = 9;
    action[{51, "if"}] = -7;
    action[{52, "if"}] = 9;
    action[{55, "if"}] = 9;
    action[{56, "if"}] = -12;
    action[{58, "if"}] = -8;

    action[{9, "("}] = 16;
    action[{10, "("}] = 17;
    action[{18, "("}] = 25;
    action[{20, "("}] = 25;
    action[{25, "("}] = 25;
    action[{40, "("}] = 25;

    action[{24, ")"}] = -13;
    action[{26, ")"}] = -16;
    action[{27, ")"}] = -17;
    action[{28, ")"}] = -18;
    action[{29, ")"}] = -14;
    action[{36, ")"}] = 41;
    action[{45, ")"}] = 48;
    action[{47, ")"}] = 50;
    action[{48, ")"}] = -15;

    action[{51, "else"}] = 53;

    action[{27, "logical_operator"}] = -17;
    action[{28, "logical_operator"}] = -18;
    action[{31, "logical_operator"}] = 37;
    action[{43, "logical_operator"}] = -11;

    action[{32, "comparator"}] = 38;

    action[{2, "for"}] = 10;
    action[{4, "for"}] = 10;
    action[{5, "for"}] = -3;
    action[{6, "for"}] = -4;
    action[{15, "for"}] = -6;
    action[{19, "for"}] = -5;
    action[{46, "for"}] = 10;
    action[{51, "for"}] = -7;
    action[{52, "for"}] = 10;
    action[{55, "for"}] = 10;
    action[{56, "for"}] = -12;
    action[{58, "for"}] = -8;

    action[{11, "="}] = 18;
    action[{14, "="}] = 20;
    action[{34, "="}] = 20;

    action[{26, "arithmetic_operator"}] = -16;
    action[{27, "arithmetic_operator"}] = -17;
    action[{28, "arithmetic_operator"}] = -18;
    action[{35, "arithmetic_operator"}] = 40;
    action[{48, "arithmetic_operator"}] = -15;

    action[{18, "literal"}] = 28;
    action[{20, "literal"}] = 28;
    action[{25, "literal"}] = 28;
    action[{38, "literal"}] = 28;
    action[{40, "literal"}] = 28;

    action[{12, "$"}] = acc;

    gotoState[{2, "STATEMENTS"}] = 3;
    gotoState[{4, "STATEMENTS"}] = 13;
    gotoState[{46, "STATEMENTS"}] = 49;
    gotoState[{52, "STATEMENTS"}] = 54;
    gotoState[{55, "STATEMENTS"}] = 57;

    gotoState[{2, "STATEMENT"}] = 4;
    gotoState[{4, "STATEMENT"}] = 4;
    gotoState[{46, "STATEMENT"}] = 4;
    gotoState[{52, "STATEMENT"}] = 4;
    gotoState[{55, "STATEMENT"}] = 4;

    gotoState[{2, "CONDITIONAL"}] = 5;
    gotoState[{4, "CONDITIONAL"}] = 5;
    gotoState[{46, "CONDITIONAL"}] = 5;
    gotoState[{52, "CONDITIONAL"}] = 5;
    gotoState[{55, "CONDITIONAL"}] = 5;

    gotoState[{21, "CONDITIONS"}] = 30;
    gotoState[{33, "CONDITIONS"}] = 39;

    gotoState[{21, "CONDITION"}] = 31;
    gotoState[{33, "CONDITION"}] = 31;
    gotoState[{37, "CONDITION"}] = 42;

    gotoState[{2, "LOOP"}] = 6;
    gotoState[{4, "LOOP"}] = 6;
    gotoState[{46, "LOOP"}] = 6;
    gotoState[{52, "LOOP"}] = 6;
    gotoState[{55, "LOOP"}] = 6;

    gotoState[{2, "ASSIGNMENT"}] = 8;
    gotoState[{4, "ASSIGNMENT"}] = 8;
    gotoState[{17, "ASSIGNMENT"}] = 22;
    gotoState[{44, "ASSIGNMENT"}] = 47;
    gotoState[{46, "ASSIGNMENT"}] = 8;
    gotoState[{52, "ASSIGNMENT"}] = 8;
    gotoState[{55, "ASSIGNMENT"}] = 8;

    gotoState[{18, "EXPRESSION"}] = 24;
    gotoState[{20, "EXPRESSION"}] = 29;
    gotoState[{25, "EXPRESSION"}] = 35;
    gotoState[{40, "EXPRESSION"}] = 45;

    gotoState[{18, "TERM"}] = 26;
    gotoState[{20, "TERM"}] = 26;
    gotoState[{25, "TERM"}] = 26;
    gotoState[{38, "TERM"}] = 43;
    gotoState[{40, "TERM"}] = 26;
}
