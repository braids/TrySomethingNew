#ifndef _INTRO_H_
#define _INTRO_H_

#include <vector>
#include <SDL2\SDL.h>
#include "Assets.h"
#include "Scenes\Scene.h"
#include "Timer.h"

//// Intro scene class
class Intro : public Scene {
protected:
	struct {
		ImageData* Wall1;
		ImageData* Wall2;
	} Images;

	struct {
		ImageData* OpeningDate;
		ImageData* OpeningLocation;
		ImageData* IntroScreen2;
		ImageData* IntroScreen3;
		ImageData* SettingBlurb;
		ImageData* EnterShopName;
		ImageData* ShopIsSetUp;
		ImageData* PressReturn;
	} TextObjects;

	struct {
		TextBox* ShopNameEntry;
	} TextBoxObjects;

	struct {
		EventTimer* IntroScreen1_Date;
		EventTimer* IntroScreen1_Location;
		EventTimer* IntroScreen2;
		EventTimer* IntroScreen3;
	} EventTimers;

	struct {
		bool ExitToTitleScreen;
		bool IntroScreen1;
		bool IntroScreen2;
		bool IntroScreen3;
		bool EditName;
		bool ShopNamed;
	} EventFlags;

public:
	// Scene ctor
	Intro();

	// Scene funcs
	void ResetFlags();
	void LoadEventTimers();
	void LoadImagesText();
	void SceneStart();
	void HandleEvent(SDL_Event* Event);
	void Update(Uint32 timeStep);
	void Render();
	void Cleanup();

	// Scene Events
	void SEvent_IntroScreen1_ShowLocation();
	void SEvent_IntroScreen1_Skip();
	void SEvent_IntroScreen2_Show();
	void SEvent_IntroScreen2_Skip();
	void SEvent_IntroScreen3_Show();
	void SEvent_IntroScreen3_Skip();
	void SEvent_NameEntryScreen_Show();
	void SEvent_ShopNamed();
	void SEvent_ToMarket();
};

#endif
