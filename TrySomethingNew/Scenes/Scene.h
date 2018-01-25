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
		ImageData* IntroScreen2;
		ImageData* IntroScreen3;
		ImageData* SettingBlurb;
		ImageData* EnterShopName;
		ImageData* ShopIsSetUp;
		ImageData* PressReturn;
	} TextObjects;
	
	struct {
		TextBox* ShopNameEntry;
	} TextBoxObjects;
	
	struct {
		EventTimer* IntroScreen1_Date;
		EventTimer* IntroScreen1_Location;
		EventTimer* IntroScreen2;
		EventTimer* IntroScreen3;
	} EventTimers;
	
	struct {
		bool ExitToTitleScreen;
		bool IntroScreen2;
		bool IntroScreen3;
		bool EditName;
		bool ShopNamed;
	} EventFlags;

public:
	// Scene ctor
	Intro();

	// Scene funcs
	void ResetFlags();
	void LoadEventTimers();
	void LoadImagesText();
	void SceneStart();
	void HandleEvent(SDL_Event* Event);
	void Update(Uint32 timeStep);
	void Render();

	// Scene Events
	void SEvent_IntroScreen1_ShowLocation();
	void SEvent_IntroScreen2_Show();
	void SEvent_IntroScreen2_Skip();
	void SEvent_IntroScreen3_Show();
	void SEvent_IntroScreen3_Skip();
	void SEvent_NameEntryScreen_Show();
	void SEvent_ShopNamed();
	void SEvent_ToMarket();
};

class Market : public Scene {
protected:
	// Main market screen text and text boxes
	std::vector<ImageData*> MarketText;
	std::vector<ImageData*> SubTotalText;
	ImageData*	ActiveBuySubTotal;
	std::vector<TextBox*> ItemTextBoxObjects;
	TextBox*	ActiveBuySelection;
	std::vector<ItemData*>	BuyData;
	ItemData*	ActiveItemData;
	
	int			BuyTotal;

	struct {
		// Total
		ImageData* TotalAmount;
		ImageData* PlayerMoneyAmount;
		ImageData* MoneySubTotalAmount;
		// Selection
		ImageData* SelectItem;
		ImageData* EnterQty;
		// Game Saved
		ImageData* GameSaved;
		// Errors
		ImageData* ErrBuyItem;
		ImageData* ErrNoMoney;
		// Press Return for Forecast/Guide
		ImageData* PressReturn;
		// Forecast
		ImageData* WeatherHeader;
		ImageData* WeatherInfo;
		ImageData* EventHeader;
		ImageData* EventInfo;
		// Guide
		ImageData* GuideText;
	} TextObjects;

	struct {
		EventTimer* GameSaved;
		EventTimer* ErrorText;
	} EventTimers;

	struct {
		bool ExitToTitleScreen;
		bool MainSelection;
		bool SelectBuyItem;
		bool EnterItemQty;
		bool ShowForecast;
		bool SelectGuideItem;
		bool ShowGuide;
	} EventFlags;

public:
	// Scene ctor
	Market();

	// Scene funcs
	void ResetFlags();
	void LoadEventTimers();
	void LoadImagesText();
	void SceneStart();
	void HandleEvent(SDL_Event* Event);
	void Update(Uint32 timeStep);
	void Render();

	// Market funcs
	ImageData* AddMarketText(std::string _text, int _x, int _y);
	ImageData* AddSubTotalText(std::string _text, int _x, int _y);
	TextBox* AddMarketTextBox(Uint32 _size, int _x, int _y);

	//// Events
	// Show/Hide Events
	void SEvent_ShowMarketText();
	void SEvent_HideMarketText();
	void SEvent_ShowForecast();
	void SEvent_HideForecast();
	void SEvent_ShowGuide();
	void SEvent_HideGuide();
	// Buy
	void SEvent_SelectBuy();
	void SEvent_SetBuyItem(SDL_Keycode _key);
	void SEvent_EndItemQtyEntry();
	// Forecast
	void SEvent_SelectForecast();
	void SEvent_ExitForecast();
	// Guide
	void SEvent_SelectGuide();
	void SEvent_SetGuideItem(SDL_Keycode _key);
	void SEvent_ExitGuide();
	// Leave
	void SEvent_Leave();
	// Save
	void SEvent_Save();
	void SEvent_HideSaveText();
	// Error
	void SEvent_HideErrorText();
	
	void UpdateTotal();
};

//// SetPrices scene class
class SetPrices : public Scene {
protected:
	// Set Prices screen text and text boxes
	std::vector<ImageData*> SetPricesText;
	std::vector<TextBox*> ItemTextBoxObjects;
	std::vector<ItemData*> SellItems;

	TextBox*	ActiveSellSelection;
	ItemData*	ActiveItemData;

	struct {
		// Selection
		ImageData* SelectItem;
		ImageData* EnterPrice;
		// Error
		ImageData* ErrSetPrice;
		// Press Return for Forecast/Guide
		ImageData* PressReturn;
		// Forecast
		ImageData* WeatherHeader;
		ImageData* WeatherInfo;
		ImageData* EventHeader;
		ImageData* EventInfo;
		// Guide
		ImageData* GuideText;
	} TextObjects;

	struct {
		EventTimer* ErrorText;
	} EventTimers;

	struct {
		bool ExitToTitleScreen;
		bool MainSelection;
		bool SelectSellItem;
		bool EnterItemPrice;
		bool ShowForecast;
		bool SelectGuideItem;
		bool ShowGuide;
	} EventFlags;

public:
	// Scene ctor
	SetPrices();

	// Scene funcs
	void ResetFlags();
	void LoadEventTimers();
	void LoadImagesText();
	void SceneStart();
	void HandleEvent(SDL_Event* Event);
	void Update(Uint32 timeStep);
	void Render();

	// SetPrices funcs
	ImageData* AddSetPricesText(std::string _text, int _x, int _y);
	TextBox* AddSetPricesItemBox(Uint32 _size, int _x, int _y);
	void GetCurrentPlayerInventory();
	
	//// Events
	// Show/Hide Events
	void SEvent_ShowSetPricesText();
	void SEvent_HideSetPricesText();
	void SEvent_ShowForecast();
	void SEvent_HideForecast();
	void SEvent_ShowGuide();
	void SEvent_HideGuide();
	// Set Price
	void SEvent_SelectSell();
	void SEvent_SetSellItem(SDL_Keycode _key);
	void SEvent_EndSellEntry();
	// Forecast
	void SEvent_SelectForecast();
	void SEvent_ExitForecast();
	// Guide
	void SEvent_SelectGuide();
	void SEvent_SetGuideItem(SDL_Keycode _key);
	void SEvent_ExitGuide();
	// Leave
	void SEvent_OpenShop();
	// Error
	void SEvent_HideErrorText();
};

class DaySales : public Scene {
protected:
	std::vector<ImageData*> DaySalesText;
	std::vector<ItemData*> SellItems;
	
	std::vector<ImageData*> CustomerImages;
	std::vector<Customer*> Customers;
	std::vector<CustomerObject*> CustomerObjects;
	Uint32 CustomerSpawnInterval;
	Uint32 CustomerSpawnTotal;
	
	const Uint32 DaySegmentLength = 5000;
	int Money;

	struct {
		ImageData* Shack;
		ImageData* Shack1;
		ImageData* Shack2;
		ImageData* Shack3;
	} Images;

	struct {
		ImageData* ShopName;
		ImageData* MoneyText;
		ImageData* MoneyAmt;
		ImageData* DayText;
		ImageData* DayNum;
	} TextObjects;

	struct {

	} TextBoxObjects;

	struct {
		EventTimer* CustomerSpawn;
		EventTimer* DayRuntime1;
		EventTimer* DayRuntime2;
		EventTimer* DayRuntime3;
	} EventTimers;

	struct {
		bool ExitToTitleScreen;
		bool Simulation;
		bool DayEnd;
	} EventFlags;

public:
	// Scene ctor
	DaySales();

	// Scene funcs
	void ResetFlags();
	void LoadGameObjects();
	void LoadEventTimers();
	void LoadImagesText();
	void SceneStart();
	void HandleEvent(SDL_Event* Event);
	void Update(Uint32 timeStep);
	void Render();

	// DaySales funcs
	ImageData* AddDaySalesText(std::string _text, int _x, int _y);
	void GetCurrentPlayerInventory();
	void GenerateCustomers();
	void GetPurchase(Customer* _customer);

	//// Events
	// Show/Hide Events
	void SEvent_ShowDaySalesText();

	void SEvent_SpawnCustomer();
	void SEvent_DayRuntime2();
	void SEvent_DayRuntime3();
	void SEvent_DayRuntimeEnd();
};

class SalesResults : public Scene {
protected:
	// Sales Results screen text and text boxes
	std::vector<ImageData*> SalesResultsText;
	std::vector<ItemData*> SellItems;

	struct {
		ImageData* PlayerMoneyAmount;
	} TextObjects;

	struct {
		bool ExitToTitleScreen;
		bool ExitResults;
		bool GameOver;
	} EventFlags;

public:
	// Scene ctor
	SalesResults();

	// Scene funcs
	void ResetFlags();
	void LoadImagesText();
	void SceneStart();
	void HandleEvent(SDL_Event* Event);
	void Update(Uint32 timeStep);
	void Render();

	// SalesResults funcs
	ImageData* AddSalesResultsText(std::string _text, int _x, int _y);
	void GetCurrentPlayerInventory();

	//// Events
	// Show/Hide Events
	void SEvent_ShowSalesResultsText();
	void SEvent_HideSalesResultsText();
	void SEvent_LoseCheck();
	void SEvent_GameOver_TitleScreen();
	void SEvent_NextDay();
};

#endif