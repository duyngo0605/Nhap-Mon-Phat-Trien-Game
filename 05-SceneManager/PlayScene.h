#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
//#include "Koopas.h"

#define MAP_HEIGHT 432.0f
#define MAP_WIDTH 2816.0f

#define ID_SCENE_PLAY		1
#define ID_SCENE_WORLDMAP	2
#define ID_SCENE_INTRO		3


class CPlayScene: public CScene
{
protected: 
	// A play scene has to have player, right? 

	int numofBackgroundTile = 0;
	LPGAMEOBJECT player;

	vector<LPGAMEOBJECT> objects;

	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);

	void LoadAssets(LPCWSTR assetFile);
	
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	LPGAMEOBJECT GetPlayer() { return player; }

	void AddObject(CGameObject* object);
	size_t GetObjectsQuanity() { return objects.size(); }
	LPGAMEOBJECT GetObject(long i) { return objects[i]; }
	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

typedef CPlayScene* LPPLAYSCENE;

