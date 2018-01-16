#include "Assets.h"
#include "Camera.h"
#include "GameObjects\GameObject.h"
#include "Graphics.h"
#include "Scenes\Scene.h"
#include "SceneManager.h"

MainMenu::MainMenu() {
	// Set current scene name
	this->SetSceneName(Scene_MainMenu);
}

void MainMenu::LoadGameObjects() {
	this->mGameObjects.clear();
}

void MainMenu::SceneStart() {
	this->ExitToTitleScreen = false;

	this->LoadGameObjects();

	this->mImages.clear();
	this->AddText("N)EW GAME", 108, 64);
	this->AddText("L)OAD GAME", 108, 80);
	this->AddText("Q)UIT", 108, 96);
	this->AddText("W)INDOW MODE", 100, 176);
}

void MainMenu::HandleEvent(SDL_Event * Event) {
	switch (Event->type) {
	case SDL_KEYDOWN:
		if (Event->key.keysym.sym == SDLK_ESCAPE) this->ExitToTitleScreen = true;
		if (Event->key.keysym.sym == SDLK_q) this->mManager->quitGame = true;
		if (Event->key.keysym.sym == SDLK_r && Event->key.repeat == 0) this->SceneStart();
		if (Event->key.keysym.sym == SDLK_w) {
			bool fullscreen = this->mManager->IsFullscreen();
			this->mManager->GetGraphics()->Fullscreen(!fullscreen);
			this->mManager->SetFullscreen(!fullscreen);
		}
		break;

	case SDL_KEYUP:
		break;

	default:
		break;
	}
}

void MainMenu::Update(Uint32 timeStep) {
	// Return to title screen if quitting
	if (this->ExitToTitleScreen)
		this->mManager->StartScene(Scene_TitleScreen);
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

	for (int i = 0; i < (int) this->mGameObjects.size(); i++) {
		this->mManager->GetGraphics()->DrawTextureAtLocation(
			this->mGameObjects[i]->GetImageData()->GetImage()->texture, 
			this->mGameObjects[i]->GetImageData()->GetImage()->rect,
			this->mGameObjects[i]->GetImageData()->GetDrawRect(),
			this->mGameObjects[i]->GetImageData()->GetDrawAngle()
			);			
	}
}
