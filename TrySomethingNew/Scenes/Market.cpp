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

	// Image objects

	// Text objects
	// Market Title
	this->TextObjects.MarketTitle = this->AddText("DER  MARKT", 105, 9);
	// Items
	this->TextObjects.BierName = this->AddText("1) BIER", 7, 27);
	this->TextObjects.BierCost = this->AddText("", 7, 36);
	this->TextObjects.BockwurstName = this->AddText("2) BOCKWURST", 7, 45);
	this->TextObjects.BockwurstCost = this->AddText("", 7, 54);
	this->TextObjects.MettigelName = this->AddText("3) METTIGEL", 7, 63);
	this->TextObjects.MettigelCost = this->AddText("", 7, 72);
	this->TextObjects.CurrywurstName = this->AddText("4) CURRYWURST", 7, 81);
	this->TextObjects.CurrywurstCost = this->AddText("", 7, 90);
	this->TextObjects.StreetSheetName = this->AddText("5) DIE STRAßENZEITUNG", 7, 99);
	this->TextObjects.StreetSheetCost = this->AddText("", 7, 108);
	this->TextObjects.USADAYName = this->AddText("6) USA DAY", 7, 117);
	this->TextObjects.USADAYCost = this->AddText("", 7, 126);
	// Ads
	this->TextObjects.SignName = this->AddText("7) WOOD SIGN", 140, 27);
	this->TextObjects.SignCost = this->AddText("", 140, 36);
	this->TextObjects.PosterName = this->AddText("8) WALL POSTER", 140, 45);
	this->TextObjects.PosterCost = this->AddText("", 140, 54);
	this->TextObjects.NewsAdName = this->AddText("9) NEWSPAPER AD", 140, 63);
	this->TextObjects.NewsAdCost = this->AddText("", 140, 72);
	// Options
	this->TextObjects.BuyOption = this->AddText("B)UY", 7, 180);
	this->TextObjects.ForecastOption = this->AddText("F)ORECAST", 105, 180);
	this->TextObjects.LeaveOption = this->AddText("L)EAVE", 231, 180);
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
	// Display date text
	this->TextObjects.MarketTitle->SetVisible(true);
	// Items
	this->TextObjects.BierName->SetVisible(true);
	this->TextObjects.BierCost->SetVisible(true);
	this->TextObjects.BockwurstName->SetVisible(true);
	this->TextObjects.BockwurstCost->SetVisible(true);
	this->TextObjects.MettigelName->SetVisible(true);
	this->TextObjects.MettigelCost->SetVisible(true);
	this->TextObjects.CurrywurstName->SetVisible(true);
	this->TextObjects.CurrywurstCost->SetVisible(true);
	this->TextObjects.StreetSheetName->SetVisible(true);
	this->TextObjects.StreetSheetCost->SetVisible(true);
	this->TextObjects.USADAYName->SetVisible(true);
	this->TextObjects.USADAYCost->SetVisible(true);
	// Ads
	this->TextObjects.SignName->SetVisible(true);
	this->TextObjects.SignCost->SetVisible(true);
	this->TextObjects.PosterName->SetVisible(true);
	this->TextObjects.PosterCost->SetVisible(true);
	this->TextObjects.NewsAdName->SetVisible(true);
	this->TextObjects.NewsAdCost->SetVisible(true);
	// Options
	this->TextObjects.BuyOption->SetVisible(true);
	this->TextObjects.ForecastOption->SetVisible(true);
	this->TextObjects.LeaveOption->SetVisible(true);
}

void Market::SEvent_HideMarketText() {
	// Display date text
	this->TextObjects.MarketTitle->SetVisible(false);
	// Items
	this->TextObjects.BierName->SetVisible(false);
	this->TextObjects.BierCost->SetVisible(false);
	this->TextObjects.BockwurstName->SetVisible(false);
	this->TextObjects.BockwurstCost->SetVisible(false);
	this->TextObjects.MettigelName->SetVisible(false);
	this->TextObjects.MettigelCost->SetVisible(false);
	this->TextObjects.CurrywurstName->SetVisible(false);
	this->TextObjects.CurrywurstCost->SetVisible(false);
	this->TextObjects.StreetSheetName->SetVisible(false);
	this->TextObjects.StreetSheetCost->SetVisible(false);
	this->TextObjects.USADAYName->SetVisible(false);
	this->TextObjects.USADAYCost->SetVisible(false);
	// Ads
	this->TextObjects.SignName->SetVisible(false);
	this->TextObjects.SignCost->SetVisible(false);
	this->TextObjects.PosterName->SetVisible(false);
	this->TextObjects.PosterCost->SetVisible(false);
	this->TextObjects.NewsAdName->SetVisible(false);
	this->TextObjects.NewsAdCost->SetVisible(false);
	// Options
	this->TextObjects.BuyOption->SetVisible(false);
	this->TextObjects.ForecastOption->SetVisible(false);
	this->TextObjects.LeaveOption->SetVisible(false);
}