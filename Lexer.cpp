#include<bits/stdc++.h>
#include <string>
using namespace std;

vector<string> lexemes;
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

void handle_delimeter(string &s, int &pos) {
    if(pos==(int)s.size()-1) {
        lexemes.push_back(string(1, s[pos]));
        return;
    }

    else if(s[pos]=='"') {
        string temp = "\"";
        pos++;

        while(pos < (int)s.size() && s[pos]!='"') {
            if(s[pos]=='\\') {
                temp.push_back(s[pos]);
                temp.push_back(s[pos+1]);
                pos++;
            } 
        
            else {
                temp.push_back(s[pos]);
            }
            pos++;
        }

        if(pos<(int)s.size()) {
            temp.push_back('\"');
        }

        lexemes.push_back(temp);
        return;
    }

    string temp = s.substr(pos, 2);
    if(double_operators.find(temp) != double_operators.end()) {
        lexemes.push_back(temp);
        pos++;
    }

    else {
        if(whitespaces.find(s[pos]) == whitespaces.end())
            lexemes.push_back(string(1, s[pos]));
    }
}

bool check_numeric(string token) {
    int n = token.size();

    for(int i=0;i<n;i++) {
        if(!isdigit(token[i])) {
            return false;
        }
    }

    return true;
}

bool check_indentifier(string token) {
    int n = token.size();

    if(!(isalpha(token[0]))) {
        return false;
    }

    for(int i=1;i<n;i++) {
        if(!isdigit(token[i]) && !isalpha(token[i])) {
            return false;
        }
    }

    return true;
}

bool check_float_literal(string token) {
    int decimal_count=0;
    int n = token.size();

    for(int i=0;i<n;i++) {
        if(token[i]=='.') {
            decimal_count++;
        }
        else if(!isdigit(token[i])) {
            return false;
        }
    }

    if(decimal_count!=1) return false;
    else return true;
}

bool check_string_literal(string token) {
    if(token[0]=='\"') {
        return true;
    }

    return false;
}

signed main() {
    string s;
    getline(cin, s);

    //TODO handle the case with strings
    delimeters = {'<', '>', '=', '&', '|', '+', '-',
                    '*', '/', '^', '%', ' ', ',', '[', 
                    ']', '{', '}', '(', ')', ';', '"', '\'', '\n',
                    '\t', '\r'};

    double_operators = {"<<", ">>", "==", "&&", "||", "++",
                        "+=", "--", "-=", "**", "//", "!=",
                        "*=", "/="};

    whitespaces = {' ', '\n', '\t', '\r'};

    keywords = {"int", "char", "float", "bool", "if", 
                "else", "for", "main", "true", "false",
                "function", "return"};

    arithmetic_operators = {"+","*","/","-","%","<<",">>","**","//"};

    logical_operators = {"&&","||"};

    comparators = {"<",">","==",">=","<=","!="};

    assignment_operators = {"=","+=","-=","*=","/="};

    unary_operators = {"++","--","!","+","-","~"};

    special_chars = {"(",")","{","}","[","]",";",","};

    int n = s.size();
    string temp = "";
    for(int i=0;i<n;i++) {
        if(delimeters.find(s[i]) != delimeters.end()) {
            if(!temp.empty()) {
                lexemes.push_back(temp);
            }

            temp.clear();
            handle_delimeter(s, i);
        }

        else {
            temp.push_back(s[i]);
        }
    }
    if(!temp.empty()) {
        lexemes.push_back(temp);
    }

    for(int i=0;i<(int)lexemes.size();i++) cout<<lexemes[i]<<endl;

    /***************************************finished obtaining lexemes*************************************/

    for(int i=0;i<(int)lexemes.size();i++) {
        if(arithmetic_operators.find(lexemes[i]) != arithmetic_operators.end()) {
            cout<<lexemes[i]<<" "<<"arithmetic_operator"<<endl;
        }
        else if(logical_operators.find(lexemes[i]) != logical_operators.end()) {
            cout<<lexemes[i]<<" "<<"logical_operator"<<endl;
        }
        else if(unary_operators.find(lexemes[i]) != unary_operators.end()) {
            cout<<lexemes[i]<<" "<<"unary_operator"<<endl;
        }
        else if(keywords.find(lexemes[i]) != keywords.end()) {
            cout<<lexemes[i]<<" keyword"<<endl;
        }
        else if(special_chars.find(lexemes[i]) != special_chars.end()) {
            cout<<lexemes[i]<<" special_char"<<endl;
        }
        else if(comparators.find(lexemes[i]) != comparators.end()) {
            cout<<lexemes[i]<<" comparator"<<endl;
        }
        else if(check_numeric(lexemes[i])) {
            cout<<lexemes[i]<<" number"<<endl;
        }
        else if(check_indentifier(lexemes[i])) {
            cout<<lexemes[i]<<" identifier"<<endl;
        }
        else if(check_string_literal(lexemes[i])) {
            cout<<lexemes[i]<<" string literal"<<endl;
        }
        else if(check_float_literal(lexemes[i])) {
            cout<<lexemes[i]<<" decimal literal"<<endl;
        }
    }
}
