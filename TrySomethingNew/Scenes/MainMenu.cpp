#include <fstream>
#include <string>
#include <vector>
#include <SDL2\SDL.h>
#include "Assets.h"
#include "Graphics.h"
#include "Scenes\Scene.h"
#include "Scenes\MainMenu.h"
#include "Scenes\SubScreens\EscapeScreen.h"
#include "SceneManager.h"

MainMenu::MainMenu() {
	// Set current scene name
	this->SetSceneName(Scene_MainMenu);
}

void MainMenu::ResetFlags() {
	this->EventFlags.Credits = false;
}

void MainMenu::LoadImagesText() {
	// Clear any existing drawn text.
	this->mImages.clear();
	this->MainMenuText.clear();
	this->CreditsImagesText.clear();

	// Main Menu
	this->MainMenuText.push_back(this->AddText("N)EW GAME", 105, 63));
	this->MainMenuText.push_back(this->AddText("L)OAD GAME", 105, 81));
	this->MainMenuText.push_back(this->AddText("Q)UIT", 105, 99));
	this->MainMenuText.push_back(this->AddText("W)INDOW MODE", 98, 171));
	this->MainMenuText.push_back(this->AddText("C)REDITS", 7, 171));

	// Credits
	this->CreditsImagesText.push_back(this->AddText("* * * KREDITE * * *", 77, 9));
	this->CreditsImagesText.push_back(this->AddText("CODE/ART/MUSIC:", 91, 27));
	this->CreditsImagesText.push_back(this->AddText("DIETING HIPPO", 98, 36));
	this->CreditsImagesText.push_back(this->AddText("DIETINGHIPPO.ITCH.IO", 70, 45));
	this->CreditsImagesText.push_back(this->AddText("ON TWITTER:", 105, 63));
	this->CreditsImagesText.push_back(this->AddText("@DIETINGHIPPO", 98, 72));
	this->CreditsImagesText.push_back(this->AddText("MADE FOR AWFUL WINTER JAM 2018", 35, 90));
	this->CreditsImagesText.push_back(this->AddText("- PRESS RETURN -", 84, 180));

	// JamLogo
	this->CreditsImagesText.push_back(this->AddImage(&Assets::Instance()->images.JamLogo, 14, 105));

	// ThemeLogo
	this->CreditsImagesText.push_back(this->AddImage(&Assets::Instance()->images.ThemeLogo, 147, 105));

	// Load escape screen text
	this->AddEscapeText(this);

	// Hide all images
	for (std::vector<ImageData*>::iterator it = this->mImages.begin(); it != this->mImages.end(); it++)
		(*it)->SetVisible(false);
}

void MainMenu::SceneStart() {
	this->ResetFlags();

	this->LoadImagesText();

	this->SEvent_ShowMainMenu();
}

void MainMenu::HandleEvent(SDL_Event* Event) {
	switch (Event->type) {
	case SDL_KEYDOWN:
		if (this->EscapeScreenVisible) {
			if (Event->key.keysym.sym == SDLK_y) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->SEvent_Quit();
			}
			if (Event->key.keysym.sym == SDLK_n) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->SEvent_HideEscapeScreen();
			}

			break;
		}

		if (this->EventFlags.Credits) {
			if (Event->key.keysym.sym == SDLK_RETURN) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->SEvent_ShowMainMenu();
			}
			if (Event->key.keysym.sym == SDLK_ESCAPE) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->SEvent_ShowEscapeScreen();
			}
			break;
		}

		// Return to title screen
		if (Event->key.keysym.sym == SDLK_ESCAPE) {
			Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
			this->SEvent_ExitToTitle();
		}
		
		else {
			// Start new game
			if (Event->key.keysym.sym == SDLK_n && Event->key.repeat == 0) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->SEvent_NewGame();
			}
			// Load saved game
			if (Event->key.keysym.sym == SDLK_l) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->SEvent_LoadGame();
			}
			// Quit game
			if (Event->key.keysym.sym == SDLK_q) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->SEvent_ShowEscapeScreen();
			}
			// Toggle window mode
			if (Event->key.keysym.sym == SDLK_w) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->SEvent_Windowed();
			}
			// Credits
			if (Event->key.keysym.sym == SDLK_c) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->SEvent_ShowCredits();
			}
		}
		break;

	case SDL_KEYUP:
		break;

	default:
		break;
	}
}

void MainMenu::Update(Uint32 timeStep) {

}

void MainMenu::Render() {
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

void MainMenu::Cleanup() {
	// Clear vectors
	this->mImages.clear();
	this->MainMenuText.clear();
	this->CreditsImagesText.clear();
	this->EscapeImagesText.clear();
}

void MainMenu::ShowMainMenuText() {
	for (std::vector<ImageData*>::iterator it = this->MainMenuText.begin(); it != this->MainMenuText.end(); it++)
		(*it)->SetVisible(true);
}

void MainMenu::HideMainMenuText() {
	for (std::vector<ImageData*>::iterator it = this->MainMenuText.begin(); it != this->MainMenuText.end(); it++)
		(*it)->SetVisible(false);
}

void MainMenu::ShowCreditsImagesText() {
	for (std::vector<ImageData*>::iterator it = this->CreditsImagesText.begin(); it != this->CreditsImagesText.end(); it++)
		(*it)->SetVisible(true);
}

void MainMenu::HideCreditsImagesText() {
	for (std::vector<ImageData*>::iterator it = this->CreditsImagesText.begin(); it != this->CreditsImagesText.end(); it++)
		(*it)->SetVisible(false);
}

void MainMenu::SEvent_ShowCredits() {
	this->EventFlags.Credits = true;
	this->ShowCreditsImagesText();
	this->HideMainMenuText();
}

void MainMenu::SEvent_ShowMainMenu() {
	this->EventFlags.Credits = false;
	this->HideCreditsImagesText();
	this->ShowMainMenuText();
}

//// Scene Events
void MainMenu::SEvent_ExitToTitle() {
	// Return to title screen
	this->mManager->StartScene(Scene_TitleScreen);
}

void MainMenu::SEvent_NewGame() {
	// Start Intro scene
	this->mManager->StartScene(Scene_Intro);
}

void MainMenu::SEvent_LoadGame() {
	// Open filestream to save file
	std::ifstream savefile(".\\TSN.sav", std::ios::binary);

	// If file can't be read (permissions, doens't exist) then bail
	if (!savefile.good())
		return;

	SaveFile savedata;

	// Read saved player data
	savefile.read((char*)&savedata, sizeof(savedata));

	// Close filestream
	savefile.close();

	// Load saved data into PlayerData
	this->mPlayerData->SetName(savedata.GetName());
	this->mPlayerData->SetMoney(savedata.GetMoney());
	this->mPlayerData->SetDay(savedata.GetDay());
	this->mPlayerData->SetWeatherForecast(savedata.GetWeatherForecast());
	this->mPlayerData->SetEventForecast(savedata.GetEventForecast());
	this->mPlayerData->SetInventory(savedata.GetInventory());

	// Stop Music
	Mix_HaltMusic();

	// Load saved game and open market
	this->mManager->StartScene(Scene_Market);
}

void MainMenu::SEvent_Quit() {
	// Set flag to quit game
	this->mManager->quitGame = true;
}

void MainMenu::SEvent_Windowed() {
	// Get current fullscreen state
	bool fullscreen = this->mManager->IsFullscreen();
	
	// Change window mode
	this->mManager->GetGraphics()->Fullscreen(!fullscreen);
	
	// Set new fullscreen state
	this->mManager->SetFullscreen(!fullscreen);
}

void MainMenu::SEvent_ShowEscapeScreen() {
	this->ShowEscapeText();
	this->EscapeScreenVisible = true;
}

void MainMenu::SEvent_HideEscapeScreen() {
	this->HideEscapeText();
	this->EscapeScreenVisible = false;
}