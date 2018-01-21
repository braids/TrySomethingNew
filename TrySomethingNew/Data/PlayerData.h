#ifndef _PLAYERDATA_H_
#define _PLAYERDATA_H_

#include <string>
#include <vector>
#include "Data\ItemData.h"

// Forawrd declaration
class PlayerData;

enum ForecastWeather {
	Weather_Sunny,
	Weather_Cloudy,
	Weather_Rainy
};

enum ForecastEvent {
	Event_EConcert,
	Event_WConcert,
	Event_EGallery,
	Event_WGallery,
	Event_None
};

class PlayerData {
protected:
	std::string ShopName;
	std::vector<ItemData*> Inventory;
	int Money;
	int Day;
	ForecastWeather fweather;
	ForecastEvent fevent;

public:
	PlayerData();

	void ResetPlayerData();

	std::vector<ItemData*>* GetInventory() { return &this->Inventory; }
	void ClearInventory();
	ItemData* GetInventoryItem(ItemName _name);
	bool HasInventoryItem(ItemName _name);
	void SetInventoryQty(ItemName _name, int _qty);

	void GenerateForecast();
	ForecastWeather GetWeatherForecast() { return this->fweather; }
	ForecastEvent GetEventForecast() { return this->fevent; }

	std::string GetName() { return this->ShopName; }
	void SetName(std::string _name) { this->ShopName = _name; }
	int GetMoney() { return this->Money; }
	void SetMoney(int _money) { this->Money = _money; }
	int GetDay() { return this->Day; }
	void SetDay(int _day) { this->Day = _day; }
};

std::string GetWeatherDesc(ForecastWeather _weather);
std::string GetEventDesc(ForecastEvent _event);
#endif