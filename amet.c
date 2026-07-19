// Autumn's Mediocre Editor of Text (AMET)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>

#define FILE_LIMIT 4096
#define LINE_LIMIT 1000

int* fix_cursor(int position[2]){
    if (position[0] >= COLS){
        position[0] = COLS - 1;
    } 
    if (position[0] < 0){
        position[0] = 0;
    }

    if (position[1] >= LINES){
        position[1] = LINES - 1;
    } 
    if (position[1] < 0){
        position[1] = 0;
    }

    return position;
}

void insert(char* buffer, char to_insert, int index){
    char* new_buffer = malloc(FILE_LIMIT * sizeof(char));

    strncpy(new_buffer, buffer, index);
    new_buffer[index] = to_insert;
    strcpy(new_buffer + index + 1, buffer + index);
    
    strcpy(buffer, new_buffer);
    free(new_buffer);

}

int screen_to_real(char* screen, int position[2], int scroll_position){
    position = fix_cursor(position);
    int row = 0;
    int col = 0;

    for (int i = 0; i <= strlen(screen); i++){
        if ((position[0] == col) && ((position[1] + scroll_position) == row)) return i;

        if (screen[i] == '\n') {
            row++;
            col = 0;
        } else {
            col++;
        }
    }

    return -1;
}

int* real_to_screen(char* screen, int position, int scroll_position){
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
    buffer[1] = row - scroll_position;
    return buffer;
}

int allow_vertical_arrow_helper(char* screen, int next_position[2], int cur_position, int scroll_position, bool up){
    char* last_newline, *next_newline, *end_of_line;
    int last_index, next_index_nl;

    char strrchr_tmp[cur_position + 1];
    strncpy(strrchr_tmp, screen, cur_position);
    strrchr_tmp[cur_position + 1] = '\0';
    last_newline = strrchr(strrchr_tmp, '\n');
    end_of_line = strchr(screen + cur_position, '\n');

    if (last_newline == NULL) last_index = 0;
    else last_index = (int)(last_newline - strrchr_tmp);

    if (end_of_line == NULL) next_newline = screen + strlen(screen);
    else {
        next_newline = strchr(end_of_line + 1, '\n');
    }
    if (next_newline == NULL) next_newline = screen + strlen(screen);
    next_index_nl = (int)(next_newline - screen);

    int next_index = screen_to_real(screen, next_position, scroll_position);

    if (up){
        if (next_index >= 0) return next_index;
        return last_index;
    } else {
        if (next_index < 0) return next_index_nl;
        return next_index;
    }
}

int allow_horizontal_arrow_helper(char* screen, int next_position[2], int cur_position, int scroll_position, bool right){
    int next_index = screen_to_real(screen, next_position, scroll_position);
    if (next_index < 0) return cur_position;
    if (right){
        if (screen[next_index - 1] == '\n') return cur_position;
    }
    else {
        if ((next_index < 0) || (screen[next_index] == '\n')) return cur_position;
    }
    return next_index;
}

int handle_arrows(int** position, int real_position, char* screen, int scroll_position, int arrow){
    int new_position = real_position;

    if (arrow == 1){ // Up
        (*position)[1]--;
        new_position = allow_vertical_arrow_helper(screen, *position, real_position, scroll_position, true);
        *position = real_to_screen(screen, new_position, scroll_position);
    }

    else if (arrow == 3){ // Down
        (*position)[1]++;
        new_position = allow_vertical_arrow_helper(screen, *position, real_position, scroll_position, false);
        *position = real_to_screen(screen, new_position, scroll_position);
    }

    else if (arrow == 0){ // Left
        (*position)[0]--;
        new_position = allow_horizontal_arrow_helper(screen, *position, real_position, scroll_position, false);
        *position = real_to_screen(screen, new_position, scroll_position);
    }

    else if (arrow == 2){ // Right
        (*position)[0]++;
        new_position = allow_horizontal_arrow_helper(screen, *position, real_position, scroll_position, true);
        *position = real_to_screen(screen, new_position, scroll_position);
    }

    return new_position;
}

int handle_scroll(int scroll_position, int position[2], bool up){
    if (up){
        if ((position[1]) < scroll_position) return --scroll_position;
    }
    else {
        if (position[1] >= (LINES - 1)) return ++scroll_position;
    }

    return scroll_position;
}

char* substring(const char* input, int offset, int len, char* destination){
    int in_len = strlen(input);

    if ((offset + len) > in_len) return NULL;

    strncpy(destination, input + offset, len);
    destination[len + 1] = 0;

    return destination;
}

void get_display(char* display, char* screen, int scroll_position){
    int start_position[2] = {0, scroll_position};

    int start_index = screen_to_real(screen, start_position, 0);
    int end_index = strlen(screen);

    display = substring(screen, start_index, end_index - start_index, display);
}

void handle_backspace(char* screen, int real_position){
    char* updated = malloc(FILE_LIMIT * sizeof(char));

    strncpy(updated, screen, real_position - 1);
    strcpy(updated + real_position - 1, screen + real_position);

    memset(screen, 0, FILE_LIMIT * sizeof(char));

    strcpy(screen, updated);
    free(updated);
}

int main(){
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    int scroll_position = 0;
    // Position of the cursor
    int* position = malloc(2 * sizeof(int));
    position[0] = 0;
    position[1] = 0;

    // Index of the buffer
    int real_position = 0;

    // Buffer to be written
    char* screen_buffer = malloc(FILE_LIMIT * sizeof(char));
    char* display_buffer = malloc((COLS * LINES * sizeof(char)) + 1);

    while (true){
        int ch = getch();

        memset(display_buffer, 0, (COLS * LINES * sizeof(char)) + 1);
        
        int old_scroll = scroll_position;

        switch(ch){
            case KEY_F(1):
                goto cleanup;
                break;
            case KEY_BACKSPACE:
                if (position[0] > 0){
                    handle_backspace(screen_buffer, real_position);
                    real_position--;
                    position[0]--;
                }
                break;
            case KEY_UP:
                scroll_position = handle_scroll(scroll_position, position, true);
                real_position = handle_arrows(&position, real_position, screen_buffer, scroll_position, 1);
                break;
            case KEY_DOWN:
                scroll_position = handle_scroll(scroll_position, position, false);    
                real_position = handle_arrows(&position, real_position, screen_buffer, scroll_position, 3);
                break;
            case KEY_LEFT:
                real_position = handle_arrows(&position, real_position, screen_buffer, scroll_position, 0);
                break;
            case KEY_RIGHT:
                real_position = handle_arrows(&position, real_position, screen_buffer, scroll_position, 2);
                break;
            default:
                if (ch == '\n'){
                    scroll_position = handle_scroll(scroll_position, position, false);
                    position[0] = 0;
                    position[1]++;
                } else {
                    position[0]++;
                }
                insert(screen_buffer, ch, real_position);
                real_position++;

        }
        
        position = fix_cursor(position);
        get_display(display_buffer, screen_buffer, scroll_position);
        clear();
        mvprintw(0, 0, "%s", display_buffer);
        move(position[1], position[0]);
        refresh();
    }
    
cleanup:
    free(display_buffer);
    free(screen_buffer);
    free(position);
    endwin();

    return 0;
}
