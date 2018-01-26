#pragma once
#ifndef _SCENE_H_
#define _SCENE_H_

#include <vector>
#include "Assets.h"
#include "Camera.h"
#include "Data\Customer.h"
#include "Data\ItemData.h"
#include "Data\PlayerData.h"
#include "GameObjects\GameObject.h"
#include "Timer.h"

// Forward dec the SceneManager
class SceneManager;

// Class forward decs
class Scene;
class TitleScreen;
class MainMenu;
class Intro;
class Market;
class SetPrices;
class DaySales;
class SalesResults;

//// Scene name enum
enum SceneName {
	Scene_TitleScreen,	// Title Screen
	Scene_MainMenu,		// Main menu
	Scene_Intro,		// Intro
	Scene_Market,		// Market
	Scene_SetPrices,	// Set Prices
	Scene_DaySales,		// Day Sales
	Scene_SalesResults	// Sales Results
};

//// Scene class
class Scene {
protected:
	SceneName		mSceneName; // Current scene name (pulled from enum)
	bool			mActive;	// Scene is active or inactive
	SceneManager*	mManager;	// Parent scene manager pointer
	Camera			mCamera;
	PlayerData*		mPlayerData;
	std::vector<ImageData*>		mImages;
	std::vector<GameObject*>	mGameObjects;
	std::vector<EventTimer*>	mEventTimers;

public:
	// Scene ctor/dtor
	Scene();
	~Scene();

	// Scene funcs
	void Init(SceneManager* manager);

	// Scene methods
	void SetActive(bool active) { this->mActive = active; }
	bool IsActive() const { return this->mActive; }
	void SetManager(SceneManager* manager) { this->mManager = manager; }
	void SetSceneName(SceneName name) { this->mSceneName = name; }
	void SetPlayerData(PlayerData* _player) { this->mPlayerData = _player; }
	ImageData* AddText(std::string _text, int _x, int _y);
	TextBox* AddTextBox(size_t _size, int _x, int _y);
	EventTimer* AddEventTimer(EventTimer* _eventTimer);
	void UpdateEventTimers();
	int KeycodeNumValue(SDL_Keycode _key);

	// Scene virtual funcs
	virtual void SceneStart() = 0;
	virtual void HandleEvent(SDL_Event* Event) = 0;
	virtual void Update(Uint32 timeStep) = 0;
	virtual void Render() = 0;
	virtual void Cleanup() = 0;
};

#endif