#include <algorithm>
#include <gameux.h>

#define ID_HUD	999999
#define ID_NUMBER_0	0
#define ID_NUMBER_1	1
#define ID_NUMBER_2	2
#define ID_NUMBER_3	3
#define ID_NUMBER_4	4
#define ID_NUMBER_5	5
#define ID_NUMBER_6	6
#define ID_NUMBER_7	7
#define ID_NUMBER_8	8
#define ID_NUMBER_9	9

#define ID_RUNLEVEL_1	10
#define ID_RUNLEVEL_2	11	//can fly

#define HUD_WIDTH	270.0f
#define HUD_HEIGHT 32.0f


#define ID_DIALOG_START	1100
#define ID_DIALOG_END		1101

#define DIALOG_X	SCREEN_WIDTH/2
#define DIALOG_Y	80.0f

#define DIALOG_TIMEOUT	1500

class CHud
{
	static CHud* __hudInstance;
	float x, y;
	ULONGLONG spawn_start;
	
public:
	bool isRenderStartDialog = true;
	CHud(float x, float y)
	{
		spawn_start = GetTickCount64();
		this->x = x;
		this->y = y;
	}
	static CHud* GetInstance();

	void Update();
	void Render();
	void RenderCoin();
	void RenderHP();
	void RenderScore();
	void RenderTimer();
	void RenderSpeedBar();
	void RenderCard();
	void RenderStartHud();
	void RenderGameOverHud();
};

