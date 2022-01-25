#include "math.h"
#include "iostream"
#include "preprocess.hpp"
#include "Player.hpp"
#include "settings.hpp"
#include "Game.hpp"

using namespace std;
using namespace cv;

bool hasBeenStopped = false;
bool hasStoppedFrameBeenSaved = false;

int main(int, char **)
{
    Game game(DIFFICULTY);
    game.gameStart();
}
