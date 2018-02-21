// CMPT361 Assignment 1
// Zongrui Dai
// 301251330


#ifndef grid
#define grid

class Grid {
private:
    int blocks[20][10];
    
public:
    Grid();
    void reset();

    int numOfPoints;
    //bool collisionCheck(bool tetro_blocks[4][4], int steps, int cur_x); // detect collision between blocks and boundary
    //bool topCheck(bool tetro_blocks[4][4], int steps); // detect if the tile has reach the top of grid
    void addBlocks(bool tetro_blocks[4][4], int steps, int cur_x, int color_id); // create new blocks into the grid

    void write_buffer();
};

#endif /* defined(__tetris__Board__) */