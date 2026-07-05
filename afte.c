// Autumn's Fine Text Editor
// Add feature: editable commands
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>

#define FILE_LIMIT 4096
#define LINE_LIMIT 1000

void fix_cursor(int position[2]){
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    if (position[0] >= cols){
        position[0] = cols - 1;
    } 
    if (position[0] < 0){
        position[0] = 0;
    }

    if (position[1] >= rows){
        position[1] = rows - 1;
    } 
    if (position[1] < 0){
        position[1] = 0;
    }
}

void insert(char* buffer, char to_insert, int index){
    char* new_buffer = malloc(FILE_LIMIT * sizeof(char));

    strncpy(new_buffer, buffer, index);
    new_buffer[index] = to_insert;
    strcpy(new_buffer + index + 1, buffer + index);
    
    strcpy(buffer, new_buffer);
    free(new_buffer);

}

int screen_to_real(char* screen, int position[2]){
    int row = 0;
    int col = 0;

    int i;
    for (i = 0; screen[i] != '\0'; i++){
        if ((position[0] == col) && (position[1] == row)) return i;

        if (screen[i] == '\n') {
            row++;
            col = 0;
        } else {
            col++;
        }
    }

    return i;
}

int* real_to_screen(char* screen, int position){
    int* buffer = malloc(2 * sizeof(int));

    int row = 0;
    int col = 0;

    for (int i = 0; i != position; i++){
        if (screen[i] == '\0') break;

        if (screen[i] == '\n') {
            row++;
            col = 0;
        } else {
            col++;
        }
    }

    buffer[0] = col;
    buffer[1] = row;
    return buffer;
}

int allow_vertical_arrow_helper(char* screen, int next_position[2], bool up){
    char* last_newline;
    int last_index;

    last_newline = strrchr(screen, '\n');
    if (last_newline == NULL) last_newline = screen;
    last_index = (int)(last_newline - screen);

    int next_index = screen_to_real(screen, next_position);
    if (next_index == -1) return last_index;
    if (up){
        if (next_index < last_index) return next_index;
    } else {
        if (next_index > last_index) return next_index;
    }
    return last_index;
}

int allow_right_arrow_helper(char* screen, int next_position[2]){
    int next_index = screen_to_real(screen, next_position);
    if ((screen[next_index] == '\0') && (screen[next_index - 1] == '\0')) return next_index - 1;
    if (screen[next_index - 1] == '\n') return next_index;
    return next_index;
}

int handle_arrows(int position, char* screen, int arrow){
    // 161 -> (1, 2)
    // Move to (1, 1)
    // 
    int* next_position = real_to_screen(screen, position);

    if (arrow == 1){
        next_position[1]--;
        return allow_vertical_arrow_helper(screen, next_position, true);
    }

    else if (arrow == 3){
        next_position[1]++;
        return allow_vertical_arrow_helper(screen, next_position, false);
    }

    else if (arrow == 0){
        if (next_position[0] > 0){
            next_position[0]--;
        } else {
            next_position[0] = 0;
        }
        return screen_to_real(screen, next_position);
    }

    else if (arrow == 2){
        next_position[0]++;
        return allow_right_arrow_helper(screen, next_position);
    }

    else {
        return 0;
    }
}

int handle_scroll(int scroll_position, int position[2], bool up){
    if (up){
        if ((position[1]) < scroll_position) return --scroll_position;
    }
    else {
        if ((position[1] + 1) > (LINES - 1)) return ++scroll_position;
    }

    return scroll_position;
}

int main(){
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    WINDOW *pad = newpad(LINE_LIMIT, COLS);
    wrefresh(pad);
    int scroll_position = 0;
    // Position of the cursor
    int* position = malloc(2 * sizeof(int));
    position[0] = 0;
    position[1] = 0;

    // Index of the buffer
    int real_position = 0;

    // Buffer to be written
    char* screen_buffer = malloc(FILE_LIMIT * sizeof(char));

    while (true){
        int ch = getch();

        int old_scroll = scroll_position;


        switch(ch){
            case KEY_F(1):
                goto cleanup;
                break;
            case KEY_BACKSPACE:
                if (real_position > 0){
                    real_position--;
                    screen_buffer[real_position] = '\0';
                }
                break;
            case KEY_UP:
                real_position = handle_arrows(real_position, screen_buffer, 1);
                scroll_position = handle_scroll(scroll_position, position, true);
                break;
            case KEY_DOWN:
                real_position = handle_arrows(real_position, screen_buffer, 3);
                scroll_position = handle_scroll(scroll_position, position, false);
                break;
            case KEY_LEFT:
                real_position = handle_arrows(real_position, screen_buffer, 0);
                break;
            case KEY_RIGHT:
                real_position = handle_arrows(real_position, screen_buffer, 2);
                break;
            default:
                if (ch == '\n') scroll_position = handle_scroll(scroll_position, position, false);
                insert(screen_buffer, ch, real_position);
                real_position++;
        }

        position = real_to_screen(screen_buffer, real_position);
        if (old_scroll != scroll_position) position[1] = (LINES) * scroll_position;
        real_position = screen_to_real(screen_buffer, position);
        werase(pad);
        mvwprintw(pad, 0, 0, "%s", screen_buffer);
        move(position[1], position[0]);
        prefresh(pad, scroll_position, 0, 0, 0, LINES - 1, COLS - 1);
    }
    
cleanup:
    delwin(pad);
    free(screen_buffer);
    free(position);
    endwin();

    return 0;
}
