#ifndef _PLAYERDATA_H_
#define _PLAYERDATA_H_

#include <string>
#include <vector>
#include "Data\ItemData.h"

class PlayerData {
protected:
	std::string ShopName;
	std::vector<ItemData*> Inventory;
	int Money;
	int Day;
public:
	PlayerData();

	void ResetPlayerData();

	ItemData* GetInventoryItem(ItemName _name);
	bool HasInventoryItem(ItemName _name);
	void SetInventoryQty(ItemName _name, int _qty);

	std::string GetName() { return this->ShopName; }
	void SetName(std::string _name) { this->ShopName = _name; }
	int GetMoney() { return this->Money; }
	void SetMoney(int _money) { this->Money = _money; }
	int GetDay() { return this->Day; }
	void SetDay(int _day) { this->Day = _day; }
};

#endif