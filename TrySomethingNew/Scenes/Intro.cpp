#include <vector>
#include <SDL2\SDL.h>
#include <SDL2\SDL_mixer.h>
#include "Assets.h"
#include "Graphics.h"
#include "Scenes\Intro.h"
#include "Scenes\Scene.h"
#include "SceneManager.h"
#include "Timer.h"

Intro::Intro() {
	// Set current scene name
	this->SetSceneName(Scene_Intro);
}

void Intro::ResetFlags() {
	// Set flags to false
	this->EventFlags.ExitToTitleScreen = false;
	this->EventFlags.IntroScreen2 = false;
	this->EventFlags.IntroScreen3 = false;
	this->EventFlags.EditName = false;
	this->EventFlags.ShopNamed = false;
}

void Intro::LoadEventTimers() {
	// Clear existing timers
	this->mEventTimers.clear();

	// Init event timers
	this->EventTimers.IntroScreen1_Date = this->AddEventTimer(new EventTimer(std::bind(&Intro::SEvent_IntroScreen1_ShowLocation, this), (Uint32) 2500));
	this->EventTimers.IntroScreen1_Location = this->AddEventTimer(new EventTimer(std::bind(&Intro::SEvent_IntroScreen2_Show, this), (Uint32) 2500));
	this->EventTimers.IntroScreen2 = this->AddEventTimer(new EventTimer(std::bind(&Intro::SEvent_IntroScreen3_Show, this), (Uint32) 9000));
	this->EventTimers.IntroScreen3 = this->AddEventTimer(new EventTimer(std::bind(&Intro::SEvent_NameEntryScreen_Show, this), (Uint32) 9000));
}

void Intro::LoadImagesText() {
	// Clear any existing drawn text.
	this->mImages.clear();

	//// Image objects
	// Intro Screen 2
	this->Images.Wall1 = new ImageData();
	this->Images.Wall1->SetImage(&Assets::Instance()->images.Wall1);
	this->mImages.push_back(this->Images.Wall1);
	// Intro Screen 3
	this->Images.Wall2 = new ImageData();
	this->Images.Wall2->SetImage(&Assets::Instance()->images.Wall2);
	this->mImages.push_back(this->Images.Wall2);

	//// Text objects
	// Intro Screen 1
	this->TextObjects.OpeningDate = this->AddText("DECEMBER, 1989", 91, 90);
	this->TextObjects.OpeningLocation = this->AddText("BERLIN, WEST GERMANY", 70, 99);
	// Intro Screen 2
	this->TextObjects.IntroScreen2 = this->AddText("THE BERLIN WALL NO LONGER SPLITS EAST\nAND WEST GERMANY. AFTER THREE DECADES,\nFAMILIES AND FRIENDS CAN REUNITE.", 7, 164);
	// Intro Screen 3
	this->TextObjects.IntroScreen3 = this->AddText("CAPITALISM SEES THIS CLOSING RIFT AND\nHAS STEPPED IN TO GIVE EACH SIDE A\nTASTE OF THE OTHER...FOR A SMALL COST.", 7, 164);
	// Shop Name Entry
	this->TextObjects.SettingBlurb = this->AddText("       *** DEINE GESCHICHTE ***\n\n    YOU HAVE ARRIVED FROM THE UNITED STATES TO SET UP SHOP OUTSIDE OF THE BRANDENBURG GATE. THERE ARE MANY EAST AND WEST BERLINERS WALKING BY LOOKING TO BUY WARES THEY HAVE NEVER SEEN.\n\n    YOUR SHOP WILL NEED A NAME BEFORE YOUR BUSINESS CAN OPEN.\n", 7, 9);
	this->TextObjects.EnterShopName = this->AddText("SHOP NAME:", 7, 108);
	this->TextBoxObjects.ShopNameEntry = this->AddTextBox(25, 7, 117);
	this->TextObjects.ShopIsSetUp = this->AddText("    AFTER COBBLING TOGETHER A SMALL SHACK ON THE OUTER PERIMETER OF THE WALL, YOU HAVE 50 DEUTSCHE MARKS (DM) LEFT TO BUY WARES.", 7, 135);
	this->TextObjects.PressReturn = this->AddText("- PRESS RETURN -", 84, 180);

	// Set all images to not visible
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

	// Load Event Timers
	this->LoadEventTimers();
	
	// Load Images and Text Images
	this->LoadImagesText();

	// Reset player data
	this->mPlayerData->ResetPlayerData();

	// Display date text
	this->TextObjects.OpeningDate->SetVisible(true);
	
	// Start screen timer
	this->EventTimers.IntroScreen1_Date->StartEventTimer();
}

void Intro::HandleEvent(SDL_Event * Event) {
	switch (Event->type) {
	case SDL_KEYDOWN:
		if (Event->key.keysym.sym == SDLK_ESCAPE) {
			Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
			this->EventFlags.ExitToTitleScreen = true;
		}
		if (Event->key.keysym.sym == SDLK_RETURN) {
			if (this->EventFlags.IntroScreen2) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				// Skip Intro Screen 2
				this->SEvent_IntroScreen2_Skip();
			}
			else if (this->EventFlags.IntroScreen3) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				// Skip Intro Screen 3
				this->SEvent_IntroScreen3_Skip();
			}
			else if (this->EventFlags.EditName) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				// Name shop (if text entered) and show blurb
				this->SEvent_ShopNamed();
			}
			else if (this->EventFlags.ShopNamed) {
				Mix_PlayChannel(2, Assets::Instance()->sounds.Blip, 0);
				// Switch to Market screen
				this->SEvent_ToMarket();
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

void Intro::Cleanup() {

}

void Intro::SEvent_IntroScreen1_ShowLocation() {
	// Show location text
	this->TextObjects.OpeningLocation->SetVisible(true);
	
	// Start screen timer
	this->EventTimers.IntroScreen1_Location->StartEventTimer();
}

void Intro::SEvent_IntroScreen2_Show() {
	// Set screen state
	this->EventFlags.IntroScreen2 = true;

	// Hide date/time text
	this->TextObjects.OpeningDate->SetVisible(false);
	this->TextObjects.OpeningLocation->SetVisible(false);

	// Show image and text
	this->Images.Wall1->SetVisible(true);
	this->TextObjects.IntroScreen2->SetVisible(true);

	// Start screen timer
	this->EventTimers.IntroScreen2->StartEventTimer();
}

void Intro::SEvent_IntroScreen2_Skip() {
	// Halt timer
	this->EventTimers.IntroScreen2->stop();
	
	// Advance to next screen
	this->SEvent_IntroScreen3_Show();
}

void Intro::SEvent_IntroScreen3_Show() {
	// Set screen state
	this->EventFlags.IntroScreen2 = false;
	this->EventFlags.IntroScreen3 = true;

	// Hide previous image and text
	this->Images.Wall1->SetVisible(false);
	this->TextObjects.IntroScreen2->SetVisible(false);

	// Show image and text
	this->Images.Wall2->SetVisible(true);
	this->TextObjects.IntroScreen3->SetVisible(true);

	//Start screen timer
	this->EventTimers.IntroScreen3->StartEventTimer();
}

void Intro::SEvent_IntroScreen3_Skip() {
	// Halt timer
	this->EventTimers.IntroScreen3->stop();

	// Advance to next screen
	this->SEvent_NameEntryScreen_Show();
}

void Intro::SEvent_NameEntryScreen_Show() {
	// Set screen state
	this->EventFlags.IntroScreen3 = false;

	// Hide previous image and text
	this->Images.Wall2->SetVisible(false);
	this->TextObjects.IntroScreen3->SetVisible(false);

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

void Intro::SEvent_ShopNamed() {
	// If no name is entered, ignore event.
	if (this->TextBoxObjects.ShopNameEntry->GetText()->length() <= 0)
		return;

	// Set state flags
	this->EventFlags.EditName = false;
	this->EventFlags.ShopNamed = true;

	// Set player shop name
	this->mPlayerData->SetName(*(this->TextBoxObjects.ShopNameEntry->GetText()));
	
	// Stop text input and disable text box
	SDL_StopTextInput();
	this->TextBoxObjects.ShopNameEntry->SetActive(false);

	// Display post-naming blurb
	this->TextObjects.ShopIsSetUp->SetVisible(true);
	this->TextObjects.PressReturn->SetVisible(true);
}

void Intro::SEvent_ToMarket() {
	// Stop current music
	Mix_HaltMusic();

	// Set Forecast
	this->mPlayerData->GenerateForecast();

	// Go to market.
	this->mManager->StartScene(Scene_Market);
}