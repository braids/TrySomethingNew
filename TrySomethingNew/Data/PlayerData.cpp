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
	//return *(find_if(this->Inventory.begin(), this->Inventory.end(), [&_name](ItemData* &_item) { return _item->GetName() == _name; }));
}

bool PlayerData::HasInventoryItem(ItemName _name) {
	/*
	ItemData* item = this->GetInventoryItem(_name);
	if (item == nullptr)
		return false;
	return (item->GetQuantity() > 0) ? true : false;
	*/
	return (this->GetInventoryItem(_name)->GetQuantity() > 0) ? true : false;
}

void PlayerData::SetInventoryQty(ItemName _name, int _qty) {
	/*
	ItemData* item = this->GetInventoryItem(_name);
	if (item == nullptr) {
		this->Inventory.push_back(new ItemData(_name));
		item = this->GetInventoryItem(_name);
	}
	item->SetQuantity(_qty);
	*/
	this->GetInventoryItem(_name)->SetQuantity(_qty);
}