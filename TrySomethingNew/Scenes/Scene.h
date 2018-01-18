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
class Intro;
class Market;

//// Scene name enum
enum SceneName {
	Scene_TitleScreen,	// Title Screen
	Scene_MainMenu,		// Main menu
	Scene_Intro,		// Intro
	Scene_Market		// Market
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
	bool LoadGame;

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
		EventTimer* ToMarket;
	} EventTimers;
	
	struct {
		bool ExitToTitleScreen;
		bool EditName;
		bool ShopNamed;
	} EventFlags;

public:
	// Scene ctor
	Intro();

	// Scene funcs
	void ResetFlags();
	void LoadGameObjects();
	void LoadEventTimers();
	void LoadImagesText();
	void SceneStart();
	void HandleEvent(SDL_Event* Event);
	void Update(Uint32 timeStep);
	void Render();

	// Scene Events
	void SEvent_1();
	void SEvent_2();
	void SEvent_3();
	void SEvent_4();
	void SEvent_ToMarket();
};

class Market : public Scene {
protected:
	std::vector<ImageData*> MarketText;

	struct {
		// Title
		ImageData* MarketTitle;
		// Headers
		ImageData* Foods;
		ImageData* Newspapers;
		ImageData* Ads;
		ImageData* Cost;
		ImageData* Qty;
		ImageData* Total;
		// Items
		ImageData* BierName;
		ImageData* BierCost;
		ImageData* BockwurstName;
		ImageData* BockwurstCost;
		ImageData* MettigelName;
		ImageData* MettigelCost;
		ImageData* CurrywurstName;
		ImageData* CurrywurstCost;
		ImageData* StreetSheetName;
		ImageData* StreetSheetCost;
		ImageData* USADAYName;
		ImageData* USADAYCost;
		// Ads
		ImageData* SignName;
		ImageData* SignCost;
		ImageData* PosterName;
		ImageData* PosterCost;
		ImageData* NewsAdName;
		ImageData* NewsAdCost;
		// Options
		ImageData* BuyOption;
		ImageData* ForecastOption;
		ImageData* GuideOption;
		ImageData* LeaveOption;
		ImageData* SaveOption;
		// Forecast
		ImageData* WeatherHeader;
		ImageData* WeatherInfo;
		ImageData* EventHeader;
		ImageData* EventInfo;
	} TextObjects;

	struct {
		bool ExitToTitleScreen;
	} EventFlags;

public:
	// Scene ctor
	Market();

	// Scene funcs
	void ResetFlags();
	void LoadGameObjects();
	void LoadEventTimers();
	void LoadImagesText();
	void SceneStart();
	void HandleEvent(SDL_Event* Event);
	void Update(Uint32 timeStep);
	void Render();

	// Market funcs
	ImageData* AddMarketText(std::string _text, int _x, int _y) { 
		ImageData* textImage = this->AddText(_text, _x, _y);
		this->MarketText.push_back(textImage);
		return textImage; 
	}

	// Market Events
	void SEvent_ShowMarketText();
	void SEvent_HideMarketText();
};

#endif