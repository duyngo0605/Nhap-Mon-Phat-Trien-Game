#pragma once

#define TEXTURE_PATH_BRICK L"poolballred.png"
#define BRICK_START_X 8.0f
#define BRICK_START_Y 200.0f

#define BRICK_START_VX 0.2f
#define BRICK_START_VY 0.2f

#define BRICK_WIDTH 32.0f
#define BRICK_HEIGHT 32.0f

class Ball
{

	float brick_x = BRICK_START_X;
	float brick_vx = BRICK_START_VX;
	float brick_y = BRICK_START_Y;
	float brick_vy = BRICK_START_VY;
public:
	Ball(){}

};

