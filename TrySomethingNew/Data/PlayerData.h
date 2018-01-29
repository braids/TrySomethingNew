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
	ForecastWeather FWeather;
	ForecastEvent FEvent;

public:
	PlayerData();

	void ResetPlayerData();

	std::vector<ItemData*>* GetInventory() { return &this->Inventory; }
	void ClearInventory();
	void SetMorningState();
	ItemData* GetInventoryItem(ItemName _name);
	bool HasInventoryItem(ItemName _name);
	void SetInventoryQty(ItemName _name, int _qty);

	void GenerateForecast();
	ForecastWeather GetWeatherForecast() { return this->FWeather; }
	ForecastEvent GetEventForecast() { return this->FEvent; }
	void SetWeatherForecast(ForecastWeather _weather) { this->FWeather = _weather; }
	void SetEventForecast(ForecastEvent _event) { this->FEvent = _event; }

	std::string GetName() { return this->ShopName; }
	void SetName(std::string _name) { this->ShopName = _name; }
	int GetMoney() { return this->Money; }
	void SetMoney(int _money) { this->Money = _money; }
	int GetDay() { return this->Day; }
	void SetDay(int _day) { this->Day = _day; }
};

class SaveFile {
protected:
	int Money;
	int Day;
	ForecastWeather FWeather;
	ForecastEvent FEvent;
	char ShopName[26];

public:
	SaveFile();
	SaveFile(PlayerData _data);

	char* GetName() { return this->ShopName; }
	int GetDay() { return this->Day; }
	int GetMoney() { return this->Money; }
	ForecastWeather GetWeatherForecast() { return this->FWeather; }
	ForecastEvent GetEventForecast() { return this->FEvent; }
};

std::string GetWeatherDesc(ForecastWeather _weather);
std::string GetEventDesc(ForecastEvent _event);
#endif