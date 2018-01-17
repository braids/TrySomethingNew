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
class MainMenu;

//// Scene name enum
enum SceneName {
	Scene_TitleScreen,	// Title Screen
	Scene_MainMenu,		// Main menu
	Scene_Intro			// Intro
};

//// Scene class
class Scene {
protected:
	SceneName		mSceneName; // Current scene name (pulled from enum)
	bool			mActive;	// Scene is active or inactive
	SceneManager*	mManager;	// Parent scene manager pointer
	Camera			mCamera;
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
	ImageData* AddText(std::string _text, int _x, int _y);
	TextBox* AddTextBox(size_t _size, int _x, int _y);
	EventTimer* AddEventTimer(EventTimer* _eventTimer);
	void UpdateEventTimers();

	// Scene virtual funcs
	virtual void LoadGameObjects() = 0;
	virtual void SceneStart() = 0;
	virtual void HandleEvent(SDL_Event* Event) = 0;
	virtual void Update(Uint32 timeStep) = 0;
	virtual void Render() = 0;
};

//// Title screen scene class
class TitleScreen : public Scene {
protected:
	bool StartMainMenu;
public:
	// Scene ctor
	TitleScreen();

	// Scene funcs
	void LoadGameObjects();
	void SceneStart();
	void HandleEvent(SDL_Event* Event);
	void Update(Uint32 timeStep);
	void Render();
};

//// MainMenu scene class
class MainMenu : public Scene {
protected:
	bool ExitToTitleScreen;
	bool StartNewGame;
public:
	// Scene ctor
	MainMenu();

	// Scene funcs
	void LoadGameObjects();
	void SceneStart();
	void HandleEvent(SDL_Event* Event);
	void Update(Uint32 timeStep);
	void Render();
};

//// Intro scene class
class Intro : public Scene {
protected:
	bool ExitToTitleScreen;
	bool EditName;
	std::string* ShopName;
	bool ShopNamed;
	struct {
		ImageData* Wall1;
		ImageData* Wall2;
	} Images;
	struct {
		ImageData* OpeningDate;
		ImageData* OpeningLocation;
		ImageData* IntroText1;
		ImageData* IntroText2;
		ImageData* SettingBlurb;
		ImageData* EnterShopName;
		ImageData* ShopIsSetUp;
	} TextObjects;
	struct {
		TextBox* ShopNameEntry;
	} TextBoxObjects;
	struct {
		EventTimer* IntroDate1;
		EventTimer* IntroDate2;
		EventTimer* IntroText1;
		EventTimer* IntroText2;
	} EventTimers;
	struct {
		bool OpeningText1;
		bool OpeningText2;
		bool OpeningText3;
		bool OpeningText4;
		bool EnterShopName;
	} EventFlags;
public:
	// Scene ctor
	Intro();

	// Scene funcs
	void LoadGameObjects();
	void SceneStart();
	void HandleEvent(SDL_Event* Event);
	void Update(Uint32 timeStep);
	void Render();
};

#endif