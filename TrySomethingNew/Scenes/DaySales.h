#ifndef _DAYSALES_H_
#define _DAYSALES_H_

#include <string>
#include <vector>
#include <SDL2\SDL.h>
#include "Assets.h"
#include "GameObjects\GameObject.h"
#include "Data\ItemData.h"
#include "Scenes\Scene.h"
#include "Timer.h"

class DaySales : public Scene {
protected:
	std::vector<ImageData*> DaySalesText;
	std::vector<ItemData*> SellItems;

	std::vector<ImageData*> CustomerImages;
	std::vector<Customer*> Customers;
	std::vector<CustomerObject*> CustomerObjects;
	Uint32 CustomerSpawnInterval;
	Uint32 CustomerSpawnTotal;

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
	void LoadGameObjects();
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
	void GetPurchase(Customer* _customer);

	//// Events
	// Show/Hide Events
	void SEvent_ShowDaySalesText();

	void SEvent_SpawnCustomer();
	void SEvent_DayRuntime2();
	void SEvent_DayRuntime3();
	void SEvent_DayRuntimeEnd();
};

#endif
