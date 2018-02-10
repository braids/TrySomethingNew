#ifndef _DAYSALES_H_
#define _DAYSALES_H_

#include <string>
#include <vector>
#include <SDL2\SDL.h>
#include "Assets.h"
#include "Data\CustomerData.h"
#include "Data\ItemData.h"
#include "GameObjects\GameObject.h"
#include "Scenes\Scene.h"
#include "Scenes\SubScreens\EscapeScreen.h"
#include "Timer.h"

class DaySales : public Scene, EscapeScreen {
protected:
	std::vector<ImageData*> DaySalesText;
	std::vector<ItemData*> SellData;

	CustomerVec Customers;
	CustomerVecIter CustomerSpawnIter;
	Uint32 CustomerSpawnInterval;

	const Uint32 DaySegmentLength = 5000;
	int Money;

	struct {
		ImageData* Shack;
		ImageData* Shack1;
		ImageData* Shack2;
		ImageData* Shack3;
	} Images;

	struct {
		ImageData* ShopName;
		ImageData* MoneyText;
		ImageData* MoneyAmt;
		ImageData* DayText;
		ImageData* DayNum;
	} TextObjects;

	struct {

	} TextBoxObjects;

	struct {
		EventTimer* CustomerSpawn;
		EventTimer* DayRuntime1;
		EventTimer* DayRuntime2;
		EventTimer* DayRuntime3;
	} EventTimers;

	struct {
		bool ExitToTitleScreen;
		bool Simulation;
		bool DayEnd;
	} EventFlags;

public:
	// Scene ctor
	DaySales();

	// Scene funcs
	void ResetFlags();
	void LoadEventTimers();
	void LoadImagesText();
	void SceneStart();
	void HandleEvent(SDL_Event* Event);
	void Update(Uint32 timeStep);
	void Render();
	void Cleanup();

	// DaySales funcs
	ImageData* AddDaySalesText(std::string _text, int _x, int _y);
	void GetCurrentPlayerInventory();
	void GenerateCustomers();
	void GetPurchase(Customer * _customer);

	//// Events
	// Show/Hide Events
	void SEvent_ShowDaySalesText();
	// Spawn customer
	void SEvent_SpawnCustomer();
	// Change daytime phase
	void SEvent_DayRuntime2();
	void SEvent_DayRuntime3();
	void SEvent_DayRuntimeEnd();
	// Exit to title
	void SEvent_ExitToTitle();
	void SEvent_ShowEscapeScreen();
	void SEvent_HideEscapeScreen();
};

#endif
