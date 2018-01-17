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

void Intro::ResetFlags() {
	// Set flags to false
	this->EventFlags.ExitToTitleScreen = false;
	this->EventFlags.EditName = false;
	this->EventFlags.ShopNamed = false;
}

void Intro::LoadGameObjects() {
	this->mGameObjects.clear();
}

void Intro::LoadEventTimers() {
	this->mEventTimers.clear();

	// Init event timers
	this->EventTimers.IntroDate1 = this->AddEventTimer(new EventTimer(std::bind(&Intro::SEvent_1, this), (Uint32) 2500));
	this->EventTimers.IntroDate2 = this->AddEventTimer(new EventTimer(std::bind(&Intro::SEvent_2, this), (Uint32) 2500));
	this->EventTimers.IntroText1 = this->AddEventTimer(new EventTimer(std::bind(&Intro::SEvent_3, this), (Uint32) 9000));
	this->EventTimers.IntroText2 = this->AddEventTimer(new EventTimer(std::bind(&Intro::SEvent_4, this), (Uint32) 9000));
}

void Intro::LoadImagesText() {
	// Clear any existing drawn text.
	this->mImages.clear();

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
}

void Intro::SceneStart() {
	// Play intro music
	Mix_PlayMusic(this->mManager->GetAssets()->music.IntroMusic, -1);
	
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

	// Display date text
	this->TextObjects.OpeningDate->SetVisible(true);
	// Start screen timer
	this->EventTimers.IntroDate1->StartEventTimer();
}

void Intro::HandleEvent(SDL_Event * Event) {
	switch (Event->type) {
	case SDL_KEYDOWN:
		if (Event->key.keysym.sym == SDLK_ESCAPE) this->EventFlags.ExitToTitleScreen = true;
		if (Event->key.keysym.sym == SDLK_RETURN) {
			// Stop text entry when enter is pressed.
			if (SDL_IsTextInputActive() && this->EventFlags.EditName) {
				// Set state flags
				this->EventFlags.EditName = false;
				this->EventFlags.ShopNamed = true;
				// Stop text input and disable text box
				SDL_StopTextInput();
				this->TextBoxObjects.ShopNameEntry->SetActive(false);
			}
		}
		// If we're editing and hit backspace, erase a character.
		if (this->EventFlags.EditName && Event->key.keysym.sym == SDLK_BACKSPACE) {
			this->TextBoxObjects.ShopNameEntry->DeleteText();
		}
		break;

	case SDL_KEYUP:
		break;

	case SDL_TEXTINPUT:
		// As long as we are editing the name and it's less than 25 characters, add characters.
		if (this->EventFlags.EditName) {
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
	
	// Return to title screen if quitting
	if (this->EventFlags.ExitToTitleScreen) {
		// Stop current music
		Mix_HaltMusic();
		// Go to title.
		this->mManager->StartScene(Scene_TitleScreen);
	}

	if (this->EventFlags.ShopNamed && !this->TextObjects.ShopIsSetUp->IsVisible())
		this->TextObjects.ShopIsSetUp->SetVisible(true);
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

void Intro::SEvent_1() {
	// Show location text
	this->TextObjects.OpeningLocation->SetVisible(true);
	
	// Start screen timer
	this->EventTimers.IntroDate2->StartEventTimer();
}


void Intro::SEvent_2() {
	// Hide date/time text
	this->TextObjects.OpeningDate->SetVisible(false);
	this->TextObjects.OpeningLocation->SetVisible(false);

	// Show image and text
	this->Images.Wall1->SetVisible(true);
	this->TextObjects.IntroText1->SetVisible(true);

	// Start screen timer
	this->EventTimers.IntroText1->StartEventTimer();
}

void Intro::SEvent_3() {
	// Hide previous image and text
	this->Images.Wall1->SetVisible(false);
	this->TextObjects.IntroText1->SetVisible(false);

	// Show image and text
	this->Images.Wall2->SetVisible(true);
	this->TextObjects.IntroText2->SetVisible(true);

	//Start screen timer
	this->EventTimers.IntroText2->StartEventTimer();
}

void Intro::SEvent_4() {
	// Hide previous image and text
	this->Images.Wall2->SetVisible(false);
	this->TextObjects.IntroText2->SetVisible(false);

	// Show text
	this->TextObjects.SettingBlurb->SetVisible(true);
	this->TextObjects.EnterShopName->SetVisible(true);
	
	// Create text box
	this->TextBoxObjects.ShopNameEntry->SetVisible(true);
	
	// Enable text entry
	SDL_StartTextInput();
	this->EventFlags.EditName = true;
	this->TextBoxObjects.ShopNameEntry->SetActive(this->EventFlags.EditName);
}
