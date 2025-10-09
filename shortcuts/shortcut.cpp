#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <limits.h>

using namespace std;

bool file_exists(const char *name){
    struct stat buffer;
    return (stat (name, &buffer) == 0);
}

int main(int argc, char* argv[]){
    if (argc > 3){
        cout << "Too many arguments!" << endl;
        return 1;
    }
    if (argc < 3){
        cout << "Too few arguments!" << endl;
        return 1;
    }

    if (!file_exists(argv[1])){
        cout << "File " << argv[1] << " does not exist!" << endl;
        return 2;
    }

    ofstream link(argv[2]);

    char full_path[PATH_MAX];
    char *ptr = realpath(argv[1], full_path);
    if (ptr == NULL){
        cout << "realpath() error!" << endl;
        return 3;
    }

    cout << full_path << endl;

    link << full_path;

    link.close();

    return 0;
}
