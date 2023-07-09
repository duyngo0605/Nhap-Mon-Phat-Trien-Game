#include "Button.h"

void CButton::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (state == BUTTON_STATE_NOT_CLICKED)
	{
	
		animations->Get(ID_ANI_BUTTON_NOT_CLICKED)->Render(x, y);
	}
	else
	{
		animations->Get(ID_ANI_BUTTON_CLICKED)->Render(x, y);
	}
}
