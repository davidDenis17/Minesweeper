#ifndef PROJECT3_MINESWEEPER_CLASSES_H
#define PROJECT3_MINESWEEPER_CLASSES_H
#endif //PROJECT3_MINESWEEPER_CLASSES_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <unordered_map>
#include <cstdlib>
#include <chrono>
using namespace std;

struct Tile
{
    int value;
    int row;
    int col;
    bool isChecked = false;
    bool isMine = false;
    bool isRevealed = false;
    bool isFlagged = false;

    vector<Tile*> adjacentTiles;
    map<string, sf::Sprite> map_of_sprites;
    sf::Sprite tile_hidden;
    sf::Sprite tile_revealed;
    sf::Sprite tile_value;
    sf::Sprite flag;
    sf::Sprite mine;


    Tile(map<string, sf::Sprite> &m);

    void revealTile(int &num_r);
    void incrementNeighbors();
    void incrementValue();

};

class Board : public sf::Drawable
{
    int width;
    int height;
    int rows;
    int cols;
    int numMines;
    int r_flagged;
    int tiles_revealed;
    int tiles;
    vector<vector<Tile *>> board;
    map<string, sf::Sprite> map_sprites;
    map<int, sf::Sprite> digitsMap;


    sf::Sprite face_happy;
    sf::Sprite face_win;
    sf::Sprite face_lose;
    sf::Sprite debug;
    sf::Sprite pause;
    sf::Sprite play;
    sf::Sprite leaderboard;


    bool debugClick = false;
    bool leaderBoardClick = false;
    bool faceClick = false;

    bool lost = false;   // with lose
    bool won = false;

    bool display_face_happy = true;
    bool display_face_lose = false;
    bool paused = false;
    bool display_play = false;


public:


    //Board();

    Board(int b_width, int b_height, int b_rows, int b_cols, int mines, int tiles_num, map<string, sf::Sprite> &sprites, map<int, sf::Sprite> &digits);

    void initializeBoard();

    void initializeTileNeighbors();

    void initializeMines();

    bool clicked(int x, int y, bool right = false);

    bool checkWin();

    bool getLostStatus();

    void changeLeaderboardClick();


    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {

        for(int i = 0; i < rows; i++)
        {
            for(int j = 0; j < cols; j++)
            {
                // BUTTONS FUNCTIONALITIES

                // face happy button
                if(display_face_happy)
                    target.draw(face_happy);

                if(display_face_lose)
                    target.draw(face_lose);

                // pause/play button
                if(!paused)
                    target.draw(pause);
                else {
                    target.draw(play);
                    target.draw(board[i][j]->tile_revealed);
                }

                if(leaderBoardClick){
                    target.draw(board[i][j]->tile_revealed);
                }

                // BOARD

                // Debug Button
                if(board[i][j]->isMine and debugClick and !paused and !lost and !leaderBoardClick and !won){
                    target.draw(board[i][j]->tile_hidden);
                    target.draw(board[i][j]->mine);
                }
                // Tiles Revealed
                else if(board[i][j]->isRevealed and !paused and !lost and !won and !leaderBoardClick) {
                    target.draw(board[i][j]->tile_revealed);
                    target.draw(board[i][j]->tile_value);
                }
                // Tiles Flagged
                else if(board[i][j]->isFlagged and !paused and !lost and !leaderBoardClick){
                    target.draw(board[i][j]->tile_hidden);
                    target.draw(board[i][j]->flag);
                }
                // Pause Button
                else if(!paused and !lost and !won and !leaderBoardClick){
                    target.draw(board[i][j]->tile_hidden);
                }
                // LOST
                else if(lost and !paused and !leaderBoardClick){
                    target.draw(board[i][j]->tile_hidden);

                    if(board[i][j]->isRevealed){
                        target.draw(board[i][j]->tile_revealed);
                        target.draw(board[i][j]->tile_value);
                    }
                    else if(board[i][j]->isMine){
                        target.draw(board[i][j]->tile_revealed);
                        target.draw(board[i][j]->mine);
                    }
                }
                else if(won){
                    target.draw(face_win);
                    target.draw(board[i][j]->tile_hidden);

                    if(board[i][j]->isRevealed){
                        target.draw(board[i][j]->tile_revealed);
                        target.draw(board[i][j]->tile_value);
                    }
                    else if(board[i][j]->isFlagged){
                        target.draw(board[i][j]->flag);
                    }
                }


                target.draw(debug);
                target.draw(leaderboard);


                // MINE COUNTER

                int mine0 = r_flagged % 10;
                int mine1 = r_flagged / 10;
                int mine2 = r_flagged / 100;

                if(r_flagged < 0){
                    mine2 = 10;
                    sf::Sprite sprite = digitsMap.at(mine2);
                    sprite.setPosition(12, 32 * (rows + 0.5f ) + 16 );
                    target.draw(sprite);

                    mine0 *= (-1);
                    mine1 *= (-1);

                    sf::Sprite sprite1 = digitsMap.at(0);
                    sprite1.setPosition(33, 32 * ((rows) + 0.5f) + 16);
                    target.draw(sprite1);

                    sf::Sprite sprite2 = digitsMap.at(mine1);
                    sprite2.setPosition(33 + 21, 32 * ((rows) + 0.5f) + 16);
                    target.draw(sprite2);

                    sf::Sprite sprite3 = digitsMap.at(mine0);
                    sprite3.setPosition(33 + 21 + 21, 32 * ((rows) + 0.5f) + 16);
                    target.draw(sprite3);
                }
                else{
                    sf::Sprite sprite = digitsMap.at(mine2);
                    sprite.setPosition(33, 32 * ((rows) + 0.5f) + 16);
                    target.draw(sprite);

                    sf::Sprite sprite2 = digitsMap.at(mine1);
                    sprite2.setPosition(33 + 21, 32 * ((rows) + 0.5f) + 16);
                    target.draw(sprite2);

                    sf::Sprite sprite3 = digitsMap.at(mine0);
                    sprite3.setPosition(33 + 21 + 21, 32 * ((rows) + 0.5f) + 16);
                    target.draw(sprite3);

                }
            }
        }
    }
};