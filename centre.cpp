#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include <vector>

using namespace std;

vector<string> tokenize(string text, char delim){
    vector<string> tokens;
    stringstream check1(text);
    string intermediate;

    while (getline(check1, intermediate, delim)){
        tokens.push_back(intermediate);
    }

    return tokens;
}

int main(){
    string text = "Hello!\nMeow Cat";
    vector<string> tokens = tokenize(text, '\n');

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    int columns = w.ws_col;
    int centre = (w.ws_col >> 1);
    
    vector<int> move_by;

    for (int i = 0; i < tokens.size(); i++){
        int text_centre = (tokens[i].size() >> 1);
        move_by.push_back(centre - text_centre);
    }

    for (int i = 0; i < tokens.size(); i++){
        string spacer(move_by[i], ' ');
        cout << spacer << tokens[i] << endl;
    }

    return 0;
}