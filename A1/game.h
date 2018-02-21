#ifndef game
#define game

#include <iostream>
#include "include/Angel.h"
//#include "Tetromino.h"
#include "Board.h"

//class Tetromino;

class Game {
    
private:
    bool is_game_over;
    Tetromino tetromino;
    Board board;
    
protected:
    void init();
    void reset();
    static void display();
    static void keyboard(unsigned char key, int x, int y);
    static void special(int key, int x, int y);
    static void idle();
    
public:
    static Game *singleton;
    void game_over();
    void run(int argc, char **argv);
};

#endif /* defined(__tetris__game__) */
