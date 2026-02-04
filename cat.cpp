#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

enum Flags {
    NUMBER = 1,
    NONBLANK = 2
};

int main(int argc, char **argv){
    string content;
    vector<string> filenames;
    enum Flags options;
    int lines = 0;
    
    for (int i = 1; i < argc; i++){
        if (argv[i][0] == '-'){
            switch (argv[i][1]) {
                case 'n':
                    options = NUMBER;
                    break;
                case 'b':
                    options = NONBLANK;
                    break;
                default:
                    cerr << "Error: unknown option " << argv[i] << endl;
                    return 1;
            }
        }
        else {
            filenames.push_back(argv[i]);
        }
    }

    for (string filename : filenames){
        ifstream file_reader(filename);

        while (getline(file_reader, content)){
            if (options == NUMBER){
                lines++;
                cout << "  " << lines << "\t";
            } 
            else if (options == NONBLANK){
                if (!content.empty()){
                    lines++;
                    cout << "  " << lines << "\t";
                }
            }

            cout << content << endl;
        }

        file_reader.close();
    }
    

    return 0;
}