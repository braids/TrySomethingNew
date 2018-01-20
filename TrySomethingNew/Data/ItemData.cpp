#include <algorithm>
#include <vector>
#include "Data\ItemData.h"

ItemData::ItemData(ItemName _name) {
	this->Name = _name;
	this->Type = GetItemType(this->GetName());
	this->Quantity = 0;
	this->BuyPrice = GetItemBuyPrice(this->GetName());
	this->SalePrice = 0;
}

ItemType GetItemType(ItemData* _item) {
	return GetItemType(_item->GetName());
}

ItemType GetItemType(ItemName _name) {
	switch (_name) {
	case ItemName::Item_Bier:
	case ItemName::Item_Bockwurst:
	case ItemName::Item_Currywurst:
	case ItemName::Item_Mettigel:
		return ItemType::ItemType_Food;
	case ItemName::Item_StreetSheet:
	case ItemName::Item_USADAY:
		return ItemType::ItemType_Newspaper;
	case ItemName::Item_Sign:
	case ItemName::Item_Poster:
	case ItemName::Item_NewsAd:
		return ItemType::ItemType_Ad;
	}
}

ItemData* GetItemDataByName(ItemData* _item, ItemName _name) {
	return (_item->GetName() == _name) ? _item : nullptr;
}

int GetItemBuyPrice(ItemName _name) {
	switch (_name) {
	case ItemName::Item_Bier:
		return 4;
	case ItemName::Item_Bockwurst:
		return 2;
	case ItemName::Item_Currywurst:
		return 4;
	case ItemName::Item_Mettigel:
		return 6;
	case ItemName::Item_StreetSheet:
		return 3;
	case ItemName::Item_USADAY:
		return 7;
	case ItemName::Item_Sign:
		return 5;
	case ItemName::Item_Poster:
		return 10;
	case ItemName::Item_NewsAd:
		return 15;
	}
}

std::vector<ItemData*>* GetInitialItemVector() {
	std::vector<ItemData*>* itemVector = new std::vector<ItemData*>();
	itemVector->push_back(new ItemData(ItemName::Item_Bier));
	itemVector->push_back(new ItemData(ItemName::Item_Bockwurst));
	itemVector->push_back(new ItemData(ItemName::Item_Currywurst));
	itemVector->push_back(new ItemData(ItemName::Item_Mettigel));
	itemVector->push_back(new ItemData(ItemName::Item_StreetSheet));
	itemVector->push_back(new ItemData(ItemName::Item_USADAY));
	itemVector->push_back(new ItemData(ItemName::Item_Sign));
	itemVector->push_back(new ItemData(ItemName::Item_Poster));
	itemVector->push_back(new ItemData(ItemName::Item_NewsAd));
	return itemVector;
}

ItemData* GetItemFromVector(ItemName _name, std::vector<ItemData*>* _itemVector) {
	return *(find_if(_itemVector->begin(), _itemVector->end(), [&_name](ItemData* &_item) { return _item->GetName() == _name; }));
}