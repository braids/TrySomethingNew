#include <algorithm>
#include <string>
#include <vector>
#include "Data\ItemData.h"
#include "Data\PlayerData.h"

PlayerData::PlayerData() {
	this->ResetPlayerData();
}

void PlayerData::ResetPlayerData() {
	this->ShopName = "Test Name";
	this->Inventory = *GetInitialItemVector();
	this->Money = 50;
	this->Day = 1;
}

ItemData* PlayerData::GetInventoryItem(ItemName _name) {
	return GetItemFromVector(_name, &this->Inventory);
}

bool PlayerData::HasInventoryItem(ItemName _name) {
	return (this->GetInventoryItem(_name)->GetQuantity() > 0) ? true : false;
}

void PlayerData::SetInventoryQty(ItemName _name, int _qty) {
	this->GetInventoryItem(_name)->SetQuantity(_qty);
}

void PlayerData::GenerateForecast() {
	switch (rand() % 4) {
	case 1:
		this->fweather = ForecastWeather::Weather_Cloudy;
		break;
	case 2:
		this->fweather = ForecastWeather::Weather_Rainy;
		break;
	default:
		this->fweather = ForecastWeather::Weather_Sunny;
		break;
	}

	switch (rand() % 8) {
	case 1:
		this->fevent = ForecastEvent::Event_EConcert;
		break;
	case 2:
		this->fevent = ForecastEvent::Event_WConcert;
		break;
	case 3:
		this->fevent = ForecastEvent::Event_EGallery;
		break;
	case 4:
		this->fevent = ForecastEvent::Event_WGallery;
		break;
	default:
		this->fevent = ForecastEvent::Event_None;
		break;
	}
}

std::string GetWeatherDesc(ForecastWeather _weather) {
	switch (_weather) {
	case ForecastWeather::Weather_Sunny:
		return "    THE SKY IS CLEAR AND THE SUN IS SHINING.";
	case ForecastWeather::Weather_Cloudy:
		return "    CLOUDS BLANKET THE SKY. IT'S A  FAIR TEMPERATURE OUTSIDE.";
	case ForecastWeather::Weather_Rainy:
		return "    EXPECT BUCKETS OF RAIN TODAY. BE SURE TO GRAB AN UMBRELLA.";
	default:
		return "    ZALGO HAS COME TO REAP WHAT HE IS OWED. STAY INSIDE.";
	}
}

std::string GetEventDesc(ForecastEvent _event) {
	switch (_event) {
	case ForecastEvent::Event_EConcert:
		return "    A LARGE HOFF CONCERT IS ON FOR TONIGHT IN EAST BERLIN.";
	case ForecastEvent::Event_WConcert:
		return "    A LARGE ROCK CONCERT IS ON FOR TONIGHT IN WEST BERLIN."; 
	case ForecastEvent::Event_EGallery:
		return "    AN ART GALLERY IS OPEN TO THE PUBLIC IN EAST BERLIN.";
	case ForecastEvent::Event_WGallery:
		return "    AN ART GALLERY IS OPEN TO THE PUBLIC IN WEST BERLIN.";
	case ForecastEvent::Event_None:
		return "    NO EVENTS TODAY.";
	default:
		return "    HE HAS COME HAS HAS RISEN PRAISE ZALGO.";
	}
}