#include <algorithm>
#include <string>
#include <vector>
#include "Data\ItemData.h"
#include "Data\PlayerData.h"

PlayerData::PlayerData() {
	this->ResetPlayerData();
}

void PlayerData::ResetPlayerData() {
	this->ShopName = "";
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