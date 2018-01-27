#include <SDL2\SDL.h>
#include <SDL2\SDL_mixer.h>
#include "Assets.h"
#include "Graphics.h"
#include "SceneManager.h"
#include "Scenes\Scene.h"
#include "Scenes\TitleScreen.h"

TitleScreen::TitleScreen() {
	// Set current scene name
	this->SetSceneName(Scene_TitleScreen);
}

void TitleScreen::LoadTextImages() {
	this->TitleImages.push_back(this->AddImage(&Assets::Instance()->images.Title, 0, 0));

	// Load escape screen text
	this->AddEscapeText(this);

	// Hide all images
	for (std::vector<ImageData*>::iterator it = this->mImages.begin(); it != this->mImages.end(); it++)
		(*it)->SetVisible(false);
}

void TitleScreen::SceneStart() {
	this->StartMainMenu = false;

	if(!Mix_PlayingMusic())
		Mix_PlayMusic(this->mManager->GetAssets()->music.TitleMusic, -1);

	this->LoadTextImages();

	this->ShowTitleImage();
}

void TitleScreen::HandleEvent(SDL_Event * Event) {
	switch (Event->type) {
	case SDL_KEYDOWN:
		if (this->EscapeScreenVisible) {
			if (Event->key.keysym.sym == SDLK_y) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->mManager->quitGame = true;
			}
			if (Event->key.keysym.sym == SDLK_n) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->SEvent_HideEscapeScreen();
			}
		}
		else {
			if (Event->key.keysym.sym == SDLK_ESCAPE) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->SEvent_ShowEscapeScreen();
			}

			if (Event->key.keysym.sym == SDLK_RETURN) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				this->SEvent_ToMainMenu();
			}
		}

		break;

	case SDL_KEYUP:

		break;
	
	default:
		break;
	}
}

void TitleScreen::Update(Uint32 timeStep) {

}

void TitleScreen::Render() {
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

void TitleScreen::Cleanup() {
	this->TitleImages.clear();
	this->EscapeImagesText.clear();
}

void TitleScreen::ShowTitleImage() {
	for (std::vector<ImageData*>::iterator it = this->TitleImages.begin(); it != this->TitleImages.end(); it++)
		(*it)->SetVisible(true);
}

void TitleScreen::HideTitleImage() {
	for (std::vector<ImageData*>::iterator it = this->TitleImages.begin(); it != this->TitleImages.end(); it++)
		(*it)->SetVisible(false);
}

void TitleScreen::SEvent_ToMainMenu() {
	this->mManager->StartScene(Scene_MainMenu);
}

void TitleScreen::SEvent_ShowEscapeScreen() {
	this->ShowEscapeText();
	this->EscapeScreenVisible = true;
}

void TitleScreen::SEvent_HideEscapeScreen() {
	this->HideEscapeText();
	this->EscapeScreenVisible = false;
}

