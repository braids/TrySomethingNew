#ifndef _MARKET_H_
#define _MARKET_H_

#include <vector>
#include <SDL2\SDL.h>
#include "Assets.h"
#include "Data\ItemData.h"
#include "Scenes\Scene.h"
#include "Scenes\SubScreens\EscapeScreen.h"
#include "Timer.h"

class Market : public Scene, EscapeScreen {
protected:
	// Main market screen text and text boxes
	std::vector<ImageData*> MarketText;
	std::vector<ImageData*> SubTotalText;
	ImageData*	ActiveBuySubTotal;
	std::vector<TextBox*> ItemTextBoxObjects;
	TextBox*	ActiveBuySelection;
	std::vector<ItemData*>	BuyData;
	ItemData*	ActiveItemData;

	int			SelectedItem;
	int			BuyTotal;

	struct {
		ImageData* SelectionBG;
	} Images;

	struct {
		// Total
		ImageData* TotalAmount;
		ImageData* PlayerMoneyAmount;
		ImageData* MoneySubTotalAmount;
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
		bool EnterItemQty;
		bool ShowForecast;
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
	void Cleanup();

	// Market funcs
	ImageData* AddMarketImage(Assets::Image* image, int _x, int _y);
	ImageData* AddMarketText(std::string _text, int _x, int _y);
	ImageData* AddSubTotalText(std::string _text, int _x, int _y);
	TextBox* AddMarketTextBox(Uint32 _size, int _x, int _y);
	void InitSelection();
	void UpdateSelection();
	void UpdateTotal();

	//// Events
	// Show/Hide Events
	void SEvent_ShowMarketText();
	void SEvent_HideMarketText();
	void SEvent_ShowForecast();
	void SEvent_HideForecast();
	void SEvent_ShowGuide();
	void SEvent_HideGuide();
	// Selection
	void SEvent_SelectionUp();
	void SEvent_SelectionDown();
	// Buy
	void SEvent_IncreaseItemQty();
	void SEvent_DecreaseItemQty();
	void SEvent_ZeroItemQty();
	void SEvent_ItemQtyEntry();
	void SEvent_EndItemQtyEntry();
	// Forecast
	void SEvent_SelectForecast();
	void SEvent_ExitForecast();
	// Guide
	//void SEvent_SelectGuide();
	//void SEvent_SetGuideItem(SDL_Keycode _key);
	void SEvent_OpenGuide();
	void SEvent_ExitGuide();
	// Leave
	void SEvent_Leave();
	// Exit to title
	void SEvent_ExitToTitle();
	void SEvent_ShowEscapeScreen();
	void SEvent_HideEscapeScreen();
	// Save
	void SEvent_Save();
	void SEvent_HideSaveText();
	// Error
	void SEvent_HideErrorText();
};

#endif

