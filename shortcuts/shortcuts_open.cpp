#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <cstdlib>
#include <unistd.h>

using namespace std;

bool file_exists(const char *name){
    struct stat buffer;
    return (stat (name, &buffer) == 0);
}

int main(int argc, char* argv[]){
    if (argc > 2){
        cout << "Too many arguments!" << endl;
        return 1;
    }
    if (argc < 2){
        cout << "Too few arguments!" << endl;
        return 1;
    }
    if (!file_exists(argv[1])){
        cout << "File " << argv[1] << " does not exist!" << endl;
        return 2;
    }
    
    ifstream link(argv[1]);

    string line;
    getline(link, line);

    link.close();

    int option;
    string app, command;
    cout << "Opening " + line + "..." << endl;

    cout << "Type '1' to open with the default application, and '2' to open with your own application. ";
    cin >> option;

    if (option == 1){
        command = "bash -c 'xdg-open " + line + "'";
    }
    else if (option == 2){
        cout << "Please type the application name. ";
        cin >> app;
        command = "bash -c '" + app + ' ' + line + "'";
    }
    else {
        cout << "Invalid option.";
        return 3;
    }
        
    system(command.c_str());

    return 0;
}
