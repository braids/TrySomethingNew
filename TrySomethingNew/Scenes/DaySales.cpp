#include <algorithm>
#include <string>
#include <vector>
#include <SDL2\SDL.h>
#include <SDL2\SDL_mixer.h>
#include "Assets.h"
#include "Data\CustomerData.h"
#include "Data\ItemData.h"
#include "Data\PlayerData.h"
#include "GameObjects\GameObject.h"
#include "Graphics.h"
#include "Scenes\DaySales.h"
#include "Scenes\Scene.h"
#include "SceneManager.h"
#include "Timer.h"

//// Scene Ctor
DaySales::DaySales() {
	// Set current scene name
	this->SetSceneName(Scene_DaySales);
}

//// Scene funcs
void DaySales::ResetFlags() {
	// Set flags to false
	this->EventFlags.ExitToTitleScreen = false;
	this->EventFlags.Simulation = false;
}

void DaySales::LoadEventTimers() {
	this->EventTimers.CustomerSpawn = this->AddEventTimer(new EventTimer(std::bind(&DaySales::SEvent_SpawnCustomer, this), this->CustomerSpawnInterval));
	this->EventTimers.CustomerSpawn->SetLoop(true);
	this->EventTimers.DayRuntime1 = this->AddEventTimer(new EventTimer(std::bind(&DaySales::SEvent_DayRuntime2, this), DaySegmentLength));
	this->EventTimers.DayRuntime2 = this->AddEventTimer(new EventTimer(std::bind(&DaySales::SEvent_DayRuntime3, this), DaySegmentLength));
	this->EventTimers.DayRuntime3 = this->AddEventTimer(new EventTimer(std::bind(&DaySales::SEvent_DayRuntimeEnd, this), DaySegmentLength));
}

void DaySales::LoadImagesText() {
	// Clear any existing drawn text.
	this->mImages.clear();
	this->DaySalesText.clear();

	// Shack images
	this->Images.Shack1 = new ImageData();
	this->Images.Shack2 = new ImageData();
	this->Images.Shack3 = new ImageData();
	this->Images.Shack1->SetImage(&Assets::Instance()->images.Shack1);
	this->Images.Shack2->SetImage(&Assets::Instance()->images.Shack2);
	this->Images.Shack3->SetImage(&Assets::Instance()->images.Shack3);
	this->Images.Shack1->SetDrawRectY(46);
	this->Images.Shack2->SetDrawRectY(46);
	this->Images.Shack3->SetDrawRectY(46);
	this->mImages.push_back(this->Images.Shack1);
	this->mImages.push_back(this->Images.Shack2);
	this->mImages.push_back(this->Images.Shack3);

	// Text
	int shopX = 140 - ((this->mPlayerData->GetName().length() * 7) / 2);
	this->TextObjects.ShopName = this->AddDaySalesText(this->mPlayerData->GetName(), shopX, 21);
	this->TextObjects.DayText = this->AddDaySalesText("DAY:", 7, 164);
	this->TextObjects.DayNum = this->AddDaySalesText(std::to_string(this->mPlayerData->GetDay()), 42, 164);
	this->TextObjects.MoneyText = this->AddDaySalesText("MONEY:", 140, 164);
	this->TextObjects.MoneyAmt = this->AddDaySalesText(std::to_string(this->Money + this->mPlayerData->GetMoney()), 189, 164);

	// Add customer images
	std::vector<CustomerObject*>::iterator it = this->CustomerObjects.begin();
	for (; it != this->CustomerObjects.end(); it++) {
		this->CustomerImages.push_back(this->AddImage((*it)->GetImageData()));
	}

	// Load escape screen text
	this->AddEscapeText(this);

	// Disable all image/text visibility
	for (std::vector<ImageData*>::iterator it = this->mImages.begin(); it != this->mImages.end(); it++)
		(*it)->SetVisible(false);
}

void DaySales::SceneStart() {
	// Play DaySales music
	Mix_HaltMusic();
	Mix_PlayMusic(Assets::Instance()->music.DaySalesMusic, 0);

	// Reset Flags
	this->ResetFlags();

	/// Set initial vars
	this->EventFlags.Simulation = true;
	this->Money = 0;
	this->CustomerSpawnInterval = 0;

	// Get player inventory
	this->GetCurrentPlayerInventory();

	// Generate customers
	this->GenerateCustomers();

	// Load Images and Text Images
	this->LoadImagesText();
	
	// Load event timers
	this->LoadEventTimers();

	// Set active shack image
	this->Images.Shack1->SetVisible(true);
	this->Images.Shack = this->Images.Shack1;

	// Start simulation
	this->EventTimers.CustomerSpawn->StartEventTimer();
	this->EventTimers.DayRuntime1->StartEventTimer();
	this->SEvent_ShowDaySalesText();
}

void DaySales::HandleEvent(SDL_Event * Event) {
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

		break;

	case SDL_KEYUP:
		break;

	default:
		break;
	}
}

void DaySales::Update(Uint32 timeStep) {
	// Update timers
	this->UpdateEventTimers();

	// Update customers
	if (!EscapeScreenVisible) {
		std::vector<CustomerObject*>::iterator it = this->CustomerObjects.begin();

		for (; it != this->CustomerObjects.end(); it++) {
			// Check if customers are shopping and havne't purchased yet
			if (!(*it)->HasPurchased() && (*it)->IsShopping()) 
				this->GetPurchase((*it));

			(*it)->Update(timeStep);
		}
	}
}

void DaySales::Render() {
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

void DaySales::Cleanup() {
	// Clear vectors
	this->mImages.clear();
	this->DaySalesText.clear();
	this->SellItems.clear();
	this->CustomerImages.clear();
	this->CustomerObjects.clear();
	this->EscapeImagesText.clear();

	// Stop timers
	this->EventTimers.CustomerSpawn->stop();
	this->EventTimers.DayRuntime1->stop();
	this->EventTimers.DayRuntime2->stop();
	this->EventTimers.DayRuntime3->stop();
}

ImageData* DaySales::AddDaySalesText(std::string _text, int _x, int _y) {
	ImageData* textImage = this->AddText(_text, _x, _y);
	this->DaySalesText.push_back(textImage);
	return textImage;
}

//// DaySales funcs
void DaySales::GetCurrentPlayerInventory() {
	// Clear current sell item list
	this->SellItems.clear();

	// Get all player items with inventory greater than 0
	std::for_each(
		this->mPlayerData->GetInventory()->begin(),
		this->mPlayerData->GetInventory()->end(),
		[this](ItemData* &_item) { if (_item->GetQuantity() > 0 && _item->GetType() != ItemType::ItemType_Ad) this->SellItems.push_back(_item); });
}

void DaySales::GenerateCustomers() {
	// Clear current customer vector
	this->CustomerImages.clear();
	this->CustomerObjects.clear();

	// Get amount of advertising
	int signs = this->mPlayerData->GetInventoryItem(ItemName::Item_Sign)->GetQuantity();
	int posters = this->mPlayerData->GetInventoryItem(ItemName::Item_Poster)->GetQuantity();
	int newsAds = this->mPlayerData->GetInventoryItem(ItemName::Item_NewsAd)->GetQuantity();

	// Get weather modifier
	double weatherMod = 1.0;
	switch(this->mPlayerData->GetWeatherForecast()) {
	case ForecastWeather::Weather_Cloudy: 
		weatherMod = 0.8;
		break;
	case ForecastWeather::Weather_Rainy:
		weatherMod = 0.4;
		break;
	default:
		break;
	}

	// Determine number of customers adjusted for advertising
	int numCustomers = rand() % (10 + signs + (posters * 4) + (newsAds * 8)) + (1 + signs + (posters * 3) + (newsAds * 6));
	// Mod numCustomers with weatherMod value
	numCustomers = (int)((double)numCustomers * weatherMod);
	// If numCustomers ends up at 0 (or less?), spawn at least one customer.
	if (numCustomers <= 0) numCustomers = 1;

	// Get customer spawn rate
	this->CustomerSpawnInterval = ((this->DaySegmentLength * 3) - 1000) / numCustomers;

	// Create n customers 
	for (int i = 0; i < numCustomers; i++) {
		// Create new customer based on day's event.
		this->CustomerObjects.push_back( new CustomerObject(this->mPlayerData->GetEventForecast()) );
	}

	this->CustomerSpawnIter = this->CustomerObjects.begin();
}

void DaySales::GetPurchase(CustomerObject* _customer) {
	// Set customer as purchased
	_customer->SetPurchased(true);

	// Customer makes a purchase from available items.
	std::vector<ItemName>* purchaseList = _customer->GetData()->PurchaseList(this->SellItems);

	// Declare purchased item pointer
	ItemData* purchasedItem;

	// For each item purchased
	for (std::vector<ItemName>::iterator it = purchaseList->begin(); it != purchaseList->end(); it++) {
		// Get purchased item from sales list
		purchasedItem = GetItemFromVector((*it), &this->SellItems);
		// Reduce item quantity by 1
		purchasedItem->SubQuantity(1);
		// Add sell price to money amount
		this->Money += purchasedItem->GetSellPrice();
		// Add sale to item's sales total
		purchasedItem->AddSalesTotal(1);
		// Set money total text
		this->TextObjects.MoneyAmt->SetText(std::to_string(this->Money + this->mPlayerData->GetMoney()));
		// Play a noise, you earned it
		Mix_PlayChannel(3, Assets::Instance()->sounds.Buy, 0);
	}
}

void DaySales::SEvent_ShowDaySalesText() {
	for (std::vector<ImageData*>::iterator it = this->DaySalesText.begin(); it != this->DaySalesText.end(); it++)
		(*it)->SetVisible(true);
}

void DaySales::SEvent_SpawnCustomer() {
	if (this->CustomerSpawnIter != this->CustomerObjects.end()) {
		(*this->CustomerSpawnIter)->SetActive(true);
		this->CustomerSpawnIter++;
	}
}

void DaySales::SEvent_DayRuntime2() {
	this->Images.Shack1->SetVisible(false);
	this->Images.Shack2->SetVisible(true);
	this->Images.Shack = this->Images.Shack2;
	this->EventTimers.DayRuntime2->StartEventTimer();
}

void DaySales::SEvent_DayRuntime3() {
	this->Images.Shack2->SetVisible(false);
	this->Images.Shack3->SetVisible(true);
	this->Images.Shack = this->Images.Shack3;
	this->EventTimers.DayRuntime3->StartEventTimer();
}

void DaySales::SEvent_DayRuntimeEnd() {
	// Stop customer timer just in case
	this->EventTimers.CustomerSpawn->stop();

	// Set player inventory
	for (std::vector<ItemData*>::iterator it = this->SellItems.begin(); it != this->SellItems.end(); it++) {
		ItemData* item = this->mPlayerData->GetInventoryItem((*it)->GetName());
		*item = **it;
	}

	// Stop music
	Mix_HaltMusic();

	// Set player money
	this->mPlayerData->SetMoney(this->mPlayerData->GetMoney() + this->Money);
		
	// Leave Day Sales screen
	this->mManager->StartScene(Scene_SalesResults);
}

void DaySales::SEvent_ExitToTitle() {
	// Halt music
	Mix_HaltMusic();

	// Return to title screen
	this->mManager->StartScene(Scene_TitleScreen);
}

void DaySales::SEvent_ShowEscapeScreen() {
	this->PauseTimers();
	Mix_PauseMusic();

	for(std::vector<CustomerObject*>::iterator it = this->CustomerObjects.begin(); it != this->CustomerObjects.end(); it++)
		(*it)->PauseWalkTimer();

	this->EscapeScreenVisible = true;
	this->ShowEscapeText();
}

void DaySales::SEvent_HideEscapeScreen() {
	this->UnpauseTimers();
	if (Mix_PausedMusic())
		Mix_ResumeMusic();

	for (std::vector<CustomerObject*>::iterator it = this->CustomerObjects.begin(); it != this->CustomerObjects.end(); it++)
		(*it)->ResumeWalkTimer();

	this->EscapeScreenVisible = false;
	this->HideEscapeText();
}