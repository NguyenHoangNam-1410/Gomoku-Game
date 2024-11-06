#include <iostream>
#include <cstdlib>
#include <iomanip>

using namespace std;

#define MAX_SIZE 15

const char HORZ = 196;
const char VERT = 179;
const char TL = 218;
const char TM = 194;
const char TR = 191;
const char BL = 192;
const char BM = 193;
const char BR = 217;
const int numW = 3;
const int charW = 1;
const int numPerLine = 15;
const char VERT_START = 195;
const char VERT_END = 180;
const char VERT_MID = 197;
const string inputCommand = "Previous move/Next move/Stop [p/n/s]: ";
const string endOfHistory = "This is the end of the match.\nPrevious move/Next move/Stop [p/n/s]: ";
const string startOfGame = "This is the start of the match.\nPrevious move/Next move/Stop [p/n/s]: ";
const string invalidInput = "Illegal input, please try again: ";

enum Stone {
    NA, X, O
};

string lineString(char left, char mid, char right, char horz, int wcell, int ncell) {
    string result = "", cell = string(wcell, horz);
    result += left;
    for (int i = 0; i < ncell - 1; i++) result += cell + mid;
    result += cell + right;
    return result;
}

void displayBoard(Stone arr[][MAX_SIZE], int size) {
    //UPPERLINE
    cout << "   ";
    for (int i = 0; i < size; i++) {
        cout << "  " << (char) (i + 'a') << " ";
    }
    cout << endl;
    cout << "   " << lineString(TL, TM, TR, HORZ, 3, numPerLine) << "\n";

    //MIDDLE
    for (int i = 0; i < size; i++) {
        cout << setw(2) << size - i << " " << VERT;
        for (int j = 0; j < size; j++) {
            if (arr[i][j] == NA) cout << "   " << VERT;
            if (arr[i][j] == X) cout << " X " << VERT;
            if (arr[i][j] == O) cout << " O " << VERT;
        }
        cout << endl;
        if (i != size - 1) cout << "   " << lineString(VERT_START, VERT_MID, VERT_END, HORZ, 3, numPerLine) << "\n";
    }

    //UNDERLINE
    cout << "   " << lineString(BL, BM, BR, HORZ, 3, numPerLine) << "\n";
}

void displayBoardSimple(Stone arr[][MAX_SIZE], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            cout << arr[i][j];
        }
    }
    cout << endl;
}

bool isSpecialSymbol(char c) {
    return (c >= '!' && c <= '/') || (c >= ':' && c <= '@') ||
           (c >= '[' && c <= '`') || (c >= '{' && c <= '~');
}

bool makeMove(Stone board[][MAX_SIZE], int size, string playerMove, bool isFirstPlayerTurn) {
    //BEGIN TODO
    int row, col;
    if (playerMove.size() < 2 || playerMove.size() > 3 || isSpecialSymbol(playerMove[0]) || isalpha(playerMove[0]) || playerMove[0] == '0')
        return false;
    if (playerMove[0] == '1') {
        if (!isalpha(playerMove[1])) {
            row = 10 + playerMove[1] - '0';
            if (row > size || playerMove.size() != 3 || !isalpha(playerMove[2])) return false;
            col = playerMove[2] - 'a';
        } else {
            if(playerMove.size() != 2 || !isalpha(playerMove[1])) return false;
            row = 1;
            col = playerMove[1] - 'a';
        }
    } else {
        if(playerMove.size() != 2 || !isalpha(playerMove[1])) return false;
        row = playerMove[0] - '0';
        col = playerMove[1] - 'a';
    }
    if (col < 0 || col > size - 1 || board[size - row][col] != NA) return false;
    else board[size - row][col] = (isFirstPlayerTurn) ? X : O;
    return true;
    //END TODO
}

bool checkDirection(Stone board[][MAX_SIZE], int player, int row, int col, int dRow, int dCol) {
    int count = 0;
    int opponent = (player == X) ? O : X;
    bool blockedStart = false, blockedEnd = false;
    if (row - dRow >= 0 && row - dRow < MAX_SIZE && col - dCol >= 0 && col - dCol < MAX_SIZE) {
        if (board[row - dRow][col - dCol] == opponent) blockedStart = true;
        if (board[row - dRow][col - dCol] == player) return false;
    }
    for (int i = 0; i < 6; ++i) {
        int newRow = row + i * dRow;
        int newCol = col + i * dCol;
        if (newRow >= 0 && newRow < MAX_SIZE && newCol >= 0 && newCol < MAX_SIZE && board[newRow][newCol] == player) {
            count++;
        } else break;
    }
    if (count > 5) return false;
    if (row + 5 * dRow >= 0 && row + 5 * dRow < MAX_SIZE && col + 5 * dCol >= 0 && col + 5 * dCol < MAX_SIZE) {
        if (board[row + 5 * dRow][col + 5 * dCol] == opponent) blockedEnd = true;
    }
    if ((count == 5 && blockedStart && blockedEnd)) return false;
    return count == 5;
}

bool hasWon(Stone board[][MAX_SIZE], int size, bool isFirstPlayerTurn) {
    //BEGIN TODO
    int player = isFirstPlayerTurn ? X : O;
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (board[row][col] == player) {
                if (checkDirection(board, player, row, col, 0, 1) ||
                    checkDirection(board, player, row, col, 1, 0) ||
                    checkDirection(board, player, row, col, 1, 1) ||
                    checkDirection(board, player, row, col, 1, -1))
                    return true;
            }
        }
    }
    return false;
    //END TODO
}

void displayHistory(string history, int numOfMoves) {
    //BEGIN TODO
    int moves = 0;
    bool isFirstPlayer = true;
    string playermove, command, historyMemory[255];
    Stone game[15][15];
    for (int i = 0; i < MAX_SIZE; i++) {
        for (int j = 0; j < MAX_SIZE; j++) {
            game[i][j] = NA;
        }
    }
    int i = 0;
    displayBoard(game, MAX_SIZE);
    cout << inputCommand;
    while (true) {
        cin >> command;
        if (command == "s") return;
        else if (command == "n") {
            if (moves == numOfMoves) {
                cout << endOfHistory;
                continue;
            }
            if (history[i] == '1' && !isalpha(history[i + 1])) {
                playermove = string(1, history[i]) + string(1, history[i + 1]) + string(1, history[i + 2]);
                i += 3;
                game[MAX_SIZE - 10 - (playermove[1] - '0')][tolower(playermove[2]) - 'a'] = isFirstPlayer ? X : O;
            } else {
                playermove = string(1, history[i]) + string(1, history[i + 1]);
                i += 2;
                game[MAX_SIZE - (playermove[0] - '0')][tolower(playermove[1]) - 'a'] = isFirstPlayer ? X : O;
            }
            isFirstPlayer = !isFirstPlayer;
            historyMemory[moves] = playermove;
            moves++;
        } else if (command == "p") {
            if (moves == 0) {
                cout << startOfGame;
                continue;
            }
            string temp = historyMemory[moves - 1];
            if (temp.size() == 2) {
                game[MAX_SIZE - (temp[0] - '0')][tolower(temp[1]) - 'a'] = NA;
                i -= 2;
            }
            if (temp.size() == 3) {
                game[MAX_SIZE - 10 - (temp[1] - '0')][tolower(temp[2]) - 'a'] = NA;
                i -= 3;
            }
            isFirstPlayer = !isFirstPlayer;
            moves--;
        } else {
            cout << invalidInput;
        }
        displayBoard(game, MAX_SIZE);
        cout << inputCommand;
    }
    //END TODO
}

void startGame() {
    Stone game[15][15];
    for (int i = 0; i < MAX_SIZE; i++) {
        for (int j = 0; j < MAX_SIZE; j++) {
            game[i][j] = NA;
        }
    }
    displayBoard(game, MAX_SIZE);
    string playerMove;
    bool player1Turn = true;
    cout << "Player 1 turn: ";
    while (cin >> playerMove) {
        if (playerMove == "ff") {
            cout << ((player1Turn) ? "Player 2 won " : "Player 1 won ");
            break;
        } else if (makeMove(game, MAX_SIZE, playerMove, player1Turn)) {
            if (hasWon(game, MAX_SIZE, player1Turn)) {
                displayBoard(game, MAX_SIZE);
                cout << ((player1Turn) ? "Player 1 won " : "Player 2 won ");
                return;
            }
            player1Turn = !player1Turn;
            displayBoard(game, MAX_SIZE);
            cout << ((player1Turn) ? "Player 1 turn: " : "Player 2 turn: ");
        } else {
            cout << "Illegal move, please try again: ";
        }
    }
}

int main() {
    cout << "Welcome to Gomoku!" << endl;
    cout << "1. Play game" << endl;
    cout << "2. History" << endl;
    cout << "3. Exit" << endl;
    cout << "Please select mode [1/2/3]: ";
    int mode;
    while (cin >> mode) {
        if (mode == 1) {
            startGame();
            return 0;
        } else if (mode == 2) {
            int numOfMoves;
            cout << "Please enter number of moves: ";
            cin >> numOfMoves;
            string history;
            cout << "Please enter history: ";
            cin >> history;
            displayHistory(history, numOfMoves);
            return 0;
        } else if (mode == 3) {
            return 0;
        } else {
            cout << "Invalid mode, please try again: ";
        }
    }
    return 0;
}
