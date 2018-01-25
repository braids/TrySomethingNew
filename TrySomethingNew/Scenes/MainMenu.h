#ifndef _MAINMENU_H_
#define _MAINMENU_H_

#include <vector>
#include <SDL2\SDL.h>
#include "Assets.h"
#include "Scenes\Scene.h"

//// MainMenu scene class
class MainMenu : public Scene {
protected:
	std::vector<ImageData*> MainMenuText;
	std::vector<ImageData*> CreditsImagesText;

	struct {
		ImageData* JamLogo;
		ImageData* ThemeLogo;
	} Images;

	struct {
		bool Credits;
	} EventFlags;

public:
	// Scene ctor
	MainMenu();

	// Scene funcs
	void ResetFlags();
	void LoadImagesText();
	void SceneStart();
	void HandleEvent(SDL_Event* Event);
	void Update(Uint32 timeStep);
	void Render();

	void SEvent_ShowCredits();
	void SEvent_ShowMainMenu();

	// Scene Events
	void SEvent_ExitToTitle();
	void SEvent_NewGame();
	void SEvent_LoadGame();
	void SEvent_Quit();
	void SEvent_Windowed();
};


#endif