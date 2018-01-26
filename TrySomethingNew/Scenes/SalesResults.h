#ifndef _SALESRESULTS_H_
#define _SALESRESULTS_H_

#include <string>
#include <vector>
#include <SDL2\SDL.h>
#include "Assets.h"
#include "Data\ItemData.h"
#include "Scenes\Scene.h"

class SalesResults : public Scene {
protected:
	// Sales Results screen text and text boxes
	std::vector<ImageData*> SalesResultsText;
	std::vector<ItemData*> SellItems;

	struct {
		ImageData* PlayerMoneyAmount;
	} TextObjects;

	struct {
		bool ExitToTitleScreen;
		bool ExitResults;
		bool GameOver;
	} EventFlags;

public:
	// Scene ctor
	SalesResults();

	// Scene funcs
	void ResetFlags();
	void LoadImagesText();
	void SceneStart();
	void HandleEvent(SDL_Event* Event);
	void Update(Uint32 timeStep);
	void Render();
	void Cleanup();

	// SalesResults funcs
	ImageData* AddSalesResultsText(std::string _text, int _x, int _y);
	void GetCurrentPlayerInventory();

	//// Events
	// Show/Hide Events
	void SEvent_ShowSalesResultsText();
	void SEvent_HideSalesResultsText();
	void SEvent_LoseCheck();
	void SEvent_GameOver_TitleScreen();
	void SEvent_NextDay();
};

#endif
