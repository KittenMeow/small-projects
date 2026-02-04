#include <iostream>
#include <unistd.h>
#include <linux/limits.h>

using namespace std;

int main(int argc, char **argv){
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL){
        cout << cwd << endl;
    }
    else {
        cerr << "Error: getcwd() error." << endl;
        return 1;
    }

    return 0;
}