#ifndef _TITLESCREEN_H_
#define _TITLESCREEN_H_

#include <SDL2/SDL.h>
#include "Scenes/Scene.h"

//// Title screen scene class
class TitleScreen : public Scene {
protected:
	bool StartMainMenu;

public:
	// Scene ctor
	TitleScreen();

	// Scene funcs
	void SceneStart();
	void HandleEvent(SDL_Event* Event);
	void Update(Uint32 timeStep);
	void Render();
	void Cleanup();
};

#endif