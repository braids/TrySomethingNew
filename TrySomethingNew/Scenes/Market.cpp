#include "Assets.h"
#include "Camera.h"
#include "GameObjects\GameObject.h"
#include "Graphics.h"
#include "Scenes\Scene.h"
#include "SceneManager.h"

Market::Market() {
	// Set current scene name
	this->SetSceneName(Scene_Market);
}

void Market::ResetFlags() {
	// Set flags to false
	this->EventFlags.ExitToTitleScreen = false;
}

void Market::LoadGameObjects() {
	this->mGameObjects.clear();
}

void Market::LoadEventTimers() {
	this->mEventTimers.clear();

	// Init event timers
}

void Market::LoadImagesText() {
	// Clear any existing drawn text.
	this->mImages.clear();
	this->MarketText.clear();

	// Image objects

	// Text objects
	// Market Title
	this->TextObjects.MarketTitle = this->AddMarketText("DER MARKT", 112, 9);
	// Items
	this->TextObjects.Foods = this->AddMarketText("-FOODS-", 21, 27);
	this->TextObjects.Cost = this->AddMarketText("COST", 161, 27);
	this->TextObjects.Qty = this->AddMarketText("QTY", 203, 27);
	this->TextObjects.Total = this->AddMarketText("TOTAL", 238, 27);
	this->TextObjects.BierName = this->AddMarketText("1) BIER", 7, 36);
	this->TextObjects.BierCost = this->AddMarketText("DM20", 161, 36);
	this->TextObjects.BockwurstName = this->AddMarketText("2) BOCKWURST", 7, 45);
	this->TextObjects.BockwurstCost = this->AddMarketText("DM20", 161, 45);
	this->TextObjects.MettigelName = this->AddMarketText("3) METTIGEL", 7, 54);
	this->TextObjects.MettigelCost = this->AddMarketText("DM20", 161, 54);
	this->TextObjects.CurrywurstName = this->AddMarketText("4) CURRYWURST", 7, 63);
	this->TextObjects.CurrywurstCost = this->AddMarketText("DM20", 161, 63);
	this->TextObjects.Newspapers = this->AddMarketText("-NEWSPAPERS-", 21, 72);
	this->TextObjects.StreetSheetName = this->AddMarketText("5) DIE STRAßENZEITUNG", 7, 81);
	this->TextObjects.StreetSheetCost = this->AddMarketText("DM20", 161, 81);
	this->TextObjects.USADAYName = this->AddMarketText("6) USA DAY", 7, 90);
	this->TextObjects.USADAYCost = this->AddMarketText("DM20", 161, 90);
	// Ads
	this->TextObjects.Ads = this->AddMarketText("-ADS-", 21, 99);
	this->TextObjects.SignName = this->AddMarketText("7) WOOD SIGN", 7, 108);
	this->TextObjects.SignCost = this->AddMarketText("DM20", 161, 108);
	this->TextObjects.PosterName = this->AddMarketText("8) WALL POSTER", 7, 117);
	this->TextObjects.PosterCost = this->AddMarketText("DM20", 161, 117);
	this->TextObjects.NewsAdName = this->AddMarketText("9) NEWSPAPER AD", 7, 126);
	this->TextObjects.NewsAdCost = this->AddMarketText("DM20", 161, 126);
	// Options
	this->TextObjects.BuyOption = this->AddMarketText("B)UY", 7, 180);
	this->TextObjects.ForecastOption = this->AddMarketText("F)ORECAST", 49, 180);
	this->TextObjects.GuideOption = this->AddMarketText("G)UIDE", 126, 180);
	this->TextObjects.LeaveOption = this->AddMarketText("L)EAVE", 182, 180);
	this->TextObjects.SaveOption = this->AddMarketText("S)AVE", 238, 180);
	// Forecast
	this->TextObjects.WeatherHeader = this->AddText("WEATHER FORECAST:", 105, 9);
	this->TextObjects.WeatherInfo = this->AddText("", 105, 9);
	this->TextObjects.EventHeader = this->AddText("LOCAL EVENTS", 105, 9);
	this->TextObjects.EventInfo = this->AddText("", 105, 9);
	
	for (std::vector<ImageData*>::iterator it = this->mImages.begin(); it != this->mImages.end(); it++)
		(*it)->SetVisible(false);
}

void Market::SceneStart() {
	// Play intro music
	Mix_HaltMusic();
	//Mix_PlayMusic(this->mManager->GetAssets()->music.IntroMusic, -1);

	// Reset Flags
	this->ResetFlags();

	// Start with text entry off
	SDL_StopTextInput();

	// Load Game Objects
	this->LoadGameObjects();
	// Load Event Timers
	this->LoadEventTimers();
	// Load Images and Text Images
	this->LoadImagesText();

	// Display main market text
	this->SEvent_ShowMarketText();
}

void Market::HandleEvent(SDL_Event * Event) {
	switch (Event->type) {
	case SDL_KEYDOWN:
		if (Event->key.keysym.sym == SDLK_ESCAPE) this->EventFlags.ExitToTitleScreen = true;
		break;

	case SDL_KEYUP:
		break;

	case SDL_TEXTINPUT:
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

void Market::SEvent_ShowMarketText() {
	for (std::vector<ImageData*>::iterator it = this->MarketText.begin(); it != this->MarketText.end(); it++)
		(*it)->SetVisible(true);
}

void Market::SEvent_HideMarketText() {
	for (std::vector<ImageData*>::iterator it = this->MarketText.begin(); it != this->MarketText.end(); it++)
		(*it)->SetVisible(false);
}
