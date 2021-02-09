#include<bits/stdc++.h>
#include <string>
using namespace std;

//delimiters : <space> () {} [] , ; <> \n
//keywords : int char for if float return

vector<string> lexemes;
set<char> delimeters;
set<string> keywords;
set<string> double_operators;
set<char> whitespaces;

void test() {
    for(int i=0;i<10;i++) {
        cout<<i<<endl;
    }
}

void handle_delimeter(string &s, int &pos) {
    if(pos==s.size()-1) {
        lexemes.push_back(to_string(s[pos]));
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

signed main() {
    string s;
    getline(cin, s);

    //TODO handle the case with strings
    delimeters = {'<', '>', '=', '&', '|', '+', '-', '*', '/', '^', '%', ' ', ',', '[', ']', '{', '}', '(', ')', ';', '\n', '\t', '\r'};
    double_operators = {"<<", ">>", "==", "&&", "||", "++", "+=", "--", "-=", "**", "//", "!=", "*=", "/="};
    whitespaces = {' ', '\n', '\t', '\r'};

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

    cout<<"number of lexemes is "<<lexemes.size()<<endl;
    for(int i=0;i<lexemes.size();i++) cout<<lexemes[i]<<endl;
}
