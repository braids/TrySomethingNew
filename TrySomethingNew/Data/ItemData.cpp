#include <algorithm>
#include <string>
#include <vector>
#include "Data\ItemData.h"

ItemData::ItemData() {
	this->Name = ItemName::Item_Bier;
	this->Type = GetItemType(this->GetName());
	this->Quantity = 0;
	this->BoughtQuantity = 0;
	this->BuyPrice = GetItemBuyPrice(this->GetName());
	this->SellPrice = 0;
	this->SalesTotal = 0;
}

ItemData::ItemData(ItemName _name) {
	this->Name = _name;
	this->Type = GetItemType(this->GetName());
	this->Quantity = 0;
	this->BoughtQuantity = 0;
	this->BuyPrice = GetItemBuyPrice(this->GetName());
	this->SellPrice = 0;
	this->SalesTotal = 0;
}

ItemType GetItemType(ItemData* _item) {
	return GetItemType(_item->GetName());
}

ItemType GetItemType(ItemName _name) {
	switch (_name) {
	case ItemName::Item_Bier:
	case ItemName::Item_Bockwurst:
	case ItemName::Item_Mettigel:
	case ItemName::Item_Currywurst:
		return ItemType::ItemType_Food;
	case ItemName::Item_StreetSheet:
	case ItemName::Item_USADAY:
		return ItemType::ItemType_Newspaper;
	case ItemName::Item_Sign:
	case ItemName::Item_Poster:
	case ItemName::Item_NewsAd:
		return ItemType::ItemType_Ad;
	default:
		return ItemType::ItemType_Food;
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
	case ItemName::Item_Mettigel:
		return 4;
	case ItemName::Item_Currywurst:
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
	default:
		return 0;
	}
}

std::string GetItemString(ItemName _name) {
	switch (_name) {
	case ItemName::Item_Bier:
		return "BIER";
	case ItemName::Item_Bockwurst:
		return "BOCKWURST";
	case ItemName::Item_Mettigel:
		return "METTIGEL";
	case ItemName::Item_Currywurst:
		return "CURRYWURST";
	case ItemName::Item_StreetSheet:
		return "DIE STRAßENZEITUNG";
	case ItemName::Item_USADAY:
		return "USA DAY";
	case ItemName::Item_Sign:
		return "WOOD SIGN";
	case ItemName::Item_Poster:
		return "WALL POSTER";
	case ItemName::Item_NewsAd:
		return "NEWSPAPER AD";
	default:
		return "";
	}
}

std::string GetItemGuideDesc(ItemName _name) {
	switch (_name) {
	case ItemName::Item_Bier:
		return "                 BIER\n\n    GERMANS OF ALL WALKS OF LIFE ENJOY THIS ALCOHOLIC DRINK. YOU CAN BE SURE THAT ANYONE PASSING BY WILL ENJOY THIS FROSTY COLD BEVERAGE.\n\nPREFERRED BY :\n    EAST & WEST BERLINERS";
	case ItemName::Item_Bockwurst:
		return "               BOCKWURST\n\n    A SIMMERED SAUSAGE MADE WITH VEAL AND PORK, POPULAR IN EAST GERMANY AND OFTEN SERVED WITH A SMALL PIECE OF BREAD AND A DOLLOP OF MUSTARD. A CHEAP AND TASTY SNACK.\n\nPREFERRED BY:\n    EAST BERLINERS";
	case ItemName::Item_Mettigel:
		return "               METTIGEL\n\n    METT IS A MIXTURE OF RAW MINCED PORK, SALT, PEPPER, AND CHOPPED ONION AND IS USUALLY SERVED ON A BREAD ROLL. METTIGEL IS A FUN TAKE WHERE THE METT IS SHAPED LIKE A HEDGEHOG AND  QUARTERED ONION SLICES ARE USED AS SPIKES.\n\nPREFERRED BY:\n    EAST & WEST BERLINERS";
	case ItemName::Item_Currywurst:
		return "              CURRYWURST\n\n    A STEAMED, FRIED BRATWURST TOPPED WITH CURRYWURST SAUCE (TYPICALLY CURRY POWDER AND KETCHUP) SERVED WITH FRENCH FRIES. THIS IS A POPULAR SNACK BOTH DURING THE DAY AND LATE AT NIGHT.  COMES WITH ONE CURRYWURST FORK.\n\nPREFERRED BY:\n    WEST BERLINERS";
	case ItemName::Item_StreetSheet:
		return "          DIE STRAßENZEITUNG\n\n    \"THE STREET SHEET\" WAS WIDELY READ IN EAST GERMANY AND PROVIDED INFO ON UNDERGROUND EVENTS FROWNED ON BY THE GDR. WITH THE FALL OF THE WALL, THIS PAPER NOW REPORTS ON ALL MANNER OF  LIFE IN EAST AND WEST GERMANY.\n\nPREFERRED BY:\n    EAST BERLINERS";
	case ItemName::Item_USADAY:
		return "               USA DAY\n\n    THIS GERMAN TABLOID REPORTS ON THE RICH AND FAMOUS IN HOLLYWOOD. ITS WILD STORIES AND SALACIOUS DETAILS HAVE  MADE IT POPULAR WITH PEOPLE WHO WANT TO KNOW WHO'S WHO.\n\nPREFERRED BY:\n    WEST BERLINERS";
	case ItemName::Item_Sign:
		return "               WOOD SIGN\n\n    A SCRAP PIECE OF WOOD WITH YOUR SHOP'S NAME AND AN ARROW SCRAWLED ON IT. PERFECT FOR CHEAP MARKETING. NOT PERFECT FOR GETTING THE WORD OUT.\n\nVISIBILITY:\n    LOW";
	case ItemName::Item_Poster:
		return "              WALL POSTER\n\n    A PAPER POSTER THAT CAN BE AFFIXED TO A WALL WITH SOME GLUE. THE SHOP'S NAME IS PROMINENTLY DISPLAYED ALONG WITH THE ADDRESS IN A VERY TASTEFUL TYPEFACE.\n\nVISIBILITY:\n    MEDIUM";
	case ItemName::Item_NewsAd:
		return "             NEWSPAPER AD\n\n    GET MAXIMUM EXPOSURE WITH AN AD IN THE PAPER! YOUR SHOP WILL BE FEATURED NEXT TO THE POPULAR COMIC \"DIE FETTE ORANGE KATZE\" AND WILL BE SEEN BY ALL BERLINERS.\n\nVISIBILITY:\n    HIGH";
	default:
		return "";
	}
}

std::vector<ItemData*>* GetInitialItemVector() {
	std::vector<ItemData*>* itemVector = new std::vector<ItemData*>();
	itemVector->push_back(new ItemData(ItemName::Item_Bier));
	itemVector->push_back(new ItemData(ItemName::Item_Bockwurst));
	itemVector->push_back(new ItemData(ItemName::Item_Mettigel));
	itemVector->push_back(new ItemData(ItemName::Item_Currywurst));
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