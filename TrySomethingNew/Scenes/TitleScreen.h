#ifndef _TITLESCREEN_H_
#define _TITLESCREEN_H_

#include <SDL2\SDL.h>
#include "Scenes\Scene.h"
#include "Scenes\SubScreens\EscapeScreen.h"

//// Title screen scene class
class TitleScreen : public Scene, EscapeScreen {
protected:
	std::vector<ImageData*> TitleImages;

	bool StartMainMenu;

public:
	// Scene ctor
	TitleScreen();

	// Scene funcs
	void LoadTextImages();
	void SceneStart();
	void HandleEvent(SDL_Event* Event);
	void Update(Uint32 timeStep);
	void Render();
	void Cleanup();

	void ShowTitleImage();
	void HideTitleImage();
	void SEvent_ToMainMenu();
	void SEvent_ShowEscapeScreen();
	void SEvent_HideEscapeScreen();
};

#endif