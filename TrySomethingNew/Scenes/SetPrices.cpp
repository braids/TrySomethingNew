#include <algorithm>
#include <string>
#include <vector>
#include "Assets.h"
#include "Data\ItemData.h"
#include "Data\PlayerData.h"
#include "Graphics.h"
#include "Scenes\Scene.h"
#include "SceneManager.h"

SetPrices::SetPrices() {
	// Set current scene name
	this->SetSceneName(Scene_SetPrices);
}

void SetPrices::ResetFlags() {
	// Set flags to false
	this->EventFlags.ExitToTitleScreen = false;
	this->EventFlags.MainSelection = false;
	this->EventFlags.SelectSellItem = false;
	this->EventFlags.EnterItemPrice = false;
	this->EventFlags.ShowForecast = false;
	this->EventFlags.SelectGuideItem = false;
	this->EventFlags.ShowGuide = false;
}

void SetPrices::LoadGameObjects() {

}

void SetPrices::LoadImagesText() {
	// Clear any existing drawn text.
	this->mImages.clear();
	this->SetPricesText.clear();
	this->ItemTextBoxObjects.clear();

	//// Image objects
	
	//// Text objects
	
	// Item listings
	for (int i = 0; i < (int) this->SellItems.capacity(); i++) {
		int menuNum = i + 1;
		int y = 36 + (i * 9);
		// Add menu number and name
		std::string menuName = std::to_string(menuNum) + ") " + GetItemString(this->SellItems[i]->GetName());
		this->AddSetPricesText(menuName, 7, y);
		// Add buy price
		std::string buyPrice = "DM" + std::to_string(this->SellItems[i]->GetBuyPrice());
		this->AddSetPricesText(buyPrice, 161, y);
		// Add sale price and prefix
		this->AddSetPricesText("DM", 203, y);
		this->AddItemBox(3, 217, y);
		// Add quantity amount
		this->AddSetPricesText(std::to_string(this->SellItems[i]->GetQuantity()), 252, y);
	}
	// Title
	this->TextObjects.SetPricesTitle = this->AddSetPricesText("SET PRICES", 105, 9);
	// Headers
	this->TextObjects.Inventory = this->AddSetPricesText("-INVENTORY-", 21, 27);
	this->TextObjects.Cost = this->AddSetPricesText("COST", 161, 27);
	this->TextObjects.SellPrice = this->AddSetPricesText("SELL", 203, 27);
	this->TextObjects.Qty = this->AddSetPricesText("QTY", 252, 27);
	// Selection
	this->TextObjects.SelectItem = this->AddText("- SELECT ITEM #", 7, 153);
	this->TextObjects.EnterPrice = this->AddText("- ENTER PRICE", 7, 153);
	// Options
	this->TextObjects.SetPriceOption = this->AddSetPricesText("S)ET PRICE", 7, 180);
	this->TextObjects.ForecastOption = this->AddSetPricesText("F)ORECAST", 84, 180);
	this->TextObjects.GuideOption = this->AddSetPricesText("G)UIDE", 154, 180);
	this->TextObjects.LeaveOption = this->AddSetPricesText("O)PEN SHOP", 203, 180);
	// Press Return
	this->TextObjects.PressReturn = this->AddText("- PRESS RETURN -", 84, 180);
	// Forecast
	this->TextObjects.WeatherHeader = this->AddText("WEATHER FORECAST:", 105, 9);
	this->TextObjects.WeatherInfo = this->AddText("", 105, 9);
	this->TextObjects.EventHeader = this->AddText("LOCAL EVENTS", 105, 9);
	this->TextObjects.EventInfo = this->AddText("", 105, 9);
	// Guide Descriptions
	this->TextObjects.BierDesc = this->AddText("                 BIER\n\n    GERMANS OF ALL WALKS OF LIFE ENJOY THIS ALCOHOLIC DRINK. YOU CAN BE SURE THAT ANYONE PASSING BY WILL ENJOY THIS FROSTY COLD BEVERAGE.\n\nPREFERRED BY :\n    EAST & WEST BERLINERS", 7, 9);
	this->TextObjects.BockwurstDesc = this->AddText("               BOCKWURST\n\n    A SIMMERED SAUSAGE MADE WITH VEAL AND PORK, POPULAR IN EAST GERMANY AND OFTEN SERVED WITH A SMALL PIECE OF BREAD AND A DOLLOP OF MUSTARD. A CHEAP AND TASTY SNACK.\n\nPREFERRED BY:\n    EAST BERLINERS", 7, 9);
	this->TextObjects.MettigelDesc = this->AddText("               METTIGEL\n\n    METT IS A MIXTURE OF RAW MINCED PORK, SALT, PEPPER, AND CHOPPED ONION AND IS USUALLY SERVED ON A BREAD ROLL. METTIGEL IS A FUN TAKE WHERE THE METT IS SHAPED LIKE A HEDGEHOG AND  QUARTERED ONION SLICES ARE USED AS SPIKES.\n\nPREFERRED BY:\n    EAST & WEST BERLINERS", 7, 9);
	this->TextObjects.CurrywurstDesc = this->AddText("              CURRYWURST\n\n    A STEAMED, FRIED BRATWURST TOPPED WITH CURRYWURST SAUCE (TYPICALLY CURRY POWDER AND KETCHUP) SERVED WITH FRENCH FRIES. THIS IS A POPULAR SNACK BOTH DURING THE DAY AND LATE AT NIGHT.  COMES WITH ONE CURRYWURST FORK.\n\nPREFERRED BY:\n    WEST BERLINERS", 7, 9);
	this->TextObjects.StreetSheetDesc = this->AddText("          DIE STRAßENZEITUNG\n\n    \"THE STREET SHEET\" WAS WIDELY READ IN EAST GERMANY AND PROVIDED INFO ON UNDERGROUND EVENTS FROWNED ON BY THE GDR. WITH THE FALL OF THE WALL, THIS PAPER NOW REPORTS ON ALL MANNER OF  LIFE IN EAST AND WEST GERMANY.\n\nPREFERRED BY:\n    EAST BERLINERS", 7, 9);
	this->TextObjects.USADAYDesc = this->AddText("               USA DAY\n\n    THIS GERMAN TABLOID REPORTS ON THE RICH AND FAMOUS IN HOLLYWOOD. ITS WILD STORIES AND SALACIOUS DETAILS HAVE  MADE IT POPULAR WITH PEOPLE WHO WANT TO KNOW WHO'S WHO.\n\nPREFERRED BY:\n    WEST BERLINERS", 7, 9);

	for (std::vector<ImageData*>::iterator it = this->mImages.begin(); it != this->mImages.end(); it++)
		(*it)->SetVisible(false);
}

void SetPrices::SceneStart() {
	// Play Market music
	Mix_HaltMusic();

	// Reset Flags
	this->ResetFlags();

	// Start with text entry off
	SDL_StopTextInput();

	// Get player inventory
	this->GetCurrentPlayerInventory();

	// Load Images and Text Images
	this->LoadImagesText();

	// Display main market text
	this->SEvent_ShowSetPricesText();
	this->EventFlags.MainSelection = true;
}

void SetPrices::HandleEvent(SDL_Event * Event) {
	switch (Event->type) {
	case SDL_KEYDOWN:
		//// Exit event
		if (Event->key.keysym.sym == SDLK_ESCAPE)
			this->EventFlags.ExitToTitleScreen = true;

		//// Main selection
		if (this->EventFlags.MainSelection) {
			if (Event->key.keysym.sym == SDLK_s)
				this->SEvent_SelectSell();
			if (Event->key.keysym.sym == SDLK_f)
				this->SEvent_SelectForecast();
			if (Event->key.keysym.sym == SDLK_g)
				this->SEvent_SelectGuide();
			if (Event->key.keysym.sym == SDLK_o)
				this->SEvent_OpenShop();
		}

		//// Buy
		// Selecting item to buy
		if (this->EventFlags.SelectSellItem) {
			this->SEvent_SetSellItem(Event->key.keysym.sym);
		}
		// Enterting item quantity
		if (this->EventFlags.EnterItemPrice) {
			if (Event->key.keysym.sym == SDLK_BACKSPACE)
				this->ActiveSellSelection->DeleteText();
			if (Event->key.keysym.sym == SDLK_RETURN)
				this->SEvent_EndSellEntry();
		}

		//// Forecast
		// Exiting Forecast screen
		if (this->EventFlags.ShowForecast) {
			if (Event->key.keysym.sym == SDLK_RETURN)
				this->SEvent_ExitForecast();
		}

		//// Guide
		// Selecting item to inspect
		if (this->EventFlags.SelectGuideItem) {
			this->SEvent_SetGuideItem(Event->key.keysym.sym);
		}
		// Exiting Guide screen
		if (this->EventFlags.ShowGuide) {
			if (Event->key.keysym.sym == SDLK_RETURN)
				this->SEvent_ExitGuide();
		}

		break;

	case SDL_KEYUP:
		break;

	case SDL_TEXTINPUT:
		if (this->EventFlags.EnterItemPrice) {
			this->ActiveSellSelection->AppendText(Event->text.text);
		}
		break;

	case SDL_TEXTEDITING:
		break;

	default:
		break;
	}
}

void SetPrices::Update(Uint32 timeStep) {
	// Update timers
	this->UpdateEventTimers();

	// Return to title screen if quitting
	if (this->EventFlags.ExitToTitleScreen) {
		// Stop current music
		Mix_HaltMusic();
		// Go to title.
		this->mManager->StartScene(Scene_TitleScreen);
	}
}

void SetPrices::Render() {
	// Render graphics to buffer
	// If I find any game logic in here, I'll slap myself silly
	for (int i = 0; i < (int) this->mImages.size(); i++) {
		if (this->mImages[i]->IsVisible())
			this->mManager->GetGraphics()->DrawTextureAtLocation(
				this->mImages[i]->GetImage()->texture,
				this->mImages[i]->GetImage()->rect,
				this->mImages[i]->GetDrawRect(),
				this->mImages[i]->GetDrawAngle()
			);
	}
}

/// Scene funcs

void SetPrices::GetCurrentPlayerInventory() {
	// Clear current sell item list
	this->SellItems.clear();
	// Get all player items with inventory greater than 0
	std::for_each(
		this->mPlayerData->GetInventory()->begin(),
		this->mPlayerData->GetInventory()->end(),
		[this](ItemData* &_item) { if (_item->GetQuantity() > 0 && _item->GetType() != ItemType::ItemType_Ad) this->SellItems.push_back(_item); });
}

int SetPrices::KeycodeNumValue(SDL_Keycode _key) {
	return (_key >= SDLK_0 && _key <= SDLK_9) ? (int) _key - (int) SDLK_0 : -1;
}

//// Scene Events

void SetPrices::SEvent_ShowSetPricesText() {
	for (std::vector<ImageData*>::iterator it = this->SetPricesText.begin(); it != this->SetPricesText.end(); it++)
		(*it)->SetVisible(true);
}

void SetPrices::SEvent_HideSetPricesText() {
	for (std::vector<ImageData*>::iterator it = this->SetPricesText.begin(); it != this->SetPricesText.end(); it++)
		(*it)->SetVisible(false);
}

void SetPrices::SEvent_ShowForecast() {
	this->TextObjects.PressReturn->SetVisible(true);
}

void SetPrices::SEvent_HideForecast() {
	this->TextObjects.PressReturn->SetVisible(false);
}

void SetPrices::SEvent_ShowGuide() {
	this->ActiveGuideText->SetVisible(true);
	this->TextObjects.PressReturn->SetVisible(true);
}

void SetPrices::SEvent_HideGuide() {
	this->ActiveGuideText->SetVisible(false);
	this->TextObjects.PressReturn->SetVisible(false);
}

void SetPrices::SEvent_SelectSell() {
	this->EventFlags.MainSelection = false;
	this->TextObjects.SelectItem->SetVisible(true);
	this->EventFlags.SelectSellItem = true;
}

void SetPrices::SEvent_SetSellItem(SDL_Keycode _key) {
	int keyValue = this->KeycodeNumValue(_key);
	
	if (keyValue >= 1 && keyValue <= (int) this->SellItems.capacity()) {
		// Set active item if valid selection
		this->ActiveSellSelection = this->ItemTextBoxObjects[keyValue - 1];
		this->ActiveItemData = this->SellItems[keyValue - 1];
	}
	else if (_key == SDLK_RETURN) {
		// If enter is pressed, return to main selection
		this->EventFlags.SelectSellItem = false;
		this->TextObjects.SelectItem->SetVisible(false);
		this->EventFlags.MainSelection = true;
		return;
	}
	else {
		// Ignore other inputs
		return;
	}

	// Move into quantity entry state
	this->EventFlags.SelectSellItem = false;
	this->EventFlags.EnterItemPrice = true;

	// Show ENTER QUANITITY text
	this->TextObjects.SelectItem->SetVisible(false);
	this->TextObjects.EnterPrice->SetVisible(true);

	// Flush buffered text input
	SDL_PumpEvents();

	// Enable text entry for item quantity
	SDL_StartTextInput();
	this->ActiveSellSelection->SetActive(true);
}

void SetPrices::SEvent_EndSellEntry() {
	// Set sell price entered. Invalid input will convert to 0.
	this->ActiveItemData->SetSellPrice(std::atoi(this->ActiveSellSelection->GetText()->c_str()));

	// Set text to sanitized number.
	this->ActiveSellSelection->SetText(std::to_string(this->ActiveItemData->GetSellPrice()));

	// Stop text entry and return to main selection
	SDL_StopTextInput();
	this->ActiveSellSelection->SetActive(false);
	this->EventFlags.EnterItemPrice = false;
	this->TextObjects.EnterPrice->SetVisible(false);
	this->EventFlags.MainSelection = true;
}

void SetPrices::SEvent_SelectForecast() {
	this->EventFlags.MainSelection = false;
	this->SEvent_HideSetPricesText();
	this->EventFlags.ShowForecast = true;
	this->SEvent_ShowForecast();
}

void SetPrices::SEvent_ExitForecast() {
	this->EventFlags.ShowForecast = false;
	this->SEvent_HideForecast();
	this->EventFlags.MainSelection = true;
	this->SEvent_ShowSetPricesText();
}

void SetPrices::SEvent_SelectGuide() {
	this->EventFlags.MainSelection = false;
	this->TextObjects.SelectItem->SetVisible(true);
	this->EventFlags.SelectGuideItem = true;
}

void SetPrices::SEvent_SetGuideItem(SDL_Keycode _key) {
	// Select item to purchase
	switch (_key) {
	case SDLK_1:
		this->ActiveGuideText = this->TextObjects.BierDesc;
		break;
	case SDLK_2:
		this->ActiveGuideText = this->TextObjects.BockwurstDesc;
		break;
	case SDLK_3:
		this->ActiveGuideText = this->TextObjects.MettigelDesc;
		break;
	case SDLK_4:
		this->ActiveGuideText = this->TextObjects.CurrywurstDesc;
		break;
	case SDLK_5:
		this->ActiveGuideText = this->TextObjects.StreetSheetDesc;
		break;
	case SDLK_6:
		this->ActiveGuideText = this->TextObjects.USADAYDesc;
		break;
	case SDLK_RETURN:
		// If enter is pressed, return to main selection
		this->EventFlags.SelectGuideItem = false;
		this->TextObjects.SelectItem->SetVisible(false);
		this->EventFlags.MainSelection = true;
		return;
		break;
	default:
		// Ignore all other input
		return;
		break;
	}

	// Move into guide display state
	this->EventFlags.SelectGuideItem = false;
	this->EventFlags.ShowGuide = true;

	// Show Guide text
	this->SEvent_HideSetPricesText();
	this->TextObjects.SelectItem->SetVisible(false);
	this->SEvent_ShowGuide();

	// Flush buffered text input
	SDL_PumpEvents();
}

void SetPrices::SEvent_ExitGuide() {
	this->EventFlags.ShowGuide = false;
	this->SEvent_HideGuide();
	this->EventFlags.MainSelection = true;
	this->SEvent_ShowSetPricesText();
}

void SetPrices::SEvent_OpenShop() {
	// Set player sell prices
	for (std::vector<ItemData*>::iterator it = this->SellItems.begin(); it != this->SellItems.end(); it++)
		this->mPlayerData->GetInventoryItem((*it)->GetName())->SetSellPrice((*it)->GetSellPrice());
	
	// Leave Set Price screen
	this->EventFlags.ExitToTitleScreen = true;
}