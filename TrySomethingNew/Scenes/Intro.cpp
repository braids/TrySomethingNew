#include "Assets.h"
#include "Camera.h"
#include "GameObjects\GameObject.h"
#include "Graphics.h"
#include "Scenes\Scene.h"
#include "SceneManager.h"

Intro::Intro() {
	// Set current scene name
	this->SetSceneName(Scene_Intro);
}

void Intro::LoadGameObjects() {
	this->mGameObjects.clear();
}

void Intro::SceneStart() {
	// Set flags to false
	this->ExitToTitleScreen = false;
	this->EditName = false;
	
	// Play intro music
	Mix_PlayMusic(this->mManager->GetAssets()->music.IntroMusic, -1);
	
	// Load game objects
	this->LoadGameObjects();

	// Clear any existing drawn text.
	this->mImages.clear();
	
	// Text objects
	this->TextObjects.EnterShopName = this->AddText("SHOP NAME:", 8, 8);
	this->TextObjects.ShopNameEntry = this->AddText("", 8, 16);
	this->ShopName = this->TextObjects.ShopNameEntry->GetText();

	// Start with text entry off
	SDL_StopTextInput();
}

void Intro::HandleEvent(SDL_Event * Event) {
	switch (Event->type) {
	case SDL_KEYDOWN:
		if (Event->key.keysym.sym == SDLK_ESCAPE) this->ExitToTitleScreen = true;
		if (Event->key.keysym.sym == SDLK_RETURN) {
			// Start text entry if enter is pressed
			if (!SDL_IsTextInputActive()) {
				this->EditName = true;
				SDL_StartTextInput();
			}
			// Conversely, stop text entry when enter is pressed.
			else if (SDL_IsTextInputActive()) {
				this->EditName = false;
				SDL_StopTextInput();
			}
		}
		// If we're editing and hit backspace, erase a character.
		if (this->EditName && Event->key.keysym.sym == SDLK_BACKSPACE && this->ShopName->length() > 0) {
			this->ShopName->pop_back();
			this->UpdateText(this->TextObjects.ShopNameEntry);
		}
		break;

	case SDL_KEYUP:
		break;

	case SDL_TEXTINPUT:
		// As long as we are editing the name and it's less than 25 characters, add characters.
		if (this->EditName && this->ShopName->length() < 25) {
			this->ShopName->append(Event->text.text);
			this->UpdateText(this->TextObjects.ShopNameEntry);
		}
		break;
	case SDL_TEXTEDITING:
		
		break;
	default:
		break;
	}
}

void Intro::Update(Uint32 timeStep) {
	// Return to title screen if quitting
	if (this->ExitToTitleScreen) {
		// Stop current music
		Mix_HaltMusic();
		// Go to title.
		this->mManager->StartScene(Scene_TitleScreen);
	}
}

void Intro::Render() {
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