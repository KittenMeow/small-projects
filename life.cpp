#include <iostream>
#include <vector>

#define WIDTH 50
#define HEIGHT 50

using namespace std;

char gameboard[WIDTH][HEIGHT];

void init_array(){
    for (int i = 0; i < WIDTH; i++){
        for (int j = 0; j < HEIGHT; j++){
            gameboard[i][j] = '-';
        }
    }
}

void add_dot(int x, int y){
    gameboard[x][y] = '*';
}

void remove_dot(int x, int y){
    gameboard[x][y] = '-';
}

void print_board(){
    for (int i = 0; i < WIDTH; i++){
        for (int j = 0; j < HEIGHT; j++){
            cout << gameboard[i][j];
        }
        cout << endl;
    }

    cout << "|||||||||||||||||||||||||||||||||||||||||||||||||||||||||" << endl;
    cout << "|||||||||||||||||||||||||||||||||||||||||||||||||||||||||" << endl;
    cout << "|||||||||||||||||||||||||||||||||||||||||||||||||||||||||" << endl;
}

char tick_cell(int x, int y){
    char new_point = gameboard[x][y];

    char neighbours[8] = {gameboard[x - 1][y], gameboard[x + 1][y], gameboard[x][y + 1], gameboard[x][y - 1],
                          gameboard[x - 1][y - 1], gameboard[x + 1][y + 1], gameboard[x - 1][y + 1], gameboard[x + 1][y - 1]};
    int neighbour_count = 0;

    for (int i = 0; i < 8; i++){
        if (neighbours[i] == '*'){
            neighbour_count++;
        }
    }

    if (neighbour_count < 2){
        new_point = '-'; // Dies by underpopulation
    }
    else if (neighbour_count > 3){
        new_point = '-'; // Dies by overpopulation
    }
    else if (neighbour_count == 3){
        new_point = '*';
    }

    return new_point;
}

void tick_all(){
    char changes[WIDTH][HEIGHT];

    for (int i = 0; i < WIDTH; i++){
        for (int j = 0; j < HEIGHT; j++){
            char change = tick_cell(i, j);
            changes[i][j] = change;
        }
    }

    for (int i = 0; i < WIDTH; i++){
        for (int j = 0; j < HEIGHT; j++){
            if (changes[i][j] == '*') add_dot(i, j);
            if (changes[i][j] == '-') remove_dot(i, j);
        }
    }
}

void ticks(int num){
    for (int i = 0; i < num; i++){
        tick_all();
        print_board();
    }
}

int main(){
    init_array();
    
    add_dot(5, 5);
    add_dot(4, 4);
    add_dot(3, 4);
    add_dot(3, 5);
    add_dot(3, 6);

    print_board();

    ticks(10);

    return 0;
}