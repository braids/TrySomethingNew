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

	// Scene virtual funcs
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
	std::vector<ImageData*> MainMenuText;
	std::vector<ImageData*> CreditsImagesText;
	
	struct {
		ImageData* JamLogo;
		ImageData* ThemeLogo;
	} Images;
	
	struct {
		bool Credits;
	} EventFlags;

public:
	// Scene ctor
	MainMenu();

	// Scene funcs
	void ResetFlags();
	void LoadImagesText();
	void SceneStart();
	void HandleEvent(SDL_Event* Event);
	void Update(Uint32 timeStep);
	void Render();

	void SEvent_ShowCredits();
	void SEvent_ShowMainMenu();

	// Scene Events
	void SEvent_ExitToTitle();
	void SEvent_NewGame();
	void SEvent_LoadGame();
	void SEvent_Quit();
	void SEvent_Windowed();
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
	
	TextBox*	ActiveBuySelection;
	ItemData*	ActiveItemData;
	ImageData*	ActiveBuySubTotal;
	
	std::vector<ItemData*>	BuyData;
	int						BuyTotal;

	ImageData*	ActiveForecastWeatherText;
	ImageData*	ActiveForecastEventText;
	ImageData*	ActiveGuideText;

	struct {
		int Bier = 4;
		int Bockwurst = 2;
		int Mettigel = 4;
		int Currywurst = 6;
		int StreetSheet = 3;
		int USADAY = 7;
		int Sign = 5;
		int Poster = 10;
		int NewsAd = 15;
	} Prices;

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
		// Purchse symbols
		ImageData* MultSigns;
		ImageData* EqualSigns;
		// Items
		ImageData* BierName;
		ImageData* BierCost;
		ImageData* BierTotal;
		ImageData* BockwurstName;
		ImageData* BockwurstCost;
		ImageData* BockwurstTotal;
		ImageData* MettigelName;
		ImageData* MettigelCost;
		ImageData* MettigelTotal;
		ImageData* CurrywurstName;
		ImageData* CurrywurstCost;
		ImageData* CurrywurstTotal;
		ImageData* StreetSheetName;
		ImageData* StreetSheetCost;
		ImageData* StreetSheetTotal;
		ImageData* USADAYName;
		ImageData* USADAYCost;
		ImageData* USADAYTotal;
		// Ads
		ImageData* SignName;
		ImageData* SignCost;
		ImageData* SignTotal;
		ImageData* PosterName;
		ImageData* PosterCost;
		ImageData* PosterTotal;
		ImageData* NewsAdName;
		ImageData* NewsAdCost;
		ImageData* NewsAdTotal;
		// Total
		ImageData* TotalText;
		ImageData* TotalAmount;
		ImageData* PlayerMoneyText;
		ImageData* PlayerMoneyAmount;
		ImageData* MoneySubTotalText;
		ImageData* MoneySubTotalAmount;
		// Selection
		ImageData* SelectItem;
		ImageData* EnterQty;
		// Options
		ImageData* BuyOption;
		ImageData* ForecastOption;
		ImageData* GuideOption;
		ImageData* LeaveOption;
		ImageData* SaveOption;
		// Game Saved
		ImageData* GameSaved;
		// Press Return for Forecast/Guide
		ImageData* PressReturn;
		// Forecast
		ImageData* WeatherHeader;
		ImageData* WeatherInfo;
		ImageData* EventHeader;
		ImageData* EventInfo;
		// Guide
		ImageData* BierDesc;
		ImageData* BockwurstDesc;
		ImageData* MettigelDesc;
		ImageData* CurrywurstDesc;
		ImageData* StreetSheetDesc;
		ImageData* USADAYDesc;
		ImageData* SignDesc;
		ImageData* PosterDesc;
		ImageData* NewsAdDesc;
	} TextObjects;

	struct {
		//TextBox* Selection;
		TextBox* BierQty;
		TextBox* BockwurstQty;
		TextBox* MettigelQty;
		TextBox* CurrywurstQty;
		TextBox* StreetSheetQty;
		TextBox* USADAYQty;
		TextBox* SignQty;
		TextBox* PosterQty;
		TextBox* NewsAdQty;
	} TextBoxObjects;

	struct {
		EventTimer* GameSaved;
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
	TextBox* AddMarketTextBox(Uint32 _size, int _x, int _y) {
		TextBox* textBox = this->AddTextBox(_size, _x, _y);
		this->MarketText.push_back(textBox);
		return textBox;
	}

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

	ImageData*	ActiveForecastWeatherText;
	ImageData*	ActiveForecastEventText;

	struct {
		// Selection
		ImageData* SelectItem;
		ImageData* EnterPrice;
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
	void LoadImagesText();
	void SceneStart();
	void HandleEvent(SDL_Event* Event);
	void Update(Uint32 timeStep);
	void Render();

	// SetPrices funcs
	ImageData* AddSetPricesText(std::string _text, int _x, int _y);
	TextBox* AddSetPricesItemBox(Uint32 _size, int _x, int _y);
	void GetCurrentPlayerInventory();
	int KeycodeNumValue(SDL_Keycode _key);

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