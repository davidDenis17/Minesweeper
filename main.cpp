#include <iostream>
#include "Methods.h"
using namespace std;


int main() {

    string user_name;

    if(!WelcomeScreen(user_name))
    { return 0;}

    // ***** GAME STARTS *****
    Game_Screen(user_name);

    return 0;
}



