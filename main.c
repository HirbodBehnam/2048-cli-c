#define DIMENSION 4

#define TOP_ROW L"╔════╦════╦════╦════╗\n" // The top row of table
#define EMPTY_ROW L"║    ║    ║    ║    ║\n" // Empty row to just beatify the table
#define NUMBER_ROW L"║%-4d║%-4d║%-4d║%-4d║\n" // The cell that contains the numbers
#define CONNECTOR_ROW L"╠════╬════╬════╬════╣\n" // Connects cells to cells
#define BOTTOM_ROW L"╚════╩════╩════╩════╝\n" // Bottom raw of table
#define HELP "UP (w) Left (a) Down (s) Right (d)\nRestart (r) Quit (q)\n" // A simple help for user

//#define NCURSES_WIDECHAR 1 // <-- Uncomment this line for development; For some reason, clion always think that this is not enabled

/**
 * The direction that the numbers must move/shift
 */
enum DIRECTION {
    UP, LEFT, DOWN, RIGHT
};

#include <ctype.h>
#include <locale.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>

#ifdef __MINGW32__
#include <ncursesw/curses.h>
#else
#include <curses.h>
#endif

/**
 * The game board. All tiles are stored here.
 * 0 Tiles means empty
 */
int game[DIMENSION][DIMENSION];

/**
 * Prints the game in box formatted
 */
void print_game() {
    clear();
    addwstr(TOP_ROW);
    for (int i = 0; i < DIMENSION; i++) {
        addwstr(EMPTY_ROW); // print an empty row for beatifying
        wchar_t buffer[25];
#ifdef __MINGW32__ // For some reason, the mingw doesn't have the size parameter?
        swprintf(buffer, NUMBER_ROW, game[i][0], game[i][1], game[i][2], game[i][3]);
#else
        swprintf(buffer, 25, NUMBER_ROW, game[i][0], game[i][1], game[i][2], game[i][3]);
#endif
        addwstr(buffer); // print numbers
        addwstr(EMPTY_ROW); // print an empty row for beatifying
        if (i == DIMENSION - 1) // if this is the last row, close the table
            addwstr(BOTTOM_ROW);
        else // otherwise print the connector
            addwstr(CONNECTOR_ROW);
    }
    printw(HELP); // print help for user
    refresh();
}

/**
 * Puts a two in a random position
 * @return True if successful, otherwise false
 */
bool random_two() {
    struct point {
        int x, y;
    } empty_points[DIMENSION * DIMENSION];
    int counter = 0;
    // get all empty points
    for (int i = 0; i < DIMENSION; i++)
        for (int j = 0; j < DIMENSION; j++)
            if (game[i][j] == 0) {
                empty_points[counter].x = i;
                empty_points[counter].y = j;
                counter++;
            }

    // if counter is still zero, it means that there is no tiles left (game over)
    if (counter == 0)
        return false;

    // randomly choose a place to put the
    int random_point = rand() % counter;
    game[empty_points[random_point].x][empty_points[random_point].y] = 2;
    return true;
}

/**
 * Shifts all numbers to one side
 * Note that this function does not sum anything with anything else; It just shifts
 * @param d Direction to shift
 */
void shift_numbers(enum DIRECTION d) {
    switch (d) {
        case UP:
            for (int column = 0; column < DIMENSION; column++) {
                for (int row = 0; row < DIMENSION; row++) {
                    // if the cell is already filled, there is no need for searching for another number
                    if (game[row][column] !=
                        0)
                        continue;
                    for (int searching = row + 1; searching < DIMENSION; searching++) { // search for non zero cells
                        if (game[searching][column] != 0) {
                            game[row][column] = game[searching][column]; // move_and_sum the number to row
                            game[searching][column] = 0; // empty this cell to prevent picking it up later
                            break;
                        }
                    }
                }
            }
            break;
        case LEFT:
            for (int row = 0; row < DIMENSION; row++) {
                for (int column = 0; column < DIMENSION; column++) {
                    // if the cell is already filled, there is no need for searching for another number
                    if (game[row][column] != 0)
                        continue;
                    for (int searching = column + 1; searching < DIMENSION; searching++) { // search for non zero cells
                        if (game[row][searching] != 0) {
                            game[row][column] = game[row][searching]; // move_and_sum the number to column
                            game[row][searching] = 0; // empty this cell to prevent picking it up later
                            break;
                        }
                    }
                }
            }
            break;
        case DOWN:
            for (int column = 0; column < DIMENSION; column++) {
                for (int row = DIMENSION - 1; row >= 0; row--) {
                    // if the cell is already filled, there is no need for searching for another number
                    if (game[row][column] != 0)
                        continue;
                    for (int searching = row - 1; searching >= 0; searching--) { // search for non zero cells
                        if (game[searching][column] != 0) {
                            game[row][column] = game[searching][column]; // move_and_sum the number to row
                            game[searching][column] = 0; // empty this cell to prevent picking it up later
                            break;
                        }
                    }
                }
            }
            break;
        case RIGHT:
            for (int row = 0; row < DIMENSION; row++) {
                for (int column = DIMENSION - 1; column >= 0; column--) {
                    // if the cell is already filled, there is no need for searching for another number
                    if (game[row][column] != 0)
                        continue;
                    for (int searching = column - 1; searching >= 0; searching--) { // search for non zero cells
                        if (game[row][searching] != 0) {
                            game[row][column] = game[row][searching]; // move_and_sum the number to column
                            game[row][searching] = 0; // empty this cell to prevent picking it up later
                            break;
                        }
                    }
                }
            }
            break;
    }
}

/**
 * Move the numbers and sum the same numbers
 * @param d in this direction
 */
void move_and_sum(enum DIRECTION d) {
    shift_numbers(d); // shift to make the numbers stick to edge
    // add same numbers
    switch (d) {
        case UP:
            for (int column = 0; column < DIMENSION; column++) {
                for (int row = 0; row < DIMENSION - 1; row++) {
                    if (game[row][column] == game[row + 1][column]) {
                        game[row][column] *= 2;
                        game[row + 1][column] = 0;
                    }
                }
            }
            break;
        case LEFT:
            for (int row = 0; row < DIMENSION; row++) {
                for (int column = 0; column < DIMENSION - 1; column++) {
                    if (game[row][column] == game[row][column + 1]) {
                        game[row][column] *= 2;
                        game[row][column + 1] = 0;
                    }
                }
            }
            break;
        case DOWN:
            for (int column = 0; column < DIMENSION; column++) {
                for (int row = DIMENSION - 1; row > 0; row--) {
                    if (game[row][column] == game[row - 1][column]) {
                        game[row][column] *= 2;
                        game[row - 1][column] = 0;
                    }
                }
            }
            break;
        case RIGHT:
            for (int row = 0; row < DIMENSION; row++) {
                for (int column = DIMENSION - 1; column > 0; column--) {
                    if (game[row][column] == game[row][column - 1]) {
                        game[row][column] *= 2;
                        game[row][column - 1] = 0;
                    }
                }
            }
            break;
    }
    shift_numbers(d); // shift to make the numbers stick to edge again
}

int main() {
    setlocale(LC_ALL, ""); // fix boxing chars
    initscr(); // init ncurses
    noecho(); // disable char echo
    cbreak(); // disable read until new line
    // set the random seed
    srand(time(0));
    // game loop
    while (1) {
        GAME_LOOP:
        // at very first fill two cells randomly
        memset(game, 0, sizeof(game));
        random_two();
        random_two();
        // print the game
        while (1) {
            print_game();
            int c = getch();
            switch (c) {
                case KEY_UP:
                case 'W':
                case 'w':
                    move_and_sum(UP);
                    break;
                case KEY_LEFT:
                case 'A':
                case 'a':
                    move_and_sum(LEFT);
                    break;
                case KEY_DOWN:
                case 'S':
                case 's':
                    move_and_sum(DOWN);
                    break;
                case KEY_RIGHT:
                case 'D':
                case 'd':
                    move_and_sum(RIGHT);
                    break;
                case 'Q':
                case 'q':
                    goto END;
                case 'R':
                case 'r':
                    goto GAME_LOOP;
                default:
                    continue;
            }
            if (!random_two())
                break;
        }
        printw("You lost! Play again? (Y/n) ");
        refresh();
        if (tolower(getch()) == 'n')
            break;
    }
    END:
    endwin();
    return 0;
}
