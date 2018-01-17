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
	this->ShopNamed = false;
	this->EventFlags.OpeningText1 = true;
	this->EventFlags.OpeningText2 = false;
	this->EventFlags.OpeningText3 = false;
	this->EventFlags.OpeningText4 = false;
	this->EventFlags.EnterShopName = false;
	
	// Init event timers
	this->EventTimers.IntroDate1 = nullptr;
	this->EventTimers.IntroDate2 = nullptr;
	this->EventTimers.IntroText1 = nullptr;
	this->EventTimers.IntroText2 = nullptr;

	// Play intro music
	Mix_PlayMusic(this->mManager->GetAssets()->music.IntroMusic, -1);
	
	// Load game objects
	this->LoadGameObjects();

	// Clear any existing drawn text.
	this->mImages.clear();
	this->mEventTimers.clear();

	// Event timer
	this->EventTimers.IntroDate1 = this->AddEventTimer(new EventTimer(&this->EventFlags.OpeningText1, 2500, &this->EventTimers.IntroDate2));
	this->EventTimers.IntroDate2 = this->AddEventTimer(new EventTimer(&this->EventFlags.OpeningText2, 2500, &this->EventTimers.IntroText1));
	this->EventTimers.IntroText1 = this->AddEventTimer(new EventTimer(&this->EventFlags.OpeningText3, 9000, &this->EventTimers.IntroText2));
	this->EventTimers.IntroText2 = this->AddEventTimer(new EventTimer(&this->EventFlags.OpeningText4, 9000));

	// Image objects
	this->Images.Wall1 = new ImageData();
	this->Images.Wall1->SetImage(&Assets::Instance()->images.Wall1);
	this->mImages.push_back(this->Images.Wall1);
	this->Images.Wall2 = new ImageData();
	this->Images.Wall2->SetImage(&Assets::Instance()->images.Wall2);
	this->mImages.push_back(this->Images.Wall2);

	// Text objects
	this->TextObjects.OpeningDate = this->AddText("DECEMBER, 1989", 88, 88);
	this->TextObjects.OpeningLocation = this->AddText("BERLIN, WEST GERMANY", 67, 96);
	this->TextObjects.IntroText1 = this->AddText("THE BERLIN WALL NO LONGER SPLITS EAST\nAND WEST GERMANY. AFTER THREE DECADES,\nFAMILIES AND FRIENDS CAN REUNITE.", 7, 164);
	this->TextObjects.IntroText2 = this->AddText("CAPITALISM SEES THIS CLOSING RIFT AND\nHAS STEPPED IN TO GIVE EACH SIDE A\nTASTE OF THE OTHER...FOR A SMALL COST.", 7, 164);
	this->TextObjects.SettingBlurb = this->AddText("       *** DEINE GESCHICHTE ***\n\n    YOU HAVE ARRIVED FROM THE UNITED STATES TO SET UP SHOP OUTSIDE OF THE BRANDENBURG GATE. THERE ARE MANY EAST AND WEST BERLINERS WALKING BY LOOKING TO BUY WARES THEY HAVE NEVER SEEN.\n\n    YOUR SHOP WILL NEED A NAME BEFORE YOUR BUSINESS CAN OPEN.\n", 7, 9);
	this->TextObjects.EnterShopName = this->AddText("SHOP NAME:", 7, 108);
	this->TextBoxObjects.ShopNameEntry = this->AddTextBox(25, 7, 117);
	this->TextObjects.ShopIsSetUp = this->AddText("    AFTER COBBLING TOGETHER A SMALL SHACK ON THE OUTER PERIMETER OF THE WALL, YOU HAVE 50 DEUTSCHE MARKS (DM) LEFT TO BUY WARES.", 7, 135);

	for (std::vector<ImageData*>::iterator it = this->mImages.begin(); it != this->mImages.end(); it++)
		(*it)->SetVisible(false);

	this->TextObjects.OpeningDate->SetVisible(true);
	this->EventTimers.IntroDate1->StartEventTimer();

	// Start with text entry off
	SDL_StopTextInput();
}

void Intro::HandleEvent(SDL_Event * Event) {
	switch (Event->type) {
	case SDL_KEYDOWN:
		if (Event->key.keysym.sym == SDLK_ESCAPE) this->ExitToTitleScreen = true;
		if (Event->key.keysym.sym == SDLK_RETURN) {
			// Stop text entry when enter is pressed.
			if (SDL_IsTextInputActive() && this->EditName) {
				this->EditName = false;
				SDL_StopTextInput();
				this->TextBoxObjects.ShopNameEntry->SetActive(false);
				this->ShopNamed = true;
			}
		}
		// If we're editing and hit backspace, erase a character.
		if (this->EditName && Event->key.keysym.sym == SDLK_BACKSPACE) {
			this->TextBoxObjects.ShopNameEntry->DeleteText();
		}
		break;

	case SDL_KEYUP:
		break;

	case SDL_TEXTINPUT:
		// As long as we are editing the name and it's less than 25 characters, add characters.
		if (this->EditName) {
			this->TextBoxObjects.ShopNameEntry->AppendText(Event->text.text);
		}
		break;
	case SDL_TEXTEDITING:
		
		break;
	default:
		break;
	}
}

void Intro::Update(Uint32 timeStep) {
	// Update timers
	this->UpdateEventTimers();
	
	// If intro screens are done, start shop name entry
	if (!this->EventFlags.OpeningText1 && 
			!this->EventFlags.OpeningText2 && 
			!this->EventFlags.OpeningText3 && 
			!this->EventFlags.OpeningText4)
		this->EventFlags.EnterShopName = true;

	// Return to title screen if quitting
	if (this->ExitToTitleScreen) {
		// Stop current music
		Mix_HaltMusic();
		// Go to title.
		this->mManager->StartScene(Scene_TitleScreen);
	}

	// Display intro screens
	if (this->EventFlags.OpeningText1) {
		
	}
	else if (this->EventFlags.OpeningText2) {
		if (this->TextObjects.OpeningDate->IsVisible())
			this->TextObjects.OpeningLocation->SetVisible(true);
	}
	else if (this->EventFlags.OpeningText3) {
		if (this->TextObjects.OpeningLocation->IsVisible()) {
			this->TextObjects.OpeningDate->SetVisible(false);
			this->TextObjects.OpeningLocation->SetVisible(false);
			
			this->Images.Wall1->SetVisible(true);
			this->TextObjects.IntroText1->SetVisible(true);
		}	
	}
	else if (this->EventFlags.OpeningText4) {
		if (this->TextObjects.IntroText1->IsVisible()) {
			this->Images.Wall1->SetVisible(false);
			this->TextObjects.IntroText1->SetVisible(false);
			
			this->Images.Wall2->SetVisible(true);
			this->TextObjects.IntroText2->SetVisible(true);
		}	
	}
	// Display name entry screen
	else if (this->EventFlags.EnterShopName) {
		if (this->TextObjects.IntroText2->IsVisible()) {
			this->Images.Wall2->SetVisible(false);
			this->TextObjects.IntroText2->SetVisible(false);

			this->TextObjects.SettingBlurb->SetVisible(true);
			this->TextObjects.EnterShopName->SetVisible(true);
			this->TextBoxObjects.ShopNameEntry->SetVisible(true);
			this->EditName = true;
			SDL_StartTextInput();
			this->TextBoxObjects.ShopNameEntry->SetActive(this->EditName);
		}
		if (ShopNamed)
			this->TextObjects.ShopIsSetUp->SetVisible(true);
		this->ShopName = this->TextBoxObjects.ShopNameEntry->GetText();
	}
}

void Intro::Render() {
	// Render graphics to buffer
	// If I find any game logic in here, I'll slap myself silly
	for (int i = 0; i < (int) this->mImages.size(); i++) {
		if(this->mImages[i]->IsVisible())
		this->mManager->GetGraphics()->DrawTextureAtLocation(
			this->mImages[i]->GetImage()->texture,
			this->mImages[i]->GetImage()->rect,
			this->mImages[i]->GetDrawRect(),
			this->mImages[i]->GetDrawAngle()
		);
	}
}