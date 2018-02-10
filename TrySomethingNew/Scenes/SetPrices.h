#ifndef _SETPRICES_H
#define _SETPRICES_H_

#include <string>
#include <vector>
#include <SDL2\SDL.h>
#include "Assets.h"
#include "Data\ItemData.h"
#include "Scenes\Scene.h"
#include "Scenes\SubScreens\EscapeScreen.h"
#include "Timer.h"

//// SetPrices scene class
class SetPrices : public Scene, EscapeScreen {
protected:
	// Set Prices screen text and text boxes
	std::vector<ImageData*> SetPricesText;
	std::vector<TextBox*> ItemTextBoxObjects;
	TextBox*	ActiveSellSelection;
	std::vector<ItemData*> SellData;
	ItemData*	ActiveItemData;

	int			SelectedItem;
	
	struct {
		ImageData* SelectionBG;
	} Images;
	
	struct {
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
		bool EnterItemPrice;
		bool ShowForecast;
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
	void Cleanup();

	// SetPrices funcs
	ImageData* AddSetPricesImage(Assets::Image* image, int _x, int _y);
	ImageData* AddSetPricesText(std::string _text, int _x, int _y);
	TextBox* AddSetPricesItemBox(Uint32 _size, int _x, int _y);
	void InitSelection();
	void UpdateSelection();
	void GetCurrentPlayerInventory();

	//// Events
	// Show/Hide Events
	void SEvent_ShowSetPricesText();
	void SEvent_HideSetPricesText();
	void SEvent_ShowForecast();
	void SEvent_HideForecast();
	void SEvent_ShowGuide();
	void SEvent_HideGuide();
	// Selection
	void SEvent_SelectionUp();
	void SEvent_SelectionDown();
	// Set Price
	void SEvent_IncreaseItemSell();
	void SEvent_DecreaseItemSell();
	void SEvent_ZeroItemSell();
	void SEvent_ItemSellEntry();
	void SEvent_EndSellEntry();
	// Forecast
	void SEvent_SelectForecast();
	void SEvent_ExitForecast();
	// Guide
	void SEvent_OpenGuide();
	void SEvent_ExitGuide();
	// Leave
	void SEvent_OpenShop();
	// Exit to title
	void SEvent_ExitToTitle();
	void SEvent_ShowEscapeScreen();
	void SEvent_HideEscapeScreen();
	// Error
	void SEvent_HideErrorText();
};

#endif
