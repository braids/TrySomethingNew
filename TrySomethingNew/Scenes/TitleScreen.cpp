#include "Assets.h"
#include "Graphics.h"
#include "Scenes\Scene.h"
#include "SceneManager.h"

TitleScreen::TitleScreen() {
	// Set current scene name
	this->SetSceneName(Scene_TitleScreen);
}

void TitleScreen::LoadGameObjects() {

}

void TitleScreen::SceneStart() {
	this->StartMainMenu = false;
	if(!Mix_PlayingMusic())
		Mix_PlayMusic(this->mManager->GetAssets()->music.TitleMusic, -1);
}

void TitleScreen::HandleEvent(SDL_Event * Event) {
	switch (Event->type) {
	case SDL_KEYDOWN:
		if (Event->key.keysym.sym == SDLK_ESCAPE) this->mManager->quitGame = true;

		if (Event->key.keysym.sym == SDLK_RETURN) this->StartMainMenu = true;

		if (Event->key.keysym.sym == SDLK_r && Event->key.repeat == 0) this->SceneStart();

		break;

	case SDL_KEYUP:

		break;
	
	default:
		break;
	}
}

void TitleScreen::Update(Uint32 timeStep) {
	if (this->StartMainMenu)
		this->mManager->StartScene(Scene_MainMenu);
}

void TitleScreen::Render() {
	// Render graphics to buffer
	// If I find any game logic in here, I'll slap myself silly
	this->mManager->GetGraphics()->DrawTexture(this->mManager->GetAssets()->images.Title.texture);
}
