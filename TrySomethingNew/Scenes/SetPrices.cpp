#include <algorithm>
#include <string>
#include <vector>
#include <SDL2\SDL.h>
#include <SDL2\SDL_mixer.h>
#include "Assets.h"
#include "Data\ItemData.h"
#include "Data\PlayerData.h"
#include "Graphics.h"
#include "Scenes\Scene.h"
#include "Scenes\SetPrices.h"
#include "Scenes\SubScreens\EscapeScreen.h"
#include "SceneManager.h"
#include "Timer.h"

//// Scene Ctor
SetPrices::SetPrices() {
	// Set current scene name
	this->SetSceneName(Scene_SetPrices);
}

//// Scene funcs
void SetPrices::ResetFlags() {
	// Set flags to false
	this->EventFlags.ExitToTitleScreen = false;
	this->EventFlags.MainSelection = false;
	this->EventFlags.EnterItemPrice = false;
	this->EventFlags.ShowForecast = false;
	this->EventFlags.ShowGuide = false;
}

void SetPrices::LoadEventTimers() {
	this->EventTimers.ErrorText = this->AddEventTimer(new EventTimer(std::bind(&SetPrices::SEvent_HideErrorText, this), (Uint32)1000));
}

void SetPrices::LoadImagesText() {
	// Clear any existing drawn text.
	this->mImages.clear();
	this->SetPricesText.clear();
	this->ItemTextBoxObjects.clear();

	//// Image objects
	
	// Selection
	this->Images.SelectionBG = this->AddSetPricesImage(&Assets::Instance()->images.SelectionRect, 0, 0);

	//// Text objects
	
	// Item listings
	for (int i = 0; i < (int) this->SellData.size(); i++) {
		int menuNum = i + 1;
		int y = 36 + (i * 9);
		// Add menu number and name
		std::string menuName = std::to_string(menuNum) + ") " + GetItemString(this->SellData[i]->GetName());
		this->AddSetPricesText(menuName, 7, y);
		// Add buy price
		std::string buyPrice = "DM" + std::to_string(this->SellData[i]->GetBuyPrice());
		this->AddSetPricesText(buyPrice, 161, y);
		// Add sale price and prefix
		this->AddSetPricesText("DM", 203, y);
		ImageData* textBox = this->AddSetPricesItemBox(3, 217, y);
		textBox->SetText(std::to_string(this->SellData[i]->GetSellPrice()));

		// Add quantity amount
		this->AddSetPricesText(std::to_string(this->SellData[i]->GetBoughtQuantity()), 252, y);
	}
	// Title
	this->AddSetPricesText("SET PRICES", 105, 9);
	// Headers
	this->AddSetPricesText("-INVENTORY-", 21, 27);
	this->AddSetPricesText("COST", 161, 27);
	this->AddSetPricesText("SELL", 203, 27);
	this->AddSetPricesText("QTY", 252, 27);
	// Options
	this->AddSetPricesText("F)ORECAST", 7, 180);
	this->AddSetPricesText("G)UIDE", 98, 180);
	this->AddSetPricesText("O)PEN SHOP", 203, 180);
	// Error
	this->TextObjects.ErrSetPrice = this->AddText("- SET PRICE(S) -", 84, 171);
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

void SetPrices::SceneStart() {
	// Play Market music
	if(!Mix_PlayingMusic())
		Mix_PlayMusic(Assets::Instance()->music.JazzyMusic, -1);

	// Reset Flags
	this->ResetFlags();

	// Load event timers
	this->LoadEventTimers();

	// Start with text entry off
	SDL_StopTextInput();

	// Get player inventory
	this->GetCurrentPlayerInventory();

	// Load Images and Text Images
	this->LoadImagesText();

	// Initialize selection
	this->InitSelection();

	// Display set prices text
	this->SEvent_ShowSetPricesText();
	this->EventFlags.MainSelection = true;
}

void SetPrices::HandleEvent(SDL_Event * Event) {
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
			if (Event->key.keysym.sym == SDLK_UP) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->SEvent_SelectionUp();
			}
			if (Event->key.keysym.sym == SDLK_DOWN) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->SEvent_SelectionDown();
			}
			if (Event->key.keysym.sym == SDLK_LEFT) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->SEvent_DecreaseItemSell();
			}
			if (Event->key.keysym.sym == SDLK_RIGHT) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->SEvent_IncreaseItemSell();
			}
			if (Event->key.keysym.sym == SDLK_DELETE || Event->key.keysym.sym == SDLK_BACKSPACE) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->SEvent_ZeroItemSell();
			}
			if (Event->key.keysym.sym == SDLK_RETURN || Event->key.keysym.sym == SDLK_KP_ENTER) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->SEvent_ItemSellEntry();
				break;
			}
			if (Event->key.keysym.sym == SDLK_f) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->SEvent_SelectForecast();
			}
			if (Event->key.keysym.sym == SDLK_g) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->SEvent_OpenGuide();
			}
			if (Event->key.keysym.sym == SDLK_o) {
				this->SEvent_OpenShop();
			}
		}

		//// Set Price
		// Enterting item price
		if (this->EventFlags.EnterItemPrice) {
			if (Event->key.keysym.sym == SDLK_BACKSPACE)
				this->ActiveSellSelection->DeleteText();
			if (Event->key.keysym.sym == SDLK_RETURN || Event->key.keysym.sym == SDLK_KP_ENTER) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->SEvent_EndSellEntry();
			}
		}

		//// Forecast
		// Exiting Forecast screen
		if (this->EventFlags.ShowForecast) {
			if (Event->key.keysym.sym == SDLK_RETURN || Event->key.keysym.sym == SDLK_KP_ENTER) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->SEvent_ExitForecast();
			}
		}

		//// Guide
		// Exiting Guide screen
		if (this->EventFlags.ShowGuide) {
			if (Event->key.keysym.sym == SDLK_RETURN || Event->key.keysym.sym == SDLK_KP_ENTER) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->SEvent_ExitGuide();
			}
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

void SetPrices::Cleanup() {
	// Clear vectors
	this->mImages.clear();
	this->SetPricesText.clear();
	this->ItemTextBoxObjects.clear();
	this->SellData.clear();
	this->EscapeImagesText.clear();

	// Stop timers
	this->EventTimers.ErrorText->stop();
}

ImageData * SetPrices::AddSetPricesImage(Assets::Image * image, int _x, int _y) {
	ImageData* imageData = this->AddImage(image, _x, _y);
	this->SetPricesText.push_back(imageData);
	return imageData;
}

//// SetPrices funcs
ImageData* SetPrices::AddSetPricesText(std::string _text, int _x, int _y) {
	ImageData* textImage = this->AddText(_text, _x, _y);
	this->SetPricesText.push_back(textImage);
	return textImage;
}

TextBox* SetPrices::AddSetPricesItemBox(Uint32 _size, int _x, int _y) {
	TextBox* textBox = this->AddTextBox(_size, _x, _y);
	this->ItemTextBoxObjects.push_back(textBox);
	this->SetPricesText.push_back(textBox);
	return textBox;
}

void SetPrices::InitSelection() {
	// Set first item as initial selected item
	this->SelectedItem = 0;

	// Update selection data
	this->UpdateSelection();
}

void SetPrices::UpdateSelection() {
	// If a text box is already active, reset the color
	if (this->ActiveItemData != nullptr)
		this->ActiveSellSelection->ReverseColor();

	// Set selection data
	this->ActiveSellSelection = this->ItemTextBoxObjects[this->SelectedItem];
	this->ActiveItemData = this->SellData[this->SelectedItem];

	// Move Selection BG behind current selection
	this->Images.SelectionBG->SetDrawRectXY(this->ActiveSellSelection->GetDrawRect()->x, this->ActiveSellSelection->GetDrawRect()->y - 1);

	// Reverse text color
	this->ActiveSellSelection->ReverseColor();
}

void SetPrices::GetCurrentPlayerInventory() {
	// Clear current sell item list
	this->SellData.clear();
	// Get all player items with inventory greater than 0
	std::for_each(this->mPlayerData->GetInventory()->begin(), this->mPlayerData->GetInventory()->end(),
		[this](ItemData* &_item) { 
		if (_item->GetBoughtQuantity() > 0 && _item->GetType() != ItemType::ItemType_Ad) {
			this->SellData.push_back(_item);
		}
	});
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
	this->TextObjects.WeatherHeader->SetVisible(true);
	this->TextObjects.WeatherInfo->SetText(GetWeatherDesc(this->mPlayerData->GetWeatherForecast()));
	this->TextObjects.WeatherInfo->SetVisible(true);
	this->TextObjects.EventHeader->SetVisible(true);
	this->TextObjects.EventInfo->SetText(GetEventDesc(this->mPlayerData->GetEventForecast()));
	this->TextObjects.EventInfo->SetVisible(true);
	this->TextObjects.PressReturn->SetVisible(true);
}

void SetPrices::SEvent_HideForecast() {
	this->TextObjects.WeatherHeader->SetVisible(false);
	this->TextObjects.WeatherInfo->SetVisible(false);
	this->TextObjects.EventHeader->SetVisible(false);
	this->TextObjects.EventInfo->SetVisible(false);
	this->TextObjects.PressReturn->SetVisible(false);
}

void SetPrices::SEvent_ShowGuide() {
	this->TextObjects.GuideText->SetVisible(true);
	this->TextObjects.PressReturn->SetVisible(true);
}

void SetPrices::SEvent_HideGuide() {
	this->TextObjects.GuideText->SetVisible(false);
	this->TextObjects.PressReturn->SetVisible(false);
}

void SetPrices::SEvent_SelectionUp() {
	// Subtract one from SelectedItem. If it's lower than 0, make it last item.
	if (--this->SelectedItem < 0)
		this->SelectedItem = this->SellData.size() - 1;

	// Update selection data
	this->UpdateSelection();
}

void SetPrices::SEvent_SelectionDown() {
	// Add one to SelectedItem. If it's higher than last item, make it 0.
	if (++this->SelectedItem >= (int) this->SellData.size())
		this->SelectedItem = 0;

	// Update selection data
	this->UpdateSelection();
}

void SetPrices::SEvent_IncreaseItemSell() {
	int price = this->ActiveItemData->GetSellPrice();

	// If increased sell price is greater than the max quantity value, bail
	if (++price > 999)
		return;

	// Set sell price entered. Invalid input will convert to 0.
	this->ActiveItemData->SetSellPrice(price);

	// Set text to sanitized number.
	this->ActiveSellSelection->SetText(std::to_string(this->ActiveItemData->GetSellPrice()));
	this->ActiveSellSelection->ReverseColor();
}

void SetPrices::SEvent_DecreaseItemSell() {
	int price = this->ActiveItemData->GetSellPrice();

	// If decreased sell price is less than 0, bail
	if (--price < 0)
		return;

	// Set sell price entered. Invalid input will convert to 0.
	this->ActiveItemData->SetSellPrice(price);

	// Set text to sanitized number.
	this->ActiveSellSelection->SetText(std::to_string(this->ActiveItemData->GetSellPrice()));
	this->ActiveSellSelection->ReverseColor();
}

void SetPrices::SEvent_ZeroItemSell() {
	// Set sell price entered. Invalid input will convert to 0.
	this->ActiveItemData->SetSellPrice(0);

	// Set text to sanitized number.
	this->ActiveSellSelection->SetText(std::to_string(this->ActiveItemData->GetSellPrice()));
	this->ActiveSellSelection->ReverseColor();
}

void SetPrices::SEvent_ItemSellEntry() {
	// Move into sell price entry state
	this->EventFlags.MainSelection = false;
	this->EventFlags.EnterItemPrice = true;
	
	// Reverse colors
	this->Images.SelectionBG->ReverseColor();
	this->ActiveSellSelection->ReverseColor();

	// Flush buffered text input
	SDL_PumpEvents();

	// Enable text entry for item sell price
	SDL_StartTextInput();
	this->ActiveSellSelection->SetActive(true);
}

void SetPrices::SEvent_EndSellEntry() {
	// Get price entered. Invalid input will convert to 0.
	int price = std::atoi(this->ActiveSellSelection->GetText()->c_str());
	if (price < 0)
		price = 0;

	// Set sell price entered. Invalid input will convert to 0.
	this->ActiveItemData->SetSellPrice(price);

	// Set text to sanitized number.
	this->ActiveSellSelection->SetText(std::to_string(this->ActiveItemData->GetSellPrice()));

	// Stop text entry
	SDL_StopTextInput();
	this->ActiveSellSelection->SetActive(false);

	// Reverse colors
	this->Images.SelectionBG->ReverseColor();
	this->ActiveSellSelection->ReverseColor();
	
	// Return to main selection
	this->EventFlags.EnterItemPrice = false;
	//this->TextObjects.EnterPrice->SetVisible(false);
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


void SetPrices::SEvent_OpenGuide() {
	// Set guide text
	this->TextObjects.GuideText->SetText(GetItemGuideDesc(this->ActiveItemData->GetName()));

	// Move into guide display state
	this->EventFlags.MainSelection = false;
	this->EventFlags.ShowGuide = true;

	// Show Guide text
	this->SEvent_HideSetPricesText();
	this->SEvent_ShowGuide();

	// Flush buffered text input
	SDL_PumpEvents();
}

void SetPrices::SEvent_ExitGuide() {
	// Switch image display to main selection	
	this->SEvent_HideGuide();
	this->SEvent_ShowSetPricesText();

	// Move into main selection state
	this->EventFlags.ShowGuide = false;
	this->EventFlags.MainSelection = true;
}

void SetPrices::SEvent_OpenShop() {
	// If all prices are not set, stay in shop.
	for (std::vector<ItemData*>::iterator it = this->SellData.begin(); it != this->SellData.end(); it++) {
		if ((*it)->GetSellPrice() <= 0) {
			this->TextObjects.ErrSetPrice->SetVisible(true);
			this->EventTimers.ErrorText->StartEventTimer();
			Mix_PlayChannel(1, Assets::Instance()->sounds.Buzz, 0);
			return;
		}
	}
	Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);

	// Set player sell prices
	for (std::vector<ItemData*>::iterator it = this->SellData.begin(); it != this->SellData.end(); it++)
		this->mPlayerData->GetInventoryItem((*it)->GetName())->SetSellPrice((*it)->GetSellPrice());
	
	// Stop Music
	Mix_HaltMusic();

	// Leave Set Price screen
	this->mManager->StartScene(Scene_DaySales);
}

void SetPrices::SEvent_ExitToTitle() {
	// Halt music
	Mix_HaltMusic();

	// Return to title screen
	this->mManager->StartScene(Scene_TitleScreen);
}

void SetPrices::SEvent_ShowEscapeScreen() {
	this->PauseTimers();
	Mix_PauseMusic();

	if (this->EventFlags.EnterItemPrice) {
		SDL_StopTextInput();
		this->ActiveSellSelection->SetActive(false);
	}

	this->EscapeScreenVisible = true;
	this->ShowEscapeText();
}

void SetPrices::SEvent_HideEscapeScreen() {
	this->UnpauseTimers();
	if (Mix_PausedMusic())
		Mix_ResumeMusic();

	if (this->EventFlags.EnterItemPrice) {
		// Flush buffered text input
		SDL_PumpEvents();

		SDL_StartTextInput();
		this->ActiveSellSelection->SetActive(true);
	}

	this->EscapeScreenVisible = false;
	this->HideEscapeText();
}

void SetPrices::SEvent_HideErrorText() {
	this->TextObjects.ErrSetPrice->SetVisible(false);
}