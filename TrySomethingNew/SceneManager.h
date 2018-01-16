#pragma once
#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

#include <vector>
#include "Assets.h"
#include "Timer.h"
#include "Scenes\Scene.h"

// Class forward decs
class SceneManager;

//// Scene manager class
class SceneManager{
protected:
	// Timer Vars
	Uint32			currTick;
	Uint32			lastTick;
	Uint32			timeStep;

	Graphics*			mGraphics;	// Scene renderer pointer
	Assets*				mAssets;
	Scene*				mCurrent;	// Current active scene
	Scene*				mNext;		// Next active scene
	std::vector<Scene*>	sceneList;	// Vector of all scenes

public:
	bool quitGame;

	// Scene manager ctor/dtor
	SceneManager();
	~SceneManager();

	// Scene manager methods
	void AddScene(Scene* scene) { this->sceneList.push_back(scene); }
	Scene* GetScene() { return this->mCurrent; }
	Graphics* GetGraphics() { return this->mGraphics; }
	Assets* GetAssets() { return this->mAssets; }
	bool IsScene(SceneName scene) { return this->mCurrent == this->sceneList[scene]; }
	void Update() { this->mCurrent->Update(this->timeStep); }

	// Scene manager funcs
	bool Init();
	void Event(SDL_Event* Event);
	void Render();
	void StartScene(SceneName scene);
	void UpdateTicks();
};

#endif
