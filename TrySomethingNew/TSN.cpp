#include "TSN.h"

TSN::TSN() {
	// Set game running/playing flags
	Running = true;
}

int TSN::OnExecute() {
	// Initialize scenes and SDL Graphics/Audio
	if (scene.Init() == false) {
		return -1;
	}

	SDL_Event Event;

	// Main loop
	while (Running) {
		// Update global ticks
		scene.UpdateTicks();

		// Check events
		while (SDL_PollEvent(&Event)) {
			scene.Event(&Event);
		}
		// Perform actions and updates
		scene.Update();

		// Render scene
		scene.Render();

		// Check if scene manager has given the go-ahead to quit game
		if (scene.quitGame) Running = false;
	}

	return 0;
}

int main(int argc, char* argv[]) {
	TSN tsn;

	return tsn.OnExecute();
}
