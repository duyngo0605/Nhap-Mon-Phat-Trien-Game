#include <iostream>
#include <fstream>
#include "AssetIDs.h"

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Coin.h"
#include "Platform.h"
#include "SpecialPlatform.h"
#include "BackgroundTile.h"
#include "Pipe.h"
#include "QuestionBrick.h"
#include "FireVenusTrap.h"
#include "FireBall.h"
#include "Koopa.h"
#include "SpecialBrick.h"
#include "BlockKoopa.h"
#include "Card.h"
#include "LandScape.h"
#include "Path.h"
#include "Node.h"
#include "Arrow.h"
#include "Hud.h"
#include "Data.h"
#include "WorldMapObject.h"

#include "SampleKeyEventHandler.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	cx = cy = 0.0f;
	player = NULL;
	key_handler = new CSampleKeyHandler(this);

}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);
	
	LoadAssets(path.c_str());
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	CGameObject *obj = NULL;

	switch (object_type)
	{


	case OBJECT_TYPE_MARIO:
		if (player!=NULL) 
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		if (CData::GetInstance()->HP == 0)
		{
			x = DIALOG_X - 14.0f;
			y = DIALOG_Y + 10.0f;
			obj= new CArrow(x,y);
			player = (CArrow*)obj;
			break;
		}
		obj = new CMario(x, y);
		player = (CMario*)obj;
		if (this->id == ID_SCENE_PLAY)
		{		
			player->SetState(MARIO_STATE_IDLE);
		}
		else if (this->id == ID_SCENE_WORLDMAP)
		{
			x = CData::GetInstance()->xMarioWorld;
			y = CData::GetInstance()->yMarioWorld;
			player->SetState(MARIO_STATE_WORLDMAP_IDLE);
		}
		

		DebugOut(L"[INFO] Player object has been created!\n");
		break;
	case OBJECT_TYPE_GOOMBA: {
		int type = (int)atof(tokens[3].c_str());
		obj = new CGoomba(x, y, type); break; 
	}
	case OBJECT_TYPE_KOOPA:
	{
		int type = (int)atof(tokens[3].c_str());
		int level = (int)atof(tokens[4].c_str());
		obj = new CKoopa(x, y, type, level); break;
	}
	case OBJECT_TYPE_BRICK: obj = new CBrick(x,y); break;
	case OBJECT_TYPE_COIN: obj = new CCoin(x, y); break;
	case OBJECT_TYPE_FIREBALL: obj = new CFireBall(x, y); break;

	case OBJECT_TYPE_BACKGROUNDTILE:
	{
		numofBackgroundTile++;
		int sprite_id = atoi(tokens[3].c_str());
		obj = new CBackgroundTile(x, y, sprite_id);
		break;
	}


	case OBJECT_TYPE_PLATFORM:
	{

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		obj = new CPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		);

		break;
	}



	case OBJECT_TYPE_SPECIAL_PLATFORM:
	{

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		obj = new CSpecialPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		);

		break;
	}

	case OBJECT_TYPE_PIPE:
	{

		int sprite_id = atoi(tokens[3].c_str());
		if (tokens.size()>4)
		{
			int canDown = atoi(tokens[4].c_str());
			float xD = (float)atof(tokens[5].c_str());
			float yD = (float)atof(tokens[6].c_str());
			obj = new CPipe(x, y, sprite_id, canDown, xD, yD);
			break;
		}
		obj = new CPipe(x, y, sprite_id);
		break;
	}

	case OBJECT_TYPE_QUESTION_BRICK:
	{
		int type = atoi(tokens[3].c_str());
		obj = new CQuestionBrick(x, y, type);
		break;
	}

	case OBJECT_TYPE_FIREVENUSTRAP:
	{
		int type = atoi(tokens[3].c_str());
		obj = new CFireVenusTrap(x, y,type);
		break; 
	}
	case OBJECT_TYPE_SPECIAL_BRICK:
	{
		int type = atoi(tokens[3].c_str());
		obj = new CSpecialBrick(x, y, type);
		break;
	}

	case OBJECT_TYPE_BLOCKKOOPA:
	{
		obj = new CBlockKoopa(x, y);
		break;
	}
	case OBJECT_TYPE_CARD:
		obj = new CCard(x, y);
		break;
	case OBJECT_TYPE_LANDSCAPE:
	{
		numofBackgroundTile++;
		int sprite_id = atoi(tokens[3].c_str());
		obj = new CLandScape(x, y, sprite_id);
		break;
	}

	case OBJECT_TYPE_WORLDMAPOBJECT:
	{
		float vx= (float)atof(tokens[3].c_str());
		float vy= (float)atof(tokens[4].c_str());
		int aniId = atoi(tokens[5].c_str());
		obj = new CWorldMapObject(x, y, vx, vy, aniId);
		break;
	}

	case OBJECT_TYPE_PATH:
	{
		numofBackgroundTile++;
		int sprite_id = atoi(tokens[3].c_str());
		obj = new CPath(x, y, sprite_id);
		break;
	}

	case OBJECT_TYPE_NODE:
	{
		numofBackgroundTile++;
		int sprite_id = atoi(tokens[3].c_str());
		int l= atoi(tokens[4].c_str());
		int u= atoi(tokens[5].c_str());
		int r= atoi(tokens[6].c_str());
		int d= atoi(tokens[7].c_str());
		if (tokens.size() > 8)
		{
			int scene_id = atoi(tokens[8].c_str());
			obj = new CNode(x, y, sprite_id, l, u, r, d, scene_id);
		}
		else obj = new CNode(x, y, sprite_id, l, u, r, d);
		break;
	}

	case OBJECT_TYPE_ARROW:
	{
		obj = new CArrow(x, y);
		if (id == ID_SCENE_INTRO)
			player = (CArrow*)obj;
		break;
	}

	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;


	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);


	objects.push_back(obj);
}

void CPlayScene::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);

	int section = ASSETS_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; };
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	if (isPause) return;

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}
	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}
	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;


	UpdateCamera();
	CHud::GetInstance()->Update();

	PurgeDeletedObjects();

}

void CPlayScene::Render()
{
	

	for (int i = 0; i < objects.size(); i++)
	{
		if(!isPause||isPause&&dynamic_cast<CBackgroundTile*>(objects[i])|| dynamic_cast<CPlatform*>(objects[i]))
			objects[i]->Render();
	}
	if (isPause)
	{
		CSprites* sprites = CSprites::GetInstance();
		sprites->Get(ID_SPRITE_PAUSE)->Draw(cx+SCREEN_WIDTH/2,cy+SCREEN_HEIGHT/3);
	}
	if (id != ID_SCENE_INTRO)
		CHud::GetInstance()->Render();
	

}

void CPlayScene::AddObject(CGameObject* object)
{
	if (dynamic_cast<CBackgroundTile*>(object))
	{
		objects.insert(objects.begin() + numofBackgroundTile, object);
		numofBackgroundTile++;
	}
	else
		objects.insert(objects.begin() + numofBackgroundTile + 1, object);
}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well 

*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;
	numofBackgroundTile = 0;
	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());
}

void CPlayScene::UpdateCamera()
{
	
	// Update camera to follow mario


	///worldmap
	if (id == ID_SCENE_WORLDMAP)
	{
		cx = -7.0f;
		cy = -5.0f;
	}
	else if (id == ID_SCENE_PLAY)
	{
		//map 1-1
		player->GetPosition(cx, cy);
		CMario* mario = (CMario*)player;
		CGame* game = CGame::GetInstance();
		cx -= game->GetBackBufferWidth() / 2;
		if (cx < 0) cx = 0.0f;
		if (cx > MAP_WIDTH - game->GetBackBufferWidth()) cx = (float)(MAP_WIDTH - game->GetBackBufferWidth());
		if (cy <= 120)
		{
			cy = 0.0f;
		}
		else if (cy <= (MAP_HEIGHT-SCREEN_HEIGHT) || mario->GetIsFlying()&&cy<414.0f)
		{
			cy -= (float)game->GetBackBufferHeight() / 2;
		}
		else if (cy <= HEIGHT_DEATH || player->GetState() == MARIO_STATE_DIE)
		{
			cy = (float)(MAP_HEIGHT+HUD_HEIGHT+MARIO_BIG_BBOX_HEIGHT+14.0f-SCREEN_HEIGHT);
		}
		else ///hidenzone
		{
			cy = 640.0f;
		}
	}
	else
	{
		cx = cy = 0.0f;		
	}
	CGame::GetInstance()->SetCamPos(cx, cy);
}

