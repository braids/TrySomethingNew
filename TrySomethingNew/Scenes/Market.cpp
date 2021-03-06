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
	this->EventFlags.EnterItemQty = false;
	this->EventFlags.ShowForecast = false;
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

	// Selection
	this->Images.SelectionBG = this->AddMarketImage(&Assets::Instance()->images.SelectionRect, 0, 0);

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
		ImageData* textBox = this->AddMarketTextBox(3, 203, y);
		textBox->SetText(std::to_string(this->BuyData[i]->GetBoughtQuantity()));
		
		// Add purchase subtotal text
		this->AddSubTotalText(std::to_string(this->BuyData[i]->GetBoughtQuantity() * this->BuyData[i]->GetBuyPrice()), 245, y);
	}
	// Total
	this->AddMarketText("MONEY:", 182, 144);
	this->TextObjects.PlayerMoneyAmount = this->AddMarketText("0", 231, 144);
	this->AddMarketText("- TOTAL:", 168, 153);
	this->TextObjects.TotalAmount = this->AddMarketText("0", 231, 153);
	this->AddMarketText("=", 217, 162);
	this->TextObjects.MoneySubTotalAmount = this->AddMarketText("0", 231, 162);
	// Options
	this->AddMarketText("F)ORECAST", 7, 180);
	this->AddMarketText("G)UIDE", 98, 180);
	this->AddMarketText("L)EAVE", 168, 180);
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

	// Clear player's sales totals
	this->mPlayerData->SetMorningState();

	// Add copy of player's inventory.
	for (std::vector<ItemData*>::iterator it = this->mPlayerData->GetInventory()->begin(); it != this->mPlayerData->GetInventory()->end(); it++)
		this->BuyData.push_back(new ItemData(**it));
	
	// Load Event Timers
	this->LoadEventTimers();

	// Load Images and Text Images
	this->LoadImagesText();
	
	// Initialize selection
	this->InitSelection();

	// Update total amount
	this->UpdateTotal();

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
				this->SEvent_DecreaseItemQty();
			}
			if (Event->key.keysym.sym == SDLK_RIGHT) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->SEvent_IncreaseItemQty();
			}
			if (Event->key.keysym.sym == SDLK_DELETE || Event->key.keysym.sym == SDLK_BACKSPACE) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->SEvent_ZeroItemQty();
			}
			if (Event->key.keysym.sym == SDLK_RETURN || Event->key.keysym.sym == SDLK_KP_ENTER) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				SEvent_ItemQtyEntry();
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
			if (Event->key.keysym.sym == SDLK_l) {
				this->SEvent_Leave();
			}
			if (Event->key.keysym.sym == SDLK_s) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->SEvent_Save();
			}
		}

		//// Buy
		// Enterting item quantity
		if (this->EventFlags.EnterItemQty) {
			if(Event->key.keysym.sym == SDLK_BACKSPACE)
				this->ActiveBuySelection->DeleteText();
			if (Event->key.keysym.sym == SDLK_RETURN || Event->key.keysym.sym == SDLK_KP_ENTER) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->SEvent_EndItemQtyEntry();
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

ImageData * Market::AddMarketImage(Assets::Image* image, int _x, int _y) {
	ImageData* imageData = this->AddImage(image, _x, _y);
	this->MarketText.push_back(imageData);
	return imageData;
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

void Market::InitSelection() {
	// Set first item as initial selected item
	this->SelectedItem = 0;
	
	// Update selection data
	this->UpdateSelection();
}

void Market::UpdateSelection() {
	// If a text box is already active, reset the color
	if (this->ActiveItemData != nullptr)
		this->ActiveBuySelection->ReverseColor();

	// Set selection data
	this->ActiveBuySelection = this->ItemTextBoxObjects[this->SelectedItem];
	this->ActiveBuySubTotal = this->SubTotalText[this->SelectedItem];
	this->ActiveItemData = this->BuyData[this->SelectedItem];

	// Move Selection BG behind current selection
	this->Images.SelectionBG->SetDrawRectXY(this->ActiveBuySelection->GetDrawRect()->x, this->ActiveBuySelection->GetDrawRect()->y - 1);

	// Reverse text color
	this->ActiveBuySelection->ReverseColor();
}

void Market::UpdateTotal() {
	this->BuyTotal = 0;

	// Get all buy subtotals
	std::vector<ItemData*>::iterator it = this->BuyData.begin();
	for (; it != this->BuyData.end(); it++)
		this->BuyTotal += (*it)->GetBuyPrice() * (*it)->GetBoughtQuantity();

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

void Market::SEvent_SelectionUp() {
	// Subtract one from SelectedItem. If it's lower than 0, make it last item.
	if (--this->SelectedItem < 0)
		this->SelectedItem = this->BuyData.size() - 1;
	
	// Update selection data
	this->UpdateSelection();
}

void Market::SEvent_SelectionDown() {
	// Add one to SelectedItem. If it's higher than last item, make it 0.
	if (++this->SelectedItem >= (int) this->BuyData.size())
		this->SelectedItem = 0;

	// Update selection data
	this->UpdateSelection();
}

void Market::SEvent_IncreaseItemQty() {
	int newQty = this->ActiveItemData->GetQuantity();
	
	// If increased quantity is greater than the max quantity value, bail
	if (++newQty > 999)
		return;

	// Increase quantity
	this->ActiveItemData->SetQuantity(newQty);
	this->ActiveItemData->SetBoughtQuantity(this->ActiveItemData->GetQuantity());

	// Set text to sanitized number.
	this->ActiveBuySelection->SetText(std::to_string(this->ActiveItemData->GetQuantity()));
	this->ActiveBuySelection->ReverseColor();

	// Set subtotal price
	int subtotal = this->ActiveItemData->GetBuyPrice() * this->ActiveItemData->GetQuantity();
	this->ActiveBuySubTotal->SetText(std::to_string(subtotal));

	// Update grand total
	this->UpdateTotal();
}

void Market::SEvent_DecreaseItemQty() {
	int newQty = this->ActiveItemData->GetQuantity();

	// If decreased quantity is less than 0, bail
	if (--newQty < 0)
		return;

	// Increase quantity
	this->ActiveItemData->SetQuantity(newQty);
	this->ActiveItemData->SetBoughtQuantity(this->ActiveItemData->GetQuantity());

	// Set text to sanitized number.
	this->ActiveBuySelection->SetText(std::to_string(this->ActiveItemData->GetQuantity()));
	this->ActiveBuySelection->ReverseColor();

	// Set subtotal price
	int subtotal = this->ActiveItemData->GetBuyPrice() * this->ActiveItemData->GetQuantity();
	this->ActiveBuySubTotal->SetText(std::to_string(subtotal));

	// Update grand total
	this->UpdateTotal();
}

void Market::SEvent_ZeroItemQty() {
	// Zero out quantity
	this->ActiveItemData->SetQuantity(0);
	this->ActiveItemData->SetBoughtQuantity(this->ActiveItemData->GetQuantity());

	// Set text to sanitized number.
	this->ActiveBuySelection->SetText(std::to_string(this->ActiveItemData->GetQuantity()));
	this->ActiveBuySelection->ReverseColor();

	// Set subtotal price
	int subtotal = this->ActiveItemData->GetBuyPrice() * this->ActiveItemData->GetQuantity();
	this->ActiveBuySubTotal->SetText(std::to_string(subtotal));

	// Update grand total
	this->UpdateTotal();
}

void Market::SEvent_ItemQtyEntry() {
	// Move into quantity entry state
	this->EventFlags.MainSelection = false;
	this->EventFlags.EnterItemQty = true;
	// Reverse colors
	this->Images.SelectionBG->ReverseColor();
	this->ActiveBuySelection->ReverseColor();

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
	
	// Stop text entry
	SDL_StopTextInput();
	this->ActiveBuySelection->SetActive(false);

	// Reverse colors
	this->Images.SelectionBG->ReverseColor();
	this->ActiveBuySelection->ReverseColor();

	// Set main selection state
	this->EventFlags.EnterItemQty = false;
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

void Market::SEvent_OpenGuide() {
	// Set guide text
	this->TextObjects.GuideText->SetText(GetItemGuideDesc(this->ActiveItemData->GetName()));
	
	// Move into guide display state
	this->EventFlags.MainSelection = false;
	this->EventFlags.ShowGuide = true;
	
	// Show Guide text
	this->SEvent_HideMarketText();
	this->SEvent_ShowGuide();

	// Flush buffered text input
	SDL_PumpEvents();
}

void Market::SEvent_ExitGuide() {
	// Switch image display to main selection	
	this->SEvent_HideGuide();
	this->SEvent_ShowMarketText();

	// Move into main selection state
	this->EventFlags.ShowGuide = false;
	this->EventFlags.MainSelection = true;
}

void Market::SEvent_Leave() {
	////Check if product bought
	int productBought = 0;
	// Check all non-ad items for at least one bought item
	for (std::vector<ItemData*>::iterator it = this->BuyData.begin(); it != this->BuyData.end(); it++) {
		if ((*it)->GetBoughtQuantity() > 0 && (*it)->GetType() != ItemType::ItemType_Ad)
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
			this->mPlayerData->GetInventoryItem((*it)->GetName())->SetBoughtQuantity((*it)->GetBoughtQuantity());
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
