#include "Data\ItemData.h"

ItemData::ItemData(ItemName _name) {
	this->Name = _name;
	this->Type = GetItemType(this->GetName());
	this->Quantity = 0;
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
		break;
	case ItemName::Item_StreetSheet:
	case ItemName::Item_USADAY:
		return ItemType::ItemType_Newspaper;
		break;
	case ItemName::Item_Sign:
	case ItemName::Item_Poster:
	case ItemName::Item_NewsAd:
		return ItemType::ItemType_Newspaper;
		break;
	}
}

ItemData* GetItemDataByName(ItemData* _item, ItemName _name) {
	return (_item->GetName() == _name) ? _item : nullptr;
}