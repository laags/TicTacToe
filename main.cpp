#include <iostream>
#include <algorithm>
#include <limits>
#include <windows.h>

using namespace std;

//################################################################################################
//This class contains functions for calculating win/tie conditions
class StateChecker {
public:
    //Function checks if there is a tie
    bool checkTie(int board[3][3]) {
        for (int i = 0; i < 3; i++) {
            //Checks board to see if there are untaken squares left
            if ((board[i][0] != 0 && board[i][0] != -1) ||
                (board[i][1] != 0 && board[i][1] != -1) || (board[i][2] != 0 && board[i][2] != -1))
                return false;
        }
        return true; //If all squares are taken and no winner is found game is a tie
    }

    //Function checks for a win
    int checkWin(int board[3][3], int player) {
        for (int i = 0; i < 3; i++) {
            //Checks horizontally to see if all squares in row are taken by supplied player
            if (board[i][0] == player && board[i][1] == player && board[i][2] == player)
                return true;
            //Checks vertically to see if all squares in column are taken by supplied player
            if (board[0][i] == player && board[1][i] == player && board[2][i] == player)
                return true;
        }
        //Checks diagonals to see if all squares are taken by supplied player
        if (board[0][0] == player && board[1][1] == player && board[2][2] == player)
            return true;
        if (board[0][2] == player && board[1][1] == player && board[2][0] == player)
            return true;
        return false; //Returns false if no win is found
    }
};

//##########################################################################################################
//This class represents the games AI, it contains a struct for storing a move as well as the
//minimax functions for determining the best possible move
class gameAI {
public:
    StateChecker checker;
    struct Move {//Struct for holding the AI's move
        int x = 0;
        int y = 0;
    };

    //Function finds the best possible move by simulating all possibility's bouncing between both players
    // and taking the move with the lowest score (lower score = better for ai, higher score = better for the player)
     Move minimax(int board[3][3], int player, int aiPlayer) {
        int score = numeric_limits<int>::max();
        Move move; //Holds AI players move coordinates
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] != player && board[i][j] != aiPlayer) { //If square is untaken
                    int savedSquare = board[i][j]; //store the value of the current square before its modified
                    board[i][j] = aiPlayer; //Set current square to AI marker
                    int temp = findMax(board, 0, -1); //Find score of current move
                    if (temp < score) { //If the move has a better score than the previously evaluated move it becomes
                        score = temp;   //the new best move
                        move.x = i;
                        move.y = j;
                    }
                    board[i][j] = savedSquare; //replace the original square value for next move
                }
            }
        }
        return move; //returns the move that had the best possible score
    }

    //Finds players best move
     int findMax(int board[3][3], int player, int aiPlayer) {
        if (checker.checkWin(board,player)) { //If the player wins by taking that move, return worst possible score
            return 10;
        }
        else if (checker.checkWin(board,aiPlayer)) { //If the AI wins by taking that move, return worst possible score
            return -10;
        }
        else if (checker.checkTie(board)) { //If move causes tie, return neutral score
            return 0;
        }
        int score = numeric_limits<int>::min();
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] != player && board[i][j] != aiPlayer) { //If square is untaken
                    int savedSquare = board[i][j]; //store the value of the current square before its modified
                    board[i][j] = player; //Set current square to players marker
                    score = max(score, findMin(board, 0, -1)); //
                    board[i][j] = savedSquare; //replace the original square value for next move
                }
            }
        }
        return score;
    }

    //Finds AI's best move
     int findMin(int board[3][3], int player, int aiPlayer) {
        if (checker.checkWin(board,player)) { //If the player wins by taking that move, return worst possible score
            return 10;
        }
        else if (checker.checkWin(board,aiPlayer)) { //If the AI wins by taking that move, return best possible score
            return -10;
        }
        else if (checker.checkTie(board)) { //If move causes tie, return neutral score
            return 0;
        }
        int score = numeric_limits<int>::max();
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] != player && board[i][j] != aiPlayer) { //If square is untaken
                    int savedSquare = board[i][j]; //store the value of the current square before its modified
                    board[i][j] = aiPlayer; //Set current square to AI's marker
                    score = min(score, findMax(board, 0, -1)); //
                    board[i][j] = savedSquare; //replace the original square value for next move
                }
            }
        }
        return score;
    }
};

//#############################################################################################################
//Game board class, contains the values for the board, takes user input, prints the board and
//loops taking moves until a winner or tie is determined
class Game {
    int board[3][3] = {{1,2,3},{4,5,6},{7,8,9}};
    int player = 0, aiPlayer = -1;
    gameAI ai;

public:
    Game() {

    }

    //Prints the game board out
    void displayBoard() {
        cout << "          +-------------------+" << endl;
        for (int i = 0; i < 3; i++) {
            cout << "          |###################|" << endl;
            cout << "          |#";
            for (int j = 0; j < 3; j++) {
                //If board square is the human players mark
                if(board[i][j]==0)
                    cout << " " << " X " << " " << "#";
                //If board square is the AI's mark
                else if (board[i][j]==-1)
                    cout << " " << " O " << " " << "#";
                //If board square is untaken
                else
                    cout << "  " << board[i][j] << "  " << "#";
            }
            cout << "|" << endl;
        }
        cout << "          |###################|" << endl;
        cout << "          +-------------------+" << endl;
    }

    //Function that gets a move from the player
    void getPlayerMove() {
        bool valid = false;
        while(!valid){ //Loops until valid move is entered
            cout << "     It's your turn! Enter a move: ";
            string square;
            bool squareTaken = true; //Flag for if square is already taken
            cin >> square;
            cin.ignore();
//            cout << endl;
            int squareInt = 0;
            if(square=="1" || square=="2" || square=="3" || square=="4" || square=="5" || square=="6" ||
                    square=="7" || square=="8" || square=="9")
                squareInt = stoi(square); //Convert input to int
            if(squareInt==1 || squareInt==2 || squareInt==3 || squareInt==4 || squareInt==5 || squareInt==6 || squareInt==7
            || squareInt==8 || squareInt==9){ //If input matches a possible square check to see if its taken
                for (int i = 0; i < 3; i++){
                    for (int j = 0; j < 3; j++){
                        if(board[i][j]==squareInt){
                            squareTaken=false;
                        }
                    }
                }
                if(squareTaken) {
                    cout << "     Square already taken" << endl;
                    continue;
                }
            }
            else { //If entered move is invalid
                cout << "     Invalid move" << endl;
                continue;
            }
            switch (squareInt) { //Set users move on board
                case 1:
                    board[0][0] = player;
                    break;
                case 2:
                    board[0][1] = player;
                    break;
                case 3:
                    board[0][2] = player;
                    break;
                case 4:
                    board[1][0] = player;
                    break;
                case 5:
                    board[1][1] = player;
                    break;
                case 6:
                    board[1][2] = player;
                    break;
                case 7:
                    board[2][0] = player;
                    break;
                case 8:
                    board[2][1] = player;
                    break;
                case 9:
                    board[2][2] = player;
                    break;
            }
            valid=true;
        }
    }

    //Find out if the user wants to go first or second
    int firstOrSecond(){
        bool valid = false;
        do {
            cout << "Would you like to go first or second? Type '1' or '2': " << endl;
            string answer;
            cin >> answer;
            cin.ignore();
            if(answer == "1") {
                return 1;
                valid = true;
            }
            else if (answer == "2"){
                return 2;
                valid = true;
            }
            else
                cout << "Invalid input" << endl;
        }while(!valid);
    }

    //Main game loop
    void play() {
        int turn = firstOrSecond();
        bool gameOver = false;
        cout << " _____ _        _____            _____            _ \n"
                "|_   _(_)      |_   _|          |_   _|          | |\n"
                "  | |  _  ___    | | __ _  ___    | | ___   ___  | |\n"
                "  | | | |/ __|   | |/ _` |/ __|   | |/ _ \\ / _ \\ | |\n"
                "  | | | | (__    | | (_| | (__    | | (_) |  __/ |_|\n"
                "  \\_/ |_|\\___|   \\_/\\__,_|\\___|   \\_/\\___/ \\___| (_)\n"
                "                                                    \n";
        displayBoard();
        do {
            if (turn == 1) { //Humans turn to move
                getPlayerMove();
                if (ai.checker.checkWin(board,player)) { //Check if human won
                    cout << "You Win!" << endl;
                    gameOver = true;
                }
                cout << "\n__   __                ___  ___                   \n"
                        "\\ \\ / /                |  \\/  |                 _ \n"
                        " \\ V /___  _   _ _ __  | .  . | _____   _____  (_)\n"
                        "  \\ // _ \\| | | | '__| | |\\/| |/ _ \\ \\ / / _ \\    \n"
                        "  | | (_) | |_| | |    | |  | | (_) \\ V /  __/  _ \n"
                        "  \\_/\\___/ \\__,_|_|    \\_|  |_/\\___/ \\_/ \\___| (_)\n"
                        "                                                  " << endl;
                turn = 2;
            }
            else { //AI's turn to move
                cout << "\n  ___ _____     ___  ___                   \n"
                        " / _ \\_   _|    |  \\/  |                 _ \n"
                        "/ /_\\ \\| | ___  | .  . | _____   _____  (_)\n"
                        "|  _  || |/ __| | |\\/| |/ _ \\ \\ / / _ \\    \n"
                        "| | | || |\\__ \\ | |  | | (_) \\ V /  __/  _ \n"
                        "\\_| |_|___/___/ \\_|  |_/\\___/ \\_/ \\___| (_)\n"
                        "                                           " << endl;
                gameAI::Move aisMove = ai.minimax(board, player, aiPlayer); //Calculate best possible move
                board[aisMove.x][aisMove.y] = aiPlayer; //Place AI marker on chosen square
                if (ai.checker.checkWin(board,aiPlayer)) { //Check if AI won
                    cout << "  ___ _____   _    _               _ \n"
                            " / _ \\_   _| | |  | |             | |\n"
                            "/ /_\\ \\| |   | |  | | ___  _ __   | |\n"
                            "|  _  || |   | |/\\| |/ _ \\| '_ \\  | |\n"
                            "| | | || |_  \\  /\\  / (_) | | | | |_|\n"
                            "\\_| |_|___/   \\/  \\/ \\___/|_| |_| (_)\n"
                            "                                     " << endl;
                    gameOver = true;
                }
                turn = 1;
            }
            if (ai.checker.checkTie(board)) { //Check if game ended in a tie
                cout << " _____ _        _____                        _ \n"
                        "|_   _(_)      |  __ \\                      | |\n"
                        "  | |  _  ___  | |  \\/ __ _ _ __ ___   ___  | |\n"
                        "  | | | |/ _ \\ | | __ / _` | '_ ` _ \\ / _ \\ | |\n"
                        "  | | | |  __/ | |_\\ \\ (_| | | | | | |  __/ |_|\n"
                        "  \\_/ |_|\\___|  \\____/\\__,_|_| |_| |_|\\___| (_)\n"
                        "                                               " << endl;
                gameOver = true;
            }
            displayBoard(); //Print of the current board
        } while (!gameOver); //Loop until exit = true
    }
};

int main() {
    Game game; //Create game object
    game.play(); //Start main game loop
}