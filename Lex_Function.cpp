//Team- Temp
//Pranay Tarigopula - 2018A7PS0237H
//Dhruv Adlakha - 2018A7PS0303H
//Pranav Reddy Pesaladinne - 2018A7PS0238H
//Donkada Vishal Dheeraj - 2018A7PS0239H

#include "Lexer.h"
using namespace std;

vector<pair<string, int>> tokens;
vector<pair<string, int>> lexemes;
//predefined
set<char> delimeters;
set<string> keywords;
set<string> double_operators;
set<char> whitespaces;
set<string> arithmetic_operators;
set<string> logical_operators;
set<string> comparators;
set<string> assignment_operators;
set<string> unary_operators;
set<string> special_chars;
set<string> datatypes;

int line_count = 1;

void handle_delimeter(string &s, int &pos)
{
    //handle if last lexeme
    if (pos == (int)s.size() - 1)
    {
        if (s[pos] == '\n')
        {
            return;
        }

        lexemes.push_back({string(1, s[pos]), line_count});
        return;
    }

    //string literal checking
    else if (s[pos] == '"')
    {
        string temp = "\"";
        pos++;

        while (pos < (int)s.size() && s[pos] != '"')
        {
            if (s[pos] == '\n')
            {
                line_count++;
            }

            //if(s[pos]=='\\') {
            //    temp.push_back(s[pos]);
            //    temp.push_back(s[pos+1]);
            //    pos++;
            //}

            if (whitespaces.find(s[pos]) != whitespaces.end())
            {
                temp.push_back(' ');
            }
            else
            {
                temp.push_back(s[pos]);
            }
            pos++;
        }

        if (pos < (int)s.size())
        {
            temp.push_back('\"');
        }

        lexemes.push_back({temp, line_count});
        return;
    }

    //comment checking
    else if (s[pos] == '#')
    {
        pos++;
        while (pos < (int)s.size() && s[pos] != '#')
        {
            if (s[pos] == '\n')
                line_count++;
            pos++;
        }

        return;
    }

    //double symbol checking
    string temp = s.substr(pos, 2);
    if (double_operators.find(temp) != double_operators.end())
    {
        lexemes.push_back({temp, line_count});
        pos++;
    }

    //making sure it's not whitespace
    else
    {
        if (whitespaces.find(s[pos]) == whitespaces.end())
            lexemes.push_back({string(1, s[pos]), line_count});
    }
}

bool check_numeric(string token)
{
    int n = token.size();

    for (int i = 0; i < n; i++)
    {
        if (!isdigit(token[i]))
        {
            return false;
        }
    }
    if (token[0] == '0' && n > 1)
        return false;

    return true;
}

bool check_identifier(string token)
{
    int n = token.size();

    if (keywords.find(token) != keywords.end())
    {
        return false;
    }

    if (!(isalpha(token[0])) && token[0] != '_')
    {
        return false;
    }

    for (int i = 1; i < n; i++)
    {
        if (!isdigit(token[i]) && !isalpha(token[i]) && token[i] != '_')
        {
            return false;
        }
    }

    return true;
}

bool check_float_literal(string token)
{
    int decimal_count = 0;
    int n = token.size();
    if (n < 2)
        return false;

    for (int i = 0; i < n; i++)
    {
        if (token[i] == '.')
        {
            decimal_count++;
        }
        else if (!isdigit(token[i]))
        {
            return false;
        }
    }

    if (decimal_count != 1)
        return false;
    else if (token[0] == '0' && token[1] != '.')
        return false;
    else if (token[n - 1] == '.')
        return false;
    else
        return true;
}

bool check_string_literal(string token)
{
    if (token[0] == '\"' && token.back() == '\"')
    {
        return true;
    }

    return false;
}

string which_special_char(string s)
{
    if (s == ";")
        return "semicolon";
    else if (s == "(")
        return "left_parenthesis";
    else if (s == ")")
        return "right_paranthesis";
    else if (s == "{")
        return "left_brace";
    else if (s == "}")
        return "right_brace";
    else if (s == "[")
        return "left_bracket";
    else if (s == "]")
        return "right_bracket";
    else if (s == ",")
        return "comma";
    else
        return "None";
}

bool is_valid_file(char *fname)
{
    int i = 0;
    while (fname[i] != '.' && fname[i] != '\0')
    {
        i++;
    }
    if (fname[i] == '\0')
    {
        return false;
    }

    if (fname[i + 1] == 't' && fname[i + 2] == 'e' && fname[i + 3] == 'm' && fname[i + 4] == 'p')
    {
        return true;
    }

    return false;
}

vector<pair<string, int>> lexerCall(int argc, char *argv[])
{

    //checks for a .temp extension
    if (!is_valid_file(argv[1]))
    {
        cerr << "Invalid source file" << endl;
        return {};
    }

    //reading input file as a string and removing EOF character
    ifstream ifs(argv[1]);
    string s((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
    //This line is to remove the EOF char
    s.pop_back();

    delimeters = {'<', '>', '=', '&', '|', '!', '+', '-',
                  '*', '/', '%', ' ', ',', '[',
                  ']', '{', '}', '(', ')', ';', '"', '#', '\n',
                  '\t', '\r'};

    double_operators = {"<<", ">>", "==", "&&", "||", "++",
                        "+=", "--", "-=", "**", "//", "!=",
                        "*=", "/="};

    whitespaces = {' ', '\n', '\t', '\r'};

    keywords = {"int", "char", "float", "bool", "string", "if",
                "else", "for", "main", "true", "false",
                "function", "return", "print"};

    datatypes = {"int", "char", "float", "bool", "string"};

    arithmetic_operators = {"+", "*", "/", "-", "%", "<<", ">>", "**", "//"};

    logical_operators = {"&&", "||"};

    comparators = {"<", ">", "==", ">=", "<=", "!="};

    assignment_operators = {"=", "+=", "-=", "*=", "/="};

    unary_operators = {"++", "--", "!", "+", "-", "~"};

    special_chars = {"(", ")", "{", "}", "[", "]", ";", ","};

    //breaking into lexemes using delimiters and the handle_delimeter() function
    int n = s.size();
    string temp = "";
    for (int i = 0; i < n; i++)
    {
        if (delimeters.find(s[i]) != delimeters.end())
        {
            if (!temp.empty())
            {
                lexemes.push_back({temp, line_count});
            }

            //increasing line count whenever we encounter \n
            if (s[i] == '\n')
            {
                line_count++;
            }

            temp.clear();
            handle_delimeter(s, i);
        }

        else
        {
            temp.push_back(s[i]);
        }
    }
    if (!temp.empty())
    {
        lexemes.push_back({temp, line_count});
    }

    /***************************************finished obtaining lexemes*************************************/

    //printing all lexemes with corresponding tokens and line numbers
    for (int i = 0; i < (int)lexemes.size(); i++)
    {
        if (arithmetic_operators.find(lexemes[i].first) != arithmetic_operators.end())
        {
            //checking for unary based on token of previous lexeme
            if (i > 0 &&
                ((!check_identifier(lexemes[i - 1].first) && !check_numeric(lexemes[i - 1].first) &&
                  !check_float_literal(lexemes[i - 1].first) && !(lexemes[i - 1].first == ")")) ||
                 double_operators.find(lexemes[i - 1].first) != double_operators.end()) &&
                (lexemes[i].first == "-" || lexemes[i].first == "+" || lexemes[i].first == "!"))
            {

                tokens.push_back({"unary_operator", lexemes[i].second});
            }
            else if (i == 0 && (lexemes[i].first == "+" || lexemes[i].first == "-" || lexemes[i].first == "!"))
            {
                tokens.push_back({"unary_operator", lexemes[i].second});
            }
            else
            {
                tokens.push_back({"arithmetic_operator", lexemes[i].second});
            }
        }
        else if (logical_operators.find(lexemes[i].first) != logical_operators.end())
        {
            tokens.push_back({"logical_operator", lexemes[i].second});
        }
        else if (unary_operators.find(lexemes[i].first) != unary_operators.end())
        {
            tokens.push_back({"unary_operator", lexemes[i].second});
        }
        else if (datatypes.find(lexemes[i].first) != datatypes.end())
        {
            tokens.push_back({"datatype", lexemes[i].second});
        }
        else if (keywords.find(lexemes[i].first) != keywords.end())
        {
            if (lexemes[i].first == "main")
            { //this will convert main to main()
                tokens.push_back({"main()", lexemes[i].second});
                i += 2;
            }
            else if (lexemes[i].first == "if")
                tokens.push_back({"if", lexemes[i].second});
            else if (lexemes[i].first == "else")
                tokens.push_back({"else", lexemes[i].second});
            else if (lexemes[i].first == "for")
                tokens.push_back({"for", lexemes[i].second});
        }
        else if (special_chars.find(lexemes[i].first) != special_chars.end())
        {
            tokens.push_back({lexemes[i].first, lexemes[i].second});
        }
        else if (comparators.find(lexemes[i].first) != comparators.end())
        {
            tokens.push_back({"comparator", lexemes[i].second});
        }
        else if (assignment_operators.find(lexemes[i].first) != assignment_operators.end())
        {
            tokens.push_back({"=", lexemes[i].second});
        }
        else if (check_numeric(lexemes[i].first))
        {
            tokens.push_back({"literal", lexemes[i].second});
        }
        else if (check_identifier(lexemes[i].first))
        {
            tokens.push_back({"id", lexemes[i].second});
        }
        else if (check_string_literal(lexemes[i].first))
        {
            tokens.push_back({"literal", lexemes[i].second});
        }
        else if (check_float_literal(lexemes[i].first))
        {
            tokens.push_back({"literal", lexemes[i].second});
        }
        else
        {
            tokens.push_back({"lexical_error", lexemes[i].second});
        }
    }

    // cout << "number of lines is " << line_count << endl;

    return tokens;
}
