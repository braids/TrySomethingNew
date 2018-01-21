#include <fstream>
#include <string>
#include <vector>
#include "Assets.h"
#include "Graphics.h"
#include "Scenes\Scene.h"
#include "SceneManager.h"

MainMenu::MainMenu() {
	// Set current scene name
	this->SetSceneName(Scene_MainMenu);
}

void MainMenu::SceneStart() {
	// Load text images. It's just this stuff, nothing fancy needed.
	this->mImages.clear();
	this->AddText("N)EW GAME", 105, 63);
	this->AddText("L)OAD GAME", 105, 81);
	this->AddText("Q)UIT", 105, 99);
	this->AddText("W)INDOW MODE", 98, 171);
}

void MainMenu::HandleEvent(SDL_Event* Event) {
	switch (Event->type) {
	case SDL_KEYDOWN:
		// Return to title screen
		if (Event->key.keysym.sym == SDLK_ESCAPE)
			this->SEvent_ExitToTitle();
		// Start new game
		if (Event->key.keysym.sym == SDLK_n) 
			this->SEvent_NewGame();
		// Load saved game
		if (Event->key.keysym.sym == SDLK_l) 
			this->SEvent_LoadGame();
		// Quit game
		if (Event->key.keysym.sym == SDLK_q) 
			this->SEvent_Quit();
		// Toggle window mode
		if (Event->key.keysym.sym == SDLK_w)
			this->SEvent_Windowed();
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
		this->mManager->GetGraphics()->DrawTextureAtLocation(
			this->mImages[i]->GetImage()->texture,
			this->mImages[i]->GetImage()->rect,
			this->mImages[i]->GetDrawRect(),
			this->mImages[i]->GetDrawAngle()
		);
	}
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
