#pragma once
#ifndef _SCENE_H_
#define _SCENE_H_

#include <vector>
#include "Assets.h"
#include "Camera.h"
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

//// Scene name enum
enum SceneName {
	Scene_TitleScreen,	// Title Screen
	Scene_MainMenu,		// Main menu
	Scene_Intro,		// Intro
	Scene_Market,		// Market
	Scene_SetPrices		// Set Prices
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
	
	void UpdateTotal();
};

//// SetPrices scene class
class SetPrices : public Scene {
protected:
	// Main market screen text and text boxes
	std::vector<ImageData*> SetPricesText;
	std::vector<TextBox*> ItemTextBoxObjects;
	std::vector<ItemData*> SellItems;

	TextBox*	ActiveSellSelection;
	ItemData*	ActiveItemData;

	ImageData*	ActiveForecastWeatherText;
	ImageData*	ActiveForecastEventText;
	ImageData*	ActiveGuideText;

	struct {

	} Images;

	struct {
		// Title
		ImageData* SetPricesTitle;
		// Selection
		ImageData* SelectItem;
		ImageData* EnterPrice;
		// Options
		ImageData* SetPriceOption;
		ImageData* ForecastOption;
		ImageData* GuideOption;
		ImageData* LeaveOption;
		ImageData* SaveOption;
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
	} TextObjects;

	struct {

	} TextBoxObjects;

	struct {

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
	void LoadGameObjects();
	void LoadImagesText();
	void SceneStart();
	void HandleEvent(SDL_Event* Event);
	void Update(Uint32 timeStep);
	void Render();

	void GetCurrentPlayerInventory();

	// SetPrices funcs
	ImageData* AddSetPricesText(std::string _text, int _x, int _y) {
		ImageData* textImage = this->AddText(_text, _x, _y);
		this->SetPricesText.push_back(textImage);
		return textImage;
	}
	TextBox* AddSetPricesBox(Uint32 _size, int _x, int _y) {
		TextBox* textBox = this->AddTextBox(_size, _x, _y);
		this->SetPricesText.push_back(textBox);
		return textBox;
	}
	TextBox* AddItemBox(Uint32 _size, int _x, int _y) {
		TextBox* textBox = this->AddSetPricesBox(_size, _x, _y);
		this->ItemTextBoxObjects.push_back(textBox);
		return textBox;
	}

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

#endif