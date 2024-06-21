#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>

using namespace std;

// trims the leading whitespace from a string
string trimWhiteSpace(const string s) {
    size_t index = 0;
    while(index < s.size() && isspace(s[index++]));
    return s.substr(index-1);
}

// splits a string into two pieces, the first word and the rest
void splitFirstWord(const string &s, string &first, string &remains) {
    // first word or possibly the only word
    first = s.substr(0,s.find(" "));
 
    // skip all whitespace and get everything after the first word
    if (s.size() != first.size()) {
        remains = trimWhiteSpace(s.substr(s.find(" ")));
    }
    else {
        remains = "";
    }

}

// converts an entire string to lowercase
void tolower(string &s) {
    transform(s.begin(), s.end(), s.begin(), 
    [](unsigned char c) {return tolower(c);} );
}

//  prints out descriptions of all the commands
void helpCommand() {
    cout << "Music Library Help" << endl;
    cout << "------------------" << endl;
    
    cout << "Command Options: clear, exit, export, help, load, search";
    cout << endl << endl;
    
    // print out all the help descriptions for every command
    ifstream fstr("help-commands.txt");
    if (fstr.is_open())
    {
        cout << fstr.rdbuf();
        cout << endl;
        fstr.close();
    }
}