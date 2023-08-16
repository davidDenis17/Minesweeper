//
// Created by David Denis on 4/8/23.
//
#include "Methods.h"
#include "Classes.h"

// ***** SCREENS *****

bool WelcomeScreen(string &name)
{
    const int SCRWIDTH = 800;
    const int SCRHEIGHT = 600;

    sf::RenderWindow welcome_window(sf::VideoMode(SCRWIDTH, SCRHEIGHT), "Minesweeper");


    // *** Welcome Text ***
    sf::Text welcome_text;
    sf::Font welcome_font;

    // load font
    welcome_font.loadFromFile("files/font.ttf");

    // set welcome message
    welcome_text.setFont(welcome_font);
    welcome_text.setCharacterSize(24);
    welcome_text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    welcome_text.setString("WELCOME TO MINESWEEPER!");

    // center Text
    sf::FloatRect welcome_textRect = welcome_text.getLocalBounds();
    welcome_text.setOrigin(welcome_textRect.left + welcome_textRect.width/2.0f,
                           welcome_textRect.top + welcome_textRect.height/2.0f);
    welcome_text.setPosition(sf::Vector2f(SCRWIDTH/2.0f, SCRHEIGHT/2.0f - 150));



    // *** Prompt Text ***

    sf::Text prompt_text;

    // set string
    prompt_text.setFont(welcome_font);
    prompt_text.setCharacterSize(20);
    prompt_text.setStyle(sf::Text::Bold);
    prompt_text.setString("Enter your name:");

    // center
    sf::FloatRect prompt_textRect = prompt_text.getLocalBounds();
    prompt_text.setOrigin(prompt_textRect.left + prompt_textRect.width/2.0f,
                          prompt_textRect.top + prompt_textRect.height/2.0f);
    prompt_text.setPosition(SCRWIDTH/2.0f, SCRHEIGHT/2.0f - 75);



    //*** User Input ***

    sf::Text input_text;
    // stores the user's input
    string user_input = "|";

    // set string
    input_text.setFont(welcome_font);
    input_text.setCharacterSize(18);
    input_text.setFillColor(sf::Color::Yellow);
    input_text.setStyle(sf::Text::Bold);
    input_text.setString(user_input);

    //center
    sf::FloatRect input_textRect = input_text.getLocalBounds();
    input_text.setOrigin(input_textRect.left + input_textRect.width/2.0f,
                         input_textRect.top + input_textRect.height/2.0f);
    input_text.setPosition(SCRWIDTH/2.0f, SCRHEIGHT/2.0f - 45);


    // *** DISPLAY WINDOW ***
    while(welcome_window.isOpen()) {


        sf::Event event;

        while (welcome_window.pollEvent(event)) {
            // Check type of event
            switch (event.type) {
                case sf::Event::Closed:
                    welcome_window.close();
                    break;

                case sf::Event::TextEntered:
                    if(isalpha(event.text.unicode)){
                        if(user_input.length() > 0 and user_input.length() <= 10){

                            // store input in string and append '|'
                            user_input.pop_back();
                            user_input += event.text.unicode;
                            user_input += "|";

                            // center text
                            input_textRect = input_text.getLocalBounds();
                            input_text.setString(user_input);
                            input_text.setOrigin(input_textRect.left + input_textRect.width/2.0f,
                                                 input_textRect.top + input_textRect.height/2.0f);
                            input_text.setPosition(SCRWIDTH/2.0f, SCRHEIGHT/2.0f - 45);
                        }
                    }
                case sf::Event::KeyPressed:
                    if(event.key.code == sf::Keyboard::BackSpace and user_input.length() > 1){
                        user_input.pop_back();
                        user_input.pop_back();
                        user_input += "|";

                        // center text
                        input_textRect = input_text.getLocalBounds();
                        input_text.setString(user_input);
                        input_text.setOrigin(input_textRect.left + input_textRect.width/2.0f,
                                             input_textRect.top + input_textRect.height/2.0f);
                        input_text.setPosition(SCRWIDTH/2.0f, SCRHEIGHT/2.0f - 45);
                    }
                    if(event.key.code == sf::Keyboard::Return){
                        if(user_input.length() > 1) {

                            // get rid of '|'
                            user_input.pop_back();

                            string newstr;
                            // first char to upper
                            char first = toupper(user_input[0]);
                            newstr += first;

                            // to lower case
                            for (int i = 1; i < user_input.length(); i++) {
                                char name = user_input[i];
                                newstr += tolower(name);
                            }

                            // store name in file
                            name = newstr;

                            return true;
                        }
                    }

            }
        }
        welcome_window.clear(sf::Color::Blue);
        welcome_window.draw(welcome_text);
        welcome_window.draw(prompt_text);
        welcome_window.draw(input_text);
        welcome_window.draw(input_text);
        welcome_window.display();
    }

    return false;
}

bool Game_Screen(string &name){

    while (true) {

        bool game_continue = true;

        while (game_continue) {

            int rows, cols, mines, width, height, tile_count, minutes, seconds;


            map<string, sf::Sprite> sprites;
            map<int, sf::Sprite> digitsMap;

            // TIMER/CLOCK
            auto start_time = chrono::high_resolution_clock::now();
            auto pauseTime = chrono::high_resolution_clock::now();
            auto elapsed_p_time = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - pauseTime).count();

            bool paused = false;
            bool game_won = false;
            bool game_lost = false;
            bool leaderboard = false;

            // Getting Board Specs
            string str;

            ifstream read("files/board_config.cfg");

            getline(read, str);
            cols = stoi(str);

            getline(read, str);
            rows = stoi(str);

            getline(read, str);
            mines = stoi(str);

            width = cols * 32;
            height = (rows * 32) + 100;
            tile_count = cols * rows;

            // get sprites with positions
            sprites = getSprites(rows, cols);


            // DIGITS
            for(int i = 0; i < 11; ++i)
            {
                sf::IntRect rect(i * 21, 0, 21, 32);
                sprites["digits"].setTextureRect(rect);
                sf::Sprite sprite = sprites["digits"];
                digitsMap.emplace(i, sprite);
            }


            // Initializing Board
            Board board(width, height, rows, cols, mines, tile_count, sprites, digitsMap);


            // GAME STARTS
            sf::RenderWindow game_window(sf::VideoMode(width, height), "Minesweeper");

            while (game_window.isOpen()) {


                sf::Event event;

                while (game_window.pollEvent(event)) {
                    // Check type of event
                    switch (event.type) {
                        case sf::Event::Closed:
                            game_window.close();
                            return false;

                        case sf::Event::MouseButtonPressed:

                            sf::Mouse mouse;
                            auto coordinates = mouse.getPosition(game_window);


                            if (event.mouseButton.button == sf::Mouse::Right) {
                                board.clicked(coordinates.x, coordinates.y, true);
                            }
                            else if (event.mouseButton.button == sf::Mouse::Left) {

                               if(board.clicked(coordinates.x, coordinates.y))
                                   game_won = !game_won;

                               game_lost = board.getLostStatus();

                                // start new game if user clicks on FACE SPRITE
                                if (sprites["face_happy"].getGlobalBounds().contains(coordinates.x, coordinates.y)) {
                                    game_window.close();
                                    continue;
                                }
                                else if(sprites["leaderboard"].getGlobalBounds().contains(coordinates.x, coordinates.y)){
                                    leaderboard = !leaderboard;

                                    if(leaderboard)
                                        pauseTime = chrono::high_resolution_clock::now();

                                }
                                else if(sprites["pause"].getGlobalBounds().contains(coordinates.x, coordinates.y) and !leaderboard and !game_lost){
                                    paused = !paused;

                                    if(paused)
                                        pauseTime = chrono::high_resolution_clock::now();
                                    else{
                                        auto unPausedTime = chrono::high_resolution_clock::now();
                                        elapsed_p_time +=  chrono::duration_cast<chrono::seconds>(unPausedTime - pauseTime).count();
                                    }

                                }

                            }
                    }

                    game_window.clear(sf::Color::White);
                    game_window.draw(board);

                    // draw timer
                    auto game_duration = std::chrono::duration_cast<std::chrono::seconds>(chrono::high_resolution_clock::now() - start_time);
                    int total_time = game_duration.count();

                    if(!paused and !game_won and !leaderboard and !game_lost){
                        //enters if the game is NOT paused. This is the condition that keeps the timer from incrementing when paused.
                        total_time -= elapsed_p_time;
                        minutes = total_time / 60;      // integer division
                        seconds = total_time % 60;      // get remainder
                    }


                    // will range from 0 - 9
                    int min0 = minutes / 10 % 10; //minutes index 0
                    int min1 = minutes % 10; // minutes index 1
                    int sec0 = seconds / 10 % 10; // seconds index 0
                    int sec1 = seconds % 10; // seconds index 1

                    digitsMap[min0].setPosition((cols * 32) - 97, 32 * (rows + 0.5f) + 16);
                    game_window.draw(digitsMap[min0]);

                    digitsMap[min1].setPosition((cols * 32) - 97 + 21, 32 * (rows + 0.5f) + 16);        // displaced 21px to the right
                    game_window.draw(digitsMap[min1]);

                    digitsMap[sec0].setPosition((cols * 32) - 54, 32 * (rows + 0.5f ) + 16);
                    game_window.draw(digitsMap[sec0]);

                    digitsMap[sec1].setPosition((cols * 32) - 54 + 21, 32 * (rows + 0.5f ) + 16);       // displaced 21px to the right
                    game_window.draw(digitsMap[sec1]);


                    // Display Screen
                    game_window.display();

                    // Open Leaderboard screen
                    if(leaderboard)
                    {
                        if(game_won){
                            if(LeaderBoard(width, height, minutes, seconds, name, true)){}
                            leaderboard = false;
                        }
                        else{
                            if(LeaderBoard(width, height, minutes, seconds, name)){}
                            leaderboard = false;
                        }

                        // get time elapsed
                        auto unPausedTime = chrono::high_resolution_clock::now();
                        elapsed_p_time +=  chrono::duration_cast<chrono::seconds>(unPausedTime - pauseTime).count();

                        //switch tiles to previous state
                        if(!game_lost){board.changeLeaderboardClick();}
                    }

                }
            }
        }
    }
}

bool LeaderBoard(int width, int height, int minutes, int seconds, string &user_name, bool won){

    const int WIDTH = width / 2;
    const int HEIGHT = height / 2;

    sf::RenderWindow leaderboard_window(sf::VideoMode(WIDTH, HEIGHT), "Leaderboard");

    map<float, string> leaderboard;
    vector<float> times;
    vector<string> to_display;
    sf::String text;

    ifstream file("files/leaderboard.txt");
    string singleLine;


    // Read leaderboard.txt into a map
    while(getline(file, singleLine)) {

        string float1;
        string float2;
        string name;
        float time;

        istringstream stream(singleLine);

        getline(stream, float1, ':');
        getline(stream, float2, ',');
        getline(stream, name, ',');


        // remove the * from previous winner
        if (name.find('*') != std::string::npos)
            name.pop_back();

        // store time in string
        string value = float1 + float2;

        // convert value to time
        time = stof(value);


        // store times in vector
        times.push_back(time);

        // store in map
        leaderboard.emplace(time, name);


    }

    // if user wins, add to list
    if(won){

        string min = to_string(minutes);
        string sec = to_string(seconds);
        string value;

        if(seconds < 10)
            value = min + "0" + sec;
        else
            value = min + sec;

        float time = stof(value);

        string name = user_name + "*";

        times.push_back(time);
        leaderboard.emplace(time, name);
    }


    int place = 1;
    // Map is sorted automatically, write into leaderboard.txt
    ofstream write("files/leaderboard.txt");

    for(auto iter = leaderboard.begin(); iter != leaderboard.end(); ++iter){

        float time = iter->first;

        string name = iter->second;
        string value;


        if(time > 1000){
            value = to_string(time);
            value.erase(4, value.length());
            value.insert(2, ":");
        }
        else if(time < 1000 and time > 100)
        {
            value = "0" + to_string(time);
            value.erase(4, value.length());
            value.insert(2, ":");
        }
        else if(time < 100 and time >= 10)
        {
            value = "00" + to_string(time);
            value.erase(4, value.length());
            value.insert(2, ":");
        }
        else if(time < 10 and time > 0)
        {
            value = "000" + to_string(time);
            value.erase(4, value.length());
            value.insert(2, ":");
        }
        else{
            value = "0" + to_string(time);
            value.erase(6, value.length());
            value.erase(2, 2);
            value.insert(2, ":");

        }

        string pos = to_string(place);

        to_display.emplace_back(pos + ".\t" + value + "\t" + name + "\n\n");
        write << value << "," << name << endl;

        place++;
    }

    for(int i = 0; i < 5; ++i)
    {
        text += to_display[i];
    }

    // SCREEN BEGINS
    // *** Welcome Text ***
    sf::Text leaderboard_text;
    sf::Font leaderboard_font;

    // load font
    leaderboard_font.loadFromFile("files/font.ttf");

    // set welcome message
    leaderboard_text.setFont(leaderboard_font);
    leaderboard_text.setCharacterSize(20);
    leaderboard_text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    leaderboard_text.setString("LEADERBOARD");

    // center Text
    sf::FloatRect leaderboard_textRect = leaderboard_text.getLocalBounds();
    leaderboard_text.setOrigin(leaderboard_textRect.left + leaderboard_textRect.width/2.0f,
                               leaderboard_textRect.top + leaderboard_textRect.height/2.0f);
    leaderboard_text.setPosition(sf::Vector2f(WIDTH/2.0f, HEIGHT/2.0f - 120));


    // DISPLAY ACTUAL STATS
    sf::Text LB_text;

    LB_text.setFont(leaderboard_font);
    LB_text.setCharacterSize(18);
    LB_text.setString(text);

    // center Text
    sf::FloatRect LB_textRect = LB_text.getLocalBounds();
    LB_text.setOrigin(LB_textRect.left + LB_textRect.width/2.0f,
                               LB_textRect.top + LB_textRect.height/2.0f);
    LB_text.setPosition(WIDTH/2.0f, HEIGHT/2.0f + 20);


    while(leaderboard_window.isOpen()) {


        sf::Event event;

        while (leaderboard_window.pollEvent(event)) {
            // Check type of event
            switch (event.type) {
                case sf::Event::Closed:
                    leaderboard_window.close();
                    break;
            }

        }

        leaderboard_window.clear(sf::Color::Blue);
        leaderboard_window.draw(leaderboard_text);
        leaderboard_window.draw(LB_text);

        leaderboard_window.display();


    }

    return false;
}




// ***** HELPER METHODS *****

// returns a map containing all sprites
map<string, sf::Sprite> getSprites(int &rows, int &cols)
{
    map<string, sf::Sprite> map_sprites;

    // initialize all sprites to be used throughout the game
    map_sprites.emplace("mine", createSprite("files/images/mine.png"));
    map_sprites.emplace("tile_hidden", createSprite("files/images/tile_hidden.png"));
    map_sprites.emplace("tile_revealed", createSprite("files/images/tile_revealed.png"));
    map_sprites.emplace("num_1", createSprite("files/images/number_1.png"));
    map_sprites.emplace("num_2", createSprite("files/images/number_2.png"));
    map_sprites.emplace("num_3", createSprite("files/images/number_3.png"));
    map_sprites.emplace("num_4", createSprite("files/images/number_4.png"));
    map_sprites.emplace("num_5", createSprite("files/images/number_5.png"));
    map_sprites.emplace("num_6", createSprite("files/images/number_6.png"));
    map_sprites.emplace("num_7", createSprite("files/images/number_7.png"));
    map_sprites.emplace("num_8", createSprite("files/images/number_8.png"));
    map_sprites.emplace("flag", createSprite("files/images/flag.png"));
    map_sprites.emplace("face_happy", createSprite("files/images/face_happy.png"));
    map_sprites.emplace("face_win", createSprite("files/images/face_win.png"));
    map_sprites.emplace("face_lose", createSprite("files/images/face_lose.png"));
    map_sprites.emplace("digits", createSprite("files/images/digits.png"));
    map_sprites.emplace("debug", createSprite("files/images/debug.png"));
    map_sprites.emplace("pause", createSprite("files/images/pause.png"));
    map_sprites.emplace("play", createSprite("files/images/play.png"));
    map_sprites.emplace("leaderboard", createSprite("files/images/leaderboard.png"));
    map_sprites.emplace("digits", createSprite("files/images/digits.png"));

    map_sprites["face_happy"].setPosition(((cols / 2.0) * 32) - 32, 32 *(rows + 0.5f));
    map_sprites["face_win"].setPosition(((cols / 2.0) * 32) - 32, 32 *(rows + 0.5f));
    map_sprites["face_lose"].setPosition(((cols / 2.0) * 32) - 32, 32 *(rows + 0.5f));
    map_sprites["debug"].setPosition((cols * 32) - 304, 32 * (rows + 0.5f));
    map_sprites["pause"].setPosition((cols * 32) - 240, 32 * (rows + 0.5f));
    map_sprites["play"].setPosition((cols * 32) - 240, 32 * (rows + 0.5f));
    map_sprites["leaderboard"].setPosition((cols * 32) - 176, 32 * (rows + 0.5f));

    return map_sprites;
}

// creates a new sprite given a filename
sf::Sprite createSprite(string filename)
{
    auto *texture = new sf::Texture;
    texture->loadFromFile(filename);

    sf::Sprite sprite;
    sprite.setTexture(*texture);

    return sprite;

}