#include "SpecialBrick.h"
#include "Piece.h"
#include "Coin.h"
#include "PlayScene.h"

void CSpecialBrick::Render()
{
	if (!isEmpty)
	{
		CAnimations* animations = CAnimations::GetInstance();
		animations->Get(ID_ANI_SPECIAL_BRICK)->Render(x, y);
	}
	else
	{
		CSprites* s = CSprites::GetInstance();
		s->Get(ID_EMPTY_QUESTION_BRICK_SPRITE)->Draw(x, y);
	}
	RenderBoundingBox();
}

void CSpecialBrick::Break() ///break into pieces
{
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	int count = 0, i = 1, j = 1;
	while (count < NUM_OF_PIECES)
	{
		CPiece* piece = new CPiece(x, y + j * PIECE_BBOX_WIDTH);
		piece->SetSpeed(PIECE_SPEED_X * i, 0);
		scene->AddObject(piece);
		j *= -1;
		if(count%2==0) i *= -1;
		count++;
	}
}
