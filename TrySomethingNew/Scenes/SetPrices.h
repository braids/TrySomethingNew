#ifndef _SETPRICES_H
#define _SETPRICES_H_

#include <string>
#include <vector>
#include <SDL2\SDL.h>
#include "Assets.h"
#include "Data\ItemData.h"
#include "Scenes\Scene.h"
#include "Timer.h"

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

#endif
