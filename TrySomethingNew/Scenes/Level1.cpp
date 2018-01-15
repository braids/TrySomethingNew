#include "Assets.h"
#include "Camera.h"
#include "GameObjects\GameObject.h"
#include "Graphics.h"
#include "Scenes\Scene.h"
#include "SceneManager.h"

Level1::Level1() {
	// Set current scene name
	this->SetSceneName(Scene_Level1);
}

void Level1::LoadAssets() {
	// Load images into Assets
}

void Level1::LoadGameObjects() {
	this->mGameObjects.clear();
}

void Level1::SceneStart() {
	this->LoadAssets();

	this->ExitToMainMenu = false;
	
	this->LoadGameObjects();
}

void Level1::HandleEvent(SDL_Event * Event) {
	switch (Event->type) {
	case SDL_KEYDOWN:
		if (Event->key.keysym.sym == SDLK_ESCAPE) this->ExitToMainMenu = true;
		if (Event->key.keysym.sym == SDLK_r && Event->key.repeat == 0) this->SceneStart();
		break;

	case SDL_KEYUP:
		break;

	default:
		break;
	}
}

void Level1::Update(Uint32 timeStep) {
	// Return to title screen if quitting
	if (this->ExitToMainMenu)
		this->mManager->StartScene(Scene_TitleScreen);
}

void Level1::Render() {
	// Render graphics to buffer
	// If I find any game logic in here, I'll slap myself silly
	for (int i = 0; i < (int) this->mGameObjects.size(); i++) {
		this->mManager->GetGraphics()->DrawTextureAtLocation(
			this->mGameObjects[i]->GetImageData()->GetImage()->texture, 
			this->mGameObjects[i]->GetImageData()->GetImage()->rect,
			this->mGameObjects[i]->GetImageData()->GetDrawRect(),
			this->mGameObjects[i]->GetImageData()->GetDrawAngle()
			);			
	}
}
