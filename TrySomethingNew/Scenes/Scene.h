#pragma once
#ifndef _SCENE_H_
#define _SCENE_H_

#include <vector>
#include "Assets.h"
#include "Camera.h"
#include "GameObjects\GameObject.h"
#include "Timer.h"

// Forward dec the SceneManager
class SceneManager;

// Class forward decs
class Scene;
class TitleScreen;
class Level1;

//// Scene name enum
enum SceneName {
	Scene_TitleScreen,	// Title Screen
	Scene_Level1		// Level 1
};

//// Scene class
class Scene {
protected:
	SceneName		mSceneName; // Current scene name (pulled from enum)
	bool			mActive;	// Scene is active or inactive
	SceneManager*	mManager;	// Parent scene manager pointer
	Assets*			mAssets;	// Scene assets pointer
	Camera			mCamera;
	std::vector<GameObject*> mGameObjects;

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

	// Scene virtual funcs
	virtual void LoadAssets() = 0;
	virtual void LoadGameObjects() = 0;
	virtual void SceneStart() = 0;
	virtual void HandleEvent(SDL_Event* Event) = 0;
	virtual void Update(Uint32 timeStep) = 0;
	virtual void Render() = 0;
};

//// Title screen scene class
class TitleScreen : public Scene {
protected:
	bool StartLevel1;
public:
	// Scene ctor
	TitleScreen();

	// Scene funcs
	void LoadAssets();
	void LoadGameObjects();
	void SceneStart();
	void HandleEvent(SDL_Event* Event);
	void Update(Uint32 timeStep);
	void Render();
};

//// Level 1 scene class
class Level1 : public Scene {
protected:
	bool ExitToMainMenu;

public:
	// Scene ctor
	Level1();

	// Scene funcs
	void LoadAssets();
	void LoadGameObjects();
	void SceneStart();
	void HandleEvent(SDL_Event* Event);
	void Update(Uint32 timeStep);
	void Render();
};

#endif