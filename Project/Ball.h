#pragma once
#include<random>

#define TEXTURE_PATH_BRICK L"poolballred.png"
#define BRICK_START_X 8.0f
#define BRICK_START_Y 200.0f

#define BRICK_START_VX 0.2f
#define BRICK_START_VY 0.4f

#define BRICK_WIDTH 20.0f
#define BRICK_HEIGHT 20.0f

class Ball
{
public:
	float width, height, x, y, vx, vy;
	Ball(){
		width = BRICK_WIDTH;
		height = BRICK_HEIGHT;
		x = (float)(rand() % 650);
		y = (float(rand() % 450));
		vx = BRICK_START_VX;
		vy = BRICK_START_VY;
	}

};

