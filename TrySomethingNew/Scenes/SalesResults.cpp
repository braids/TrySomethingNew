#include <algorithm>
#include <string>
#include <vector>
#include "Assets.h"
#include "Data\ItemData.h"
#include "Data\PlayerData.h"
#include "Graphics.h"
#include "Scenes\Scene.h"
#include "SceneManager.h"

SalesResults::SalesResults() {
	// Set current scene name
	this->SetSceneName(Scene_SalesResults);
}

//// Scene funcs
void SalesResults::ResetFlags() {
	// Set flags to false
	this->EventFlags.ExitToTitleScreen = false;
	this->EventFlags.ExitResults = false;
	this->EventFlags.GameOver = false;
}

void SalesResults::LoadImagesText() {
	// Clear any existing drawn text.
	this->mImages.clear();
	this->SalesResultsText.clear();

	//// Image objects

	//// Text objects
	for (int i = 0; i < (int) this->SellItems.size(); i++) {
		int menuNum = i + 1;
		int y = 36 + (i * 9);
		// Add menu number and name
		std::string menuName = std::to_string(menuNum) + ") " + GetItemString(this->SellItems[i]->GetName());
		this->AddSalesResultsText(menuName, 7, y);
		// Display # bought
		this->AddSalesResultsText(std::to_string(this->SellItems[i]->GetBoughtQuantity()), 161, y);
		// Display # sold
		this->AddSalesResultsText(std::to_string(this->SellItems[i]->GetSalesTotal()), 203, y);
		// Display money made from sales
		this->AddSalesResultsText("DM" + std::to_string(this->SellItems[i]->GetSalesTotal() * this->SellItems[i]->GetSellPrice()), 231, y);
	}

	// Title
	this->AddSalesResultsText("EOD SALES RESULTS", 84, 9);
	// Headers
	this->AddSalesResultsText("-INVENTORY-", 21, 27);
	this->AddSalesResultsText("STOCK", 161, 27);
	this->AddSalesResultsText("SOLD", 203, 27);
	this->AddSalesResultsText("SALES", 238, 27);
	// Day
	this->AddSalesResultsText("DAY:", 7, 164);
	this->AddSalesResultsText(std::to_string(this->mPlayerData->GetDay()), 42, 164);
	// Total
	this->AddSalesResultsText("MONEY:", 140, 164);
	this->TextObjects.PlayerMoneyAmount = this->AddSalesResultsText(std::to_string(this->mPlayerData->GetMoney()), 189, 164);
	// Press Return
	this->AddSalesResultsText("- PRESS RETURN -", 84, 180);

	for (std::vector<ImageData*>::iterator it = this->mImages.begin(); it != this->mImages.end(); it++)
		(*it)->SetVisible(false);
}

void SalesResults::SceneStart() {
	// Play Market music
	Mix_HaltMusic();
		Mix_PlayMusic(Assets::Instance()->music.JazzyMusic, -1);
	// Reset Flags
	this->ResetFlags();

	// Get player inventory
	this->GetCurrentPlayerInventory();

	// Load Images and Text Images
	this->LoadImagesText();

	// Display main market text
	this->SEvent_ShowSalesResultsText();
}

void SalesResults::HandleEvent(SDL_Event * Event) {
	switch (Event->type) {
	case SDL_KEYDOWN:
		//// Exit event
		if (Event->key.keysym.sym == SDLK_ESCAPE) {
			Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
			this->EventFlags.ExitToTitleScreen = true;
		}
		if (Event->key.keysym.sym == SDLK_RETURN) {
			Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
			(this->EventFlags.GameOver) ? this->SEvent_GameOver_TitleScreen() : this->SEvent_LoseCheck();
		}
		break;

	case SDL_KEYUP:
		break;

	default:
		break;
	}
}

void SalesResults::Update(Uint32 timeStep) {
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

void SalesResults::Render() {
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

//// SetPrices funcs
ImageData* SalesResults::AddSalesResultsText(std::string _text, int _x, int _y) {
	ImageData* textImage = this->AddText(_text, _x, _y);
	this->SalesResultsText.push_back(textImage);
	return textImage;
}

void SalesResults::GetCurrentPlayerInventory() {
	// Clear current sell item list
	this->SellItems.clear();
	// Get all player items with inventory greater than 0
	std::for_each(
		this->mPlayerData->GetInventory()->begin(),
		this->mPlayerData->GetInventory()->end(),
		[this](ItemData* &_item) { if (_item->GetBoughtQuantity() > 0 && _item->GetType() != ItemType::ItemType_Ad) this->SellItems.push_back(_item); });
}

//// Scene Events
void SalesResults::SEvent_ShowSalesResultsText() {
	for (std::vector<ImageData*>::iterator it = this->SalesResultsText.begin(); it != this->SalesResultsText.end(); it++)
		(*it)->SetVisible(true);
}

void SalesResults::SEvent_HideSalesResultsText() {
	for (std::vector<ImageData*>::iterator it = this->SalesResultsText.begin(); it != this->SalesResultsText.end(); it++)
		(*it)->SetVisible(false);
}

void SalesResults::SEvent_LoseCheck() {
	if (this->mPlayerData->GetMoney() <= 1) {
		// Hide on-screen text
		this->SEvent_HideSalesResultsText();

		this->AddText("** DER BRUNNEN IST TROCKEN GELAUFEN **", 7, 9);
		this->AddText("    WITH LITTLE TO NO DEUTSCHE MARKS LEFT TO YOUR NAME, YOU MUST CLOSE UP SHOP. AFTER SELLING YOUR SHACK THERE  IS ENOUGH MONEY LEFT TO RETURN HOME TO THE STATES. MAYBE IT'S FINALLY TIME TO TAKE THOSE BUSINESS CLASSES YOU ALWAYS WANTED.\n\n\n\n           AH, CAPITALISM!", 7, 27);
		this->AddText("- PRESS RETURN -", 84, 180);

		this->EventFlags.GameOver = true;
	}
	else {
		this->SEvent_NextDay();
	}
}

void SalesResults::SEvent_GameOver_TitleScreen() {
	// Start title screen scene
	this->mManager->StartScene(Scene_TitleScreen);
}

void SalesResults::SEvent_NextDay() {
	// Set Forecast
	this->mPlayerData->GenerateForecast();
	
	// Set player day
	this->mPlayerData->SetDay(this->mPlayerData->GetDay() + 1);
	
	// Start market scene
	this->mManager->StartScene(Scene_Market);
}