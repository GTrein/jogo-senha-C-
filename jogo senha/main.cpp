
#include <iostream>
#include <raylib.h>
#include <cmath>
#include <ctime>
#include "game.h"
using namespace std;

const int screen_width = 420;
const int screen_height = 700;



int main() {
	Game game;

	srand(time(0));
	SetTargetFPS(60);

	InitWindow(screen_width, screen_height, "SENHA - GABRIEL TREIN");
	game.Run();





	CloseWindow();
	return 0;
}


