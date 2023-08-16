#ifndef PROJECT3_MINESWEEPER_METHODS_H
#define PROJECT3_MINESWEEPER_METHODS_H
#endif //PROJECT3_MINESWEEPER_METHODS_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

// ***** SCREENS *****

bool WelcomeScreen(string &name);

bool Game_Screen(string &name);

bool LeaderBoard(int width, int height, int minutes, int seconds, string &user_name, bool won = false);

// ***** HELPER METHODS *****





// returns a map containing all sprites
map<string, sf::Sprite> getSprites(int &rows, int &cols);

// creates a new sprite given a filename
sf::Sprite createSprite(string filename);