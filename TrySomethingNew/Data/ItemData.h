#ifndef _ITEMDATA_H_
#define _ITEMDATA_H_

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

public:
	ItemData(ItemName _name);

	ItemName GetName() { return this->Name; }
	ItemType GetType() { return this->Type; }
	int GetQuantity() { return this->Quantity; }
	void SetQuantity(int _qty) { this->Quantity = _qty; }
	void AddQuantity(int _qty) { this->Quantity += _qty; }
	void SubQuantity(int _qty) { this->Quantity -= _qty; }
};

ItemType GetItemType(ItemData* _item);
ItemType GetItemType(ItemName _name);

ItemData* GetItemDataByName(ItemData* _item, ItemName _name);

#endif
