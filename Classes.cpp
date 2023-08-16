//
// Created by David Denis on 4/6/23.
//
#include "Classes.h"

// ***** TILE CLASS METHODS *****
Tile::Tile(map<string, sf::Sprite> &m) {
    row = 0;
    col = 0;
    value = 0;
    map_of_sprites = m;
}

// Increments neighbor's value if it is not a mine
void Tile::incrementNeighbors() {

    for(int i = 0; i < adjacentTiles.size(); i++)
    {
        if(!adjacentTiles[i]->isMine)
            adjacentTiles[i]->incrementValue();
    }

}


// Increment Value and change sprite accordingly
void Tile::incrementValue() {

    value++;


    if(value == 0)
        tile_value = map_of_sprites["tile_revealed"];
    else if(value == 1)
        tile_value = map_of_sprites["num_1"];
    else if(value == 2)
        tile_value = map_of_sprites["num_2"];
    else if(value == 3)
        tile_value = map_of_sprites["num_3"];
    else if(value == 4)
        tile_value = map_of_sprites["num_4"];
    else if(value == 5)
        tile_value = map_of_sprites["num_5"];
    else if(value == 6)
        tile_value = map_of_sprites["num_6"];
    else if(value == 7)
        tile_value = map_of_sprites["num_7"];
    else if(value == 8)
        tile_value = map_of_sprites["num_8"];

    tile_value.setPosition(tile_revealed.getPosition());
}


// reveals clicked tile and non-mine neighbors
void Tile::revealTile(int &num_r) {

    if(isMine)
        return;
    else if(isRevealed)
        return;
    else if(isFlagged)
        return;
    else if(isChecked)
        return;
    if(value != 0 and !isMine and !isRevealed)
    {
        isRevealed = true;
        isChecked = true;
        num_r++;
        return;
    }
    else if(value == 0)
    {
        isRevealed = true;
        isChecked = true;
        num_r++;

        for(int i = 0; i < adjacentTiles.size(); ++i)
        {
            adjacentTiles[i]->revealTile(num_r);
        }

        return;
    }

}



// ***** BOARD CLASS METHODS *****


Board::Board(int b_width, int b_height, int b_rows, int b_cols, int mines, int tiles_num, map<string, sf::Sprite> &sprites, map<int, sf::Sprite> &digits) {
    width = b_width;
    height = b_height;
    rows = b_rows;
    cols = b_cols;
    numMines = mines;
    r_flagged = numMines;
    map_sprites = sprites;
    digitsMap = digits;
    tiles = tiles_num;
    tiles_revealed = 0;


    // initialize sprites (buttons)
    face_happy = map_sprites["face_happy"];
    face_win = map_sprites["face_win"];
    face_lose = map_sprites["face_lose"];
    debug = map_sprites["debug"];
    pause = map_sprites["pause"];
    play = map_sprites["play"];
    leaderboard = map_sprites["leaderboard"];



    board.resize(rows * cols);

    // start board func
    initializeBoard();
    initializeTileNeighbors();
    initializeMines();
}

// initialize board with hidden tiles
void Board::initializeBoard() {

    float positionX = 0;
    float positionY = 0;

    for(int i = 0; i < rows; i++)
    {

        for(int j = 0; j < cols; j++)
        {
            Tile* tile = new Tile(map_sprites);
            tile->row = i;
            tile->col = j;
            tile->tile_hidden = map_sprites["tile_hidden"];
            tile->tile_hidden.setPosition(positionX, positionY);

            tile->tile_revealed = map_sprites["tile_revealed"];
            tile->tile_revealed.setPosition(positionX, positionY);

            tile->flag = map_sprites["flag"];
            tile->flag.setPosition(positionX, positionY);

            tile->mine = map_sprites["mine"];
            tile->mine.setPosition(positionX, positionY);


            board[i].push_back(tile);

            positionX += 32;
        }
        positionX = 0;
        positionY += 32;
    }
}

// initialize tile's neighbors [edge cases]
void Board::initializeTileNeighbors() {

    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            if(i == 0 and j == 0) {  // top left
                board[i][j]->adjacentTiles.push_back(board[i + 1][j]);
                board[i][j]->adjacentTiles.push_back(board[i][j + 1]);
                board[i][j]->adjacentTiles.push_back(board[i + 1][j + 1]);

            }

            else if(i == rows - 1 and j == cols - 1) {   // bottom right
                board[i][j]->adjacentTiles.push_back(board[i - 1][j]);
                board[i][j]->adjacentTiles.push_back(board[i][j - 1]);
                board[i][j]->adjacentTiles.push_back(board[i - 1][j - 1]);
            }

            else if(i == rows - 1 and j == 0){  // bottom left
                board[i][j]->adjacentTiles.push_back(board[i - 1][j]);
                board[i][j]->adjacentTiles.push_back(board[i][j + 1]);
                board[i][j]->adjacentTiles.push_back(board[i - 1][j + 1]);
            }

            else if(i == 0 and j == cols - 1){  // top right
                board[i][j]->adjacentTiles.push_back(board[i + 1][j]);
                board[i][j]->adjacentTiles.push_back(board[i][j - 1]);
                board[i][j]->adjacentTiles.push_back(board[i + 1][j - 1]);
            }

            else if(i == 0 and j > 0){  // top row of board
                board[i][j]->adjacentTiles.push_back(board[i][j - 1]);
                board[i][j]->adjacentTiles.push_back(board[i][j + 1]);
                board[i][j]->adjacentTiles.push_back(board[i + 1][j]);
                board[i][j]->adjacentTiles.push_back(board[i + 1][j - 1]);
                board[i][j]->adjacentTiles.push_back(board[i + 1][j + 1]);
            }

            else if(i > 0 and j == 0){  // left column of board
                board[i][j]->adjacentTiles.push_back(board[i][j + 1]);
                board[i][j]->adjacentTiles.push_back(board[i - 1][j]);
                board[i][j]->adjacentTiles.push_back(board[i + 1][j]);
                board[i][j]->adjacentTiles.push_back(board[i - 1][j + 1]);
                board[i][j]->adjacentTiles.push_back(board[i + 1][j + 1]);
            }

            else if(i == rows - 1 and j > 0){  // bottom row of board
                board[i][j]->adjacentTiles.push_back(board[i][j - 1]);
                board[i][j]->adjacentTiles.push_back(board[i][j + 1]);
                board[i][j]->adjacentTiles.push_back(board[i - 1][j]);
                board[i][j]->adjacentTiles.push_back(board[i - 1][j - 1]);
                board[i][j]->adjacentTiles.push_back(board[i - 1][j + 1]);
            }

            else if(i > 0 and j == cols - 1){   // right column of board
                board[i][j]->adjacentTiles.push_back(board[i][j - 1]);
                board[i][j]->adjacentTiles.push_back(board[i - 1][j]);
                board[i][j]->adjacentTiles.push_back(board[i + 1][j]);
                board[i][j]->adjacentTiles.push_back(board[i + 1][j - 1]);
                board[i][j]->adjacentTiles.push_back(board[i - 1][j - 1]);
            }
            else {  // middle board
                board[i][j]->adjacentTiles.push_back(board[i][j + 1]);
                board[i][j]->adjacentTiles.push_back(board[i][j - 1]);
                board[i][j]->adjacentTiles.push_back(board[i - 1][j]);
                board[i][j]->adjacentTiles.push_back(board[i + 1][j]);
                board[i][j]->adjacentTiles.push_back(board[i - 1][j - 1]);
                board[i][j]->adjacentTiles.push_back(board[i - 1][j + 1]);
                board[i][j]->adjacentTiles.push_back(board[i + 1][j - 1]);
                board[i][j]->adjacentTiles.push_back(board[i + 1][j + 1]);
            }
        }
    }
}

// uses rand to place mines arbitrarily
void Board::initializeMines() {

    // seed
    srand( (unsigned)time(NULL) );

    int i = numMines;

    int mineR = rand() % rows;
    int mineC = rand() % cols;

    while(i != 0){


        // check if that position already has a mine and redo if so
        while(board[mineR][mineC]->isMine) {
            mineR = rand() % rows;
            mineC = rand() % cols;
        }

        // set tile on board[mineR][mineC] to mine, and increment neighbors to indicate how many mines are nearby
        board[mineR][mineC]->isMine = true;
        board[mineR][mineC]->value = 10;
        board[mineR][mineC]->incrementNeighbors();

        i--;

    }

}


// gets tile where user clicked
bool Board::clicked(int x, int y, bool right) {

    int row = 0;
    int col = 0;

    // check if click is within bounds and display board accordingly
    if(x < width and y < height - 100) {
        row = y / 32;
        col = x / 32;

        // NO FUNCTIONALITY if (display_pause is false) (GAME IS PAUSED)

        // decrease or increase mines counter
        if(right and !paused and !won and !lost and !leaderBoardClick) {
            if(!board[row][col]->isFlagged) {
                board[row][col]->isFlagged = !board[row][col]->isFlagged;
                r_flagged--;
            }
            else {
                board[row][col]->isFlagged = !board[row][col]->isFlagged;
                r_flagged++;
            }
           // return true;
        }

        // user loses
        else if (board[row][col]->isMine and !paused and !won and !lost and !leaderBoardClick and !board[row][col]->isFlagged) {
            display_face_lose = !display_face_lose;
            display_face_happy = !display_face_happy;
            lost = !lost;
            return false;
        }
        else if(!paused and !won and !lost and !leaderBoardClick and !board[row][col]->isFlagged){
            board[row][col]->revealTile(tiles_revealed);

            if((tiles_revealed == tiles - numMines) and checkWin())
                return true;

        }

    }

    // implement showing buttons functionality
    else if(!right)
    {
        if(debug.getGlobalBounds().contains(x, y) and !paused and !leaderBoardClick) // and not game win
            debugClick = !debugClick;
        else if(leaderboard.getGlobalBounds().contains(x, y) and !lost and !won)
            leaderBoardClick = !leaderBoardClick;
        else if(pause.getGlobalBounds().contains(x, y) and !lost and !won and !leaderBoardClick){
            paused = !paused;
            display_play = !display_play;
        }
    }

    //return true;
}

bool Board::checkWin() {

    int correct = 0;


    for(int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {

            if(!board[i][j]->isRevealed and board[i][j]->isMine){
                correct++;
            }
        }
    }


    if(correct == numMines)
    {
        for(int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {

                if(board[i][j]->isMine and !board[i][j]->isFlagged){
                    board[i][j]->isFlagged = !board[i][j]->isFlagged;
                    r_flagged--;
                }
            }
        }
        won = true;
        return true;
    }

    return false;
}

// returns if game is losr
bool Board::getLostStatus() {
    return lost;
}

void Board::changeLeaderboardClick() {
    leaderBoardClick = !leaderBoardClick;
}