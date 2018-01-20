#ifndef _ITEMDATA_H_
#define _ITEMDATA_H_

#include <string>
#include <vector>

// Forawrd declaration
enum ItemName;
enum ItemType;
class ItemData;

enum ItemName {
	Item_Bier,
	Item_Bockwurst,
	Item_Mettigel,
	Item_Currywurst,
	Item_StreetSheet,
	Item_USADAY,
	Item_Sign,
	Item_Poster,
	Item_NewsAd
};

enum ItemType {
	ItemType_Food,
	ItemType_Newspaper,
	ItemType_Ad
};

class ItemData {
protected:
	ItemName	Name;
	ItemType	Type;
	int			Quantity;
	int			BuyPrice;
	int			SellPrice;

public:
	ItemData(ItemName _name);

	ItemName GetName() { return this->Name; }
	ItemType GetType() { return this->Type; }
	int GetQuantity() { return this->Quantity; }
	int GetBuyPrice() { return this->BuyPrice; }
	int GetSellPrice() { return this->SellPrice; }
	void SetQuantity(int _qty) { this->Quantity = _qty; }
	void AddQuantity(int _qty) { this->Quantity += _qty; }
	void SubQuantity(int _qty) { this->Quantity -= _qty; }
	void SetSellPrice(int _price) { this->SellPrice = _price; }
};

ItemType GetItemType(ItemData* _item);
ItemType GetItemType(ItemName _name);
ItemData* GetItemDataByName(ItemData* _item, ItemName _name);
int GetItemBuyPrice(ItemName _name);
std::string GetItemString(ItemName _name);
std::string GetItemGuideDesc(ItemName _name);
std::vector<ItemData*>* GetInitialItemVector();
ItemData* GetItemFromVector(ItemName _name, std::vector<ItemData*>* _itemVector);

#endif
