#include <fstream>
#include <string>
#include <vector>
#include <SDL2\SDL.h>
#include <SDL2\SDL_mixer.h>
#include "Assets.h"
#include "Data\ItemData.h"
#include "Data\PlayerData.h"
#include "Graphics.h"
#include "Scenes\Market.h"
#include "Scenes\Scene.h"
#include "Scenes\SubScreens\EscapeScreen.h"
#include "SceneManager.h"
#include "Timer.h"

Market::Market() {
	// Set current scene name
	this->SetSceneName(Scene_Market);
}

void Market::ResetFlags() {
	// Set flags to false
	this->EventFlags.ExitToTitleScreen = false;
	this->EventFlags.MainSelection = false;
	this->EventFlags.SelectBuyItem = false;
	this->EventFlags.EnterItemQty = false;
	this->EventFlags.ShowForecast = false;
	this->EventFlags.SelectGuideItem = false;
	this->EventFlags.ShowGuide = false;
}

void Market::LoadEventTimers() {
	this->mEventTimers.clear();

	// Init event timers
	this->EventTimers.GameSaved = this->AddEventTimer(new EventTimer(std::bind(&Market::SEvent_HideSaveText, this), (Uint32)1000));
	this->EventTimers.ErrorText = this->AddEventTimer(new EventTimer(std::bind(&Market::SEvent_HideErrorText, this), (Uint32)1000));
}

void Market::LoadImagesText() {
	// Clear any existing drawn images or text
	this->mImages.clear();
	this->MarketText.clear();
	this->SubTotalText.clear();
	this->ItemTextBoxObjects.clear();

	//// Text objects
	// Market Title
	this->AddMarketText("DER MORGENMARKT", 91, 9);
	// Purchase signs
	this->AddMarketText("x\nx\nx\nx\n\nx\nx\n\nx\nx\nx", 196, 36);
	this->AddMarketText("=\n=\n=\n=\n\n=\n=\n\n=\n=\n=", 231, 36);
	// Headers
	this->AddMarketText("-FOODS-", 21, 27);
	this->AddMarketText("COST", 161, 27);
	this->AddMarketText("QTY", 203, 27);
	this->AddMarketText("TOTAL", 238, 27);
	this->AddMarketText("-NEWSPAPERS-", 21, 72);
	this->AddMarketText("-ADS-", 21, 99);
	// Items
	for (int i = 0; i < (int) this->BuyData.size(); i++) {
		int menuNum = i + 1;
		int y = 36 + (i * 9);
		
		// Offset for item sections
		if (this->BuyData[i]->GetType() == ItemType::ItemType_Newspaper)
			y += 9;
		if (this->BuyData[i]->GetType() == ItemType::ItemType_Ad)
			y += 18;
		
		// Add menu number and name
		std::string menuName = std::to_string(menuNum) + ") " + GetItemString(this->BuyData[i]->GetName());
		this->AddMarketText(menuName, 7, y);
		
		// Add buy price
		std::string buyPrice = "DM" + std::to_string(this->BuyData[i]->GetBuyPrice());		
		this->AddMarketText(buyPrice, 161, y);
		
		// Add purchase quantity text box
		this->AddMarketTextBox(3, 203, y);
		
		// Add purchase subtotal text
		this->AddSubTotalText("0", 245, y);
	}
	// Total
	this->AddMarketText("MONEY:", 182, 144);
	this->TextObjects.PlayerMoneyAmount = this->AddMarketText("0", 231, 144);
	this->AddMarketText("- TOTAL:", 168, 153);
	this->TextObjects.TotalAmount = this->AddMarketText("0", 231, 153);
	this->AddMarketText("=", 217, 162);
	this->TextObjects.MoneySubTotalAmount = this->AddMarketText("0", 231, 162);
	// Selection
	this->TextObjects.SelectItem = this->AddText("- SELECT ITEM #", 7, 153);
	this->TextObjects.EnterQty = this->AddText("- ENTER QUANTITY", 7, 153);
	// Options
	this->AddMarketText("B)UY", 7, 180);
	this->AddMarketText("F)ORECAST", 49, 180);
	this->AddMarketText("G)UIDE", 126, 180);
	this->AddMarketText("L)EAVE", 182, 180);
	this->AddMarketText("S)AVE", 238, 180);
	// Error text
	this->TextObjects.ErrBuyItem = this->AddText("- BUY FOOD/NEWS -", 84, 171);
	this->TextObjects.ErrNoMoney = this->AddText("- NOT ENOUGH MONEY -", 70, 171);
	// Game Saved
	this->TextObjects.GameSaved = this->AddText("- GAME SAVED -", 91, 171);
	// Press Return
	this->TextObjects.PressReturn = this->AddText("- PRESS RETURN -", 84, 180);
	// Forecast
	this->TextObjects.WeatherHeader = this->AddText("WEATHER FORECAST:", 84, 27);
	this->TextObjects.WeatherInfo = this->AddText("", 7, 45);
	this->TextObjects.EventHeader = this->AddText("LOCAL EVENTS:", 97, 90);
	this->TextObjects.EventInfo = this->AddText("", 7, 108);
	// Guide Descriptions
	this->TextObjects.GuideText = this->AddText("", 7, 9);

	// Load escape screen text
	this->AddEscapeText(this);

	// Hide all images
	for (std::vector<ImageData*>::iterator it = this->mImages.begin(); it != this->mImages.end(); it++)
		(*it)->SetVisible(false);
}

void Market::SceneStart() {
	// Play Market music
	if(!Mix_PlayingMusic())
		Mix_PlayMusic(Assets::Instance()->music.JazzyMusic, -1);
	// Reset Flags
	this->ResetFlags();

	// Start with text entry off
	SDL_StopTextInput();

	// Initialize buy vectors
	this->mPlayerData->ClearInventory();
	this->BuyData = *GetInitialItemVector();
	this->BuyTotal = 0;

	// Load Event Timers
	this->LoadEventTimers();
	// Load Images and Text Images
	this->LoadImagesText();

	// Display main market text
	this->SEvent_ShowMarketText();
	this->EventFlags.MainSelection = true;

	// Set player money text
	this->TextObjects.PlayerMoneyAmount->SetText(std::to_string(this->mPlayerData->GetMoney()));
}

void Market::HandleEvent(SDL_Event * Event) {
	switch (Event->type) {
	case SDL_KEYDOWN:
		//// Exit event
		if (this->EscapeScreenVisible) {
			if (Event->key.keysym.sym == SDLK_y) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->SEvent_ExitToTitle();
			}
			if (Event->key.keysym.sym == SDLK_n) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->SEvent_HideEscapeScreen();
			}

			break;
		}

		if (Event->key.keysym.sym == SDLK_ESCAPE) {
			Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
			this->SEvent_ShowEscapeScreen();
		}
		
		//// Main selection
		if (this->EventFlags.MainSelection) {
			if (Event->key.keysym.sym == SDLK_b) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->SEvent_SelectBuy();
			}
			if (Event->key.keysym.sym == SDLK_f) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->SEvent_SelectForecast();
			}
			if (Event->key.keysym.sym == SDLK_g) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->SEvent_SelectGuide();
			}
			if (Event->key.keysym.sym == SDLK_l) {
				this->SEvent_Leave();
			}
			if (Event->key.keysym.sym == SDLK_s) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->SEvent_Save();
			}
		}

		//// Buy
		// Selecting item to buy
		if (this->EventFlags.SelectBuyItem) {
			this->SEvent_SetBuyItem(Event->key.keysym.sym);
		}
		// Enterting item quantity
		if (this->EventFlags.EnterItemQty) {
			if(Event->key.keysym.sym == SDLK_BACKSPACE)
				this->ActiveBuySelection->DeleteText();
			if (Event->key.keysym.sym == SDLK_RETURN) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->SEvent_EndItemQtyEntry();
			}
		}

		//// Forecast
		// Exiting Forecast screen
		if (this->EventFlags.ShowForecast) {
			if (Event->key.keysym.sym == SDLK_RETURN) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->SEvent_ExitForecast();
			}
		}

		//// Guide
		// Selecting item to inspect
		if (this->EventFlags.SelectGuideItem) {
			this->SEvent_SetGuideItem(Event->key.keysym.sym);
		}
		// Exiting Guide screen
		if (this->EventFlags.ShowGuide) {
			if (Event->key.keysym.sym == SDLK_RETURN) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->SEvent_ExitGuide();
			}
		}

		break;

	case SDL_KEYUP:
		break;

	case SDL_TEXTINPUT:
		if (this->EventFlags.EnterItemQty) {
			this->ActiveBuySelection->AppendText(Event->text.text);
		}
		break;

	case SDL_TEXTEDITING:
		break;

	default:
		break;
	}
}

void Market::Update(Uint32 timeStep) {
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

void Market::Render() {
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

void Market::Cleanup() {
	// Clear vectors
	this->mImages.clear();
	this->MarketText.clear();
	this->SubTotalText.clear();
	this->ItemTextBoxObjects.clear();
	this->BuyData.clear();
	this->EscapeImagesText.clear();

	// Stop timers
	this->EventTimers.GameSaved->stop();
	this->EventTimers.ErrorText->stop();
}

//// Market funcs
ImageData* Market::AddMarketText(std::string _text, int _x, int _y) {
	ImageData* textImage = this->AddText(_text, _x, _y);
	this->MarketText.push_back(textImage);
	return textImage;
}

ImageData* Market::AddSubTotalText(std::string _text, int _x, int _y) {
	ImageData* textImage = this->AddText(_text, _x, _y);
	this->MarketText.push_back(textImage);
	this->SubTotalText.push_back(textImage);
	return textImage;
}

TextBox* Market::AddMarketTextBox(Uint32 _size, int _x, int _y) {
	TextBox* textBox = this->AddTextBox(_size, _x, _y);
	this->ItemTextBoxObjects.push_back(textBox);
	this->MarketText.push_back(textBox);
	return textBox;
}

void Market::UpdateTotal() {
	this->BuyTotal = 0;

	// Get all buy subtotals
	std::vector<ItemData*>::iterator it = this->BuyData.begin();
	for (; it != this->BuyData.end(); it++)
		this->BuyTotal += (*it)->GetBuyPrice() * (*it)->GetQuantity();

	// Update subtotal text
	this->TextObjects.TotalAmount->SetText(std::to_string(this->BuyTotal));

	// Update remaining money text
	this->TextObjects.MoneySubTotalAmount->SetText(std::to_string(this->mPlayerData->GetMoney() - this->BuyTotal));
}

//// Scene Events
void Market::SEvent_ShowMarketText() {
	for (std::vector<ImageData*>::iterator it = this->MarketText.begin(); it != this->MarketText.end(); it++)
		(*it)->SetVisible(true);
}

void Market::SEvent_HideMarketText() {
	for (std::vector<ImageData*>::iterator it = this->MarketText.begin(); it != this->MarketText.end(); it++)
		(*it)->SetVisible(false);
}

void Market::SEvent_ShowForecast() {
	this->TextObjects.WeatherHeader->SetVisible(true);
	this->TextObjects.WeatherInfo->SetText(GetWeatherDesc(this->mPlayerData->GetWeatherForecast()));
	this->TextObjects.WeatherInfo->SetVisible(true);
	this->TextObjects.EventHeader->SetVisible(true);
	this->TextObjects.EventInfo->SetText(GetEventDesc(this->mPlayerData->GetEventForecast()));
	this->TextObjects.EventInfo->SetVisible(true);
	this->TextObjects.PressReturn->SetVisible(true);
}

void Market::SEvent_HideForecast() {
	this->TextObjects.WeatherHeader->SetVisible(false);
	this->TextObjects.WeatherInfo->SetVisible(false);
	this->TextObjects.EventHeader->SetVisible(false);
	this->TextObjects.EventInfo->SetVisible(false);
	this->TextObjects.PressReturn->SetVisible(false);
}

void Market::SEvent_ShowGuide() {
	this->TextObjects.GuideText->SetVisible(true);
	this->TextObjects.PressReturn->SetVisible(true);
}

void Market::SEvent_HideGuide() {
	this->TextObjects.GuideText->SetVisible(false);
	this->TextObjects.PressReturn->SetVisible(false);
}

void Market::SEvent_SelectBuy() {
	this->EventFlags.MainSelection = false;
	this->TextObjects.SelectItem->SetVisible(true);
	this->EventFlags.SelectBuyItem = true;
}

void Market::SEvent_SetBuyItem(SDL_Keycode _key) {
	int keyValue = this->KeycodeNumValue(_key);

	if (keyValue >= 1 && keyValue <= (int) this->BuyData.size()) {
		// Set active item if valid selection
		this->ActiveBuySelection = this->ItemTextBoxObjects[keyValue - 1];
		this->ActiveBuySubTotal = this->SubTotalText[keyValue - 1];
		this->ActiveItemData = this->BuyData[keyValue - 1];
		Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
	}
	else if (_key == SDLK_RETURN) {
		// If enter is pressed, return to main selection
		this->EventFlags.SelectBuyItem = false;
		this->TextObjects.SelectItem->SetVisible(false);
		this->EventFlags.MainSelection = true;
		Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
		return;
	}
	else {
		// Ignore other inputs
		return;
	}

	// Move into quantity entry state
	this->EventFlags.SelectBuyItem = false;
	this->EventFlags.EnterItemQty = true;
	
	// Show ENTER QUANITITY text
	this->TextObjects.SelectItem->SetVisible(false);
	this->TextObjects.EnterQty->SetVisible(true);
	
	// Flush buffered text input
	SDL_PumpEvents();

	// Enable text entry for item quantity
	SDL_StartTextInput();
	this->ActiveBuySelection->SetActive(true);
}

void Market::SEvent_EndItemQtyEntry() {
	// Get quantity entered. Invalid input will convert to 0.
	int qty = std::atoi(this->ActiveBuySelection->GetText()->c_str());
	if (qty < 0) 
		qty = 0;

	this->ActiveItemData->SetQuantity(qty);
	this->ActiveItemData->SetBoughtQuantity(this->ActiveItemData->GetQuantity());

	// Set text to sanitized number.
	this->ActiveBuySelection->SetText(std::to_string(this->ActiveItemData->GetQuantity()));

	// Set subtotal price
	int subtotal = this->ActiveItemData->GetBuyPrice() * this->ActiveItemData->GetQuantity();
	this->ActiveBuySubTotal->SetText(std::to_string(subtotal));
	
	// Update grand total
	this->UpdateTotal();
	
	// Stop text entry and return to main selection
	SDL_StopTextInput();
	this->ActiveBuySelection->SetActive(false);
	this->EventFlags.EnterItemQty = false;
	this->TextObjects.EnterQty->SetVisible(false);
	this->EventFlags.MainSelection = true;
}

void Market::SEvent_SelectForecast() {
	this->EventFlags.MainSelection = false;
	this->SEvent_HideMarketText();
	this->EventFlags.ShowForecast = true;
	this->SEvent_ShowForecast();
}

void Market::SEvent_ExitForecast() {
	this->EventFlags.ShowForecast = false;
	this->SEvent_HideForecast();
	this->EventFlags.MainSelection = true;
	this->SEvent_ShowMarketText();
}

void Market::SEvent_SelectGuide() {
	this->EventFlags.MainSelection = false;
	this->TextObjects.SelectItem->SetVisible(true);
	this->EventFlags.SelectGuideItem = true;
}

void Market::SEvent_SetGuideItem(SDL_Keycode _key) {
	// Select item to view description of
	int keyValue = this->KeycodeNumValue(_key);

	if (keyValue >= 1 && keyValue <= (int) this->BuyData.size()) {
		this->TextObjects.GuideText->SetText(GetItemGuideDesc(this->BuyData[keyValue - 1]->GetName()));
		Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
	}
	else if (_key == SDLK_RETURN) {
		// If enter is pressed, return to main selection
		this->EventFlags.SelectGuideItem = false;
		this->TextObjects.SelectItem->SetVisible(false);
		this->EventFlags.MainSelection = true;
		Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
		return;
	}
	else {
		// Ignore other inputs
		return;
	}

	// Move into guide display state
	this->EventFlags.SelectGuideItem = false;
	this->EventFlags.ShowGuide = true;

	// Show Guide text
	this->SEvent_HideMarketText();
	this->TextObjects.SelectItem->SetVisible(false);
	this->SEvent_ShowGuide();

	// Flush buffered text input
	SDL_PumpEvents();
}

void Market::SEvent_ExitGuide() {
	this->EventFlags.ShowGuide = false;
	this->SEvent_HideGuide();
	this->EventFlags.MainSelection = true;
	this->SEvent_ShowMarketText();
}

void Market::SEvent_Leave() {
	////Check if product bought
	int productBought = 0;
	// Check all non-ad items for at least one bought item
	for (std::vector<ItemData*>::iterator it = this->BuyData.begin(); it != this->BuyData.end(); it++) {
		if ((*it)->GetQuantity() > 0 && (*it)->GetType() != ItemType::ItemType_Ad)
			productBought++;
	}
	// If nothing bought, prevent leaving.
	if (productBought == 0) {
		this->TextObjects.ErrBuyItem->SetVisible(true);
		this->EventTimers.ErrorText->StartEventTimer();
		Mix_PlayChannel(1, Assets::Instance()->sounds.Buzz, 0);
		return;
	}

	if (this->mPlayerData->GetMoney() >= this->BuyTotal) {
		// Set player inventory equal to items purchased
		for (std::vector<ItemData*>::iterator it = this->BuyData.begin(); it != this->BuyData.end(); it++) {
			this->mPlayerData->GetInventoryItem((*it)->GetName())->SetQuantity((*it)->GetQuantity());
			this->mPlayerData->GetInventoryItem((*it)->GetName())->SetBoughtQuantity((*it)->GetQuantity());
		}
		Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
		// Set player money
		this->mPlayerData->SetMoney(this->mPlayerData->GetMoney() - this->BuyTotal);
		
		// Leave market
		this->mManager->StartScene(Scene_SetPrices);
	}
	else {
		this->TextObjects.ErrNoMoney->SetVisible(true);
		this->EventTimers.ErrorText->StartEventTimer();
		Mix_PlayChannel(1, Assets::Instance()->sounds.Buzz, 0);
	}
}

void Market::SEvent_ExitToTitle() {
	// Halt music
	Mix_HaltMusic();

	// Return to title screen
	this->mManager->StartScene(Scene_TitleScreen);
}

void Market::SEvent_ShowEscapeScreen() {
	this->PauseTimers();
	Mix_PauseMusic();

	if (this->EventFlags.EnterItemQty) {
		SDL_StopTextInput();
		this->ActiveBuySelection->SetActive(false);
	}

	this->EscapeScreenVisible = true;
	this->ShowEscapeText();
}

void Market::SEvent_HideEscapeScreen() {
	this->UnpauseTimers();
	if (Mix_PausedMusic())
		Mix_ResumeMusic();

	if (this->EventFlags.EnterItemQty) {
		// Flush buffered text input
		SDL_PumpEvents();

		SDL_StartTextInput();
		this->ActiveBuySelection->SetActive(true);
	}

	this->EscapeScreenVisible = false;
	this->HideEscapeText();
}

void Market::SEvent_Save() {
	// Open filestream to save file
	std::ofstream savefile(".\\TSN.sav", std::ios::binary);

	// If file can't be read (permissions, doens't exist) then bail
	if (!savefile.good())
		return;

	SaveFile* savedata = new SaveFile(*(this->mPlayerData));

	// Read saved player data into current PlayerData
	savefile.write((char*)savedata, sizeof(*savedata));

	// Close filestream
	savefile.close();

	// Start GameSaved text timer and display text
	this->EventTimers.GameSaved->StartEventTimer();
	this->TextObjects.GameSaved->SetVisible(true);
}

void Market::SEvent_HideSaveText() {
	// Set Game Saved text to false
	this->TextObjects.GameSaved->SetVisible(false);
}

void Market::SEvent_HideErrorText() {
	this->TextObjects.ErrBuyItem->SetVisible(false);
	this->TextObjects.ErrNoMoney->SetVisible(false);
}
