#include <iterator>
#include <random>
#include <vector>
#include "Data\Customer.h"
#include "Data\ItemData.h"

Customer::Customer(ForecastEvent _event) {
	int sideChance = rand() % 6;
	WallSide sideChosen;
	switch (_event) {
	case ForecastEvent::Event_EConcert:
		sideChosen = sideChance > 4 ? WallSide::EastBerlin : WallSide::WestBerlin;
		break;
	case ForecastEvent::Event_WConcert:
		sideChosen = sideChance > 0 ? WallSide::EastBerlin : WallSide::WestBerlin;
		break;
	case ForecastEvent::Event_EGallery:
		sideChosen = sideChance > 3 ? WallSide::EastBerlin : WallSide::WestBerlin;
		break;
	case ForecastEvent::Event_WGallery:
		sideChosen = sideChance > 1 ? WallSide::EastBerlin : WallSide::WestBerlin;
		break;
	case ForecastEvent::Event_None:
		sideChosen = sideChance > 2 ? WallSide::EastBerlin : WallSide::WestBerlin;
		break;
	default:
		sideChosen = sideChance > 2 ? WallSide::EastBerlin : WallSide::WestBerlin;
		break;
	}
	this->Side = sideChosen;
}

std::vector<ItemName>* Customer::PurchaseList(std::vector<ItemData*> _stock) {
	// Init purchased item vector
	std::vector<ItemName>* Purchased = new std::vector<ItemName>;

	// Get East Berlin iterators. Will hit east-preferred items first.
	std::vector<ItemData*>::iterator EastIterBegin = _stock.begin();
	std::vector<ItemData*>::iterator EastIterEnd = _stock.end();
	// Get West Berlin iterators. Will hit West-preferred items first.
	std::vector<ItemData*>::reverse_iterator WestIterBegin = _stock.rbegin();
	std::vector<ItemData*>::reverse_iterator WestIterEnd = _stock.rend();

	// Customer looks through current stock
	if (this->GetSide() == WallSide::EastBerlin)
		this->PurchaseItems(EastIterBegin, EastIterEnd, Purchased); // Check East Berlin items first
	else
		this->PurchaseItems(WestIterBegin, WestIterEnd, Purchased); // Check West Berlin items first

	// Returned purchased items
	return Purchased;
}

template<typename Iterator>
void Customer::PurchaseItems(Iterator Begin, Iterator End, std::vector<ItemName>* Purchased) {
	// Item type purchased flags
	bool PurchasedBeer = false;
	bool PurchasedFood = false;
	bool PurchasedNews = false;

	// Init purchase chance
	double purchaseChance;

	//Get East/West Berlin status
	bool IsEast = (this->GetSide() == WallSide::EastBerlin);
	bool IsWest = (this->GetSide() == WallSide::WestBerlin);

	// Search through item stock vector
	for (Iterator Iter = Begin; Iter != End; Iter++) {
		switch ((*Iter)->GetName()) {
		case ItemName::Item_Bier:
			// If item purchased or no stock left, skip check
			if (PurchasedBeer || (*Iter)->GetQuantity() <= 0)
				break;

			// Get initial purchase chance
			purchaseChance = 90.0;

			// 1 DM over buy value is 1/1 modifier. 2DM over is 1/1.2 modifier, and so on.
			purchaseChance *= 1.0 / (0.8 + (0.2 * ((double)(*Iter)->GetSellPrice() - (double)(*Iter)->GetBuyPrice())));

			// If random lands below purchaseChance, buy item.
			if ((rand() % 100) + 1 <= (int)purchaseChance) {
				// Item type has been purchased
				PurchasedBeer = true;

				// Add item to purchased vector
				Purchased->push_back(ItemName::Item_Bier);
			}
			break;

		case ItemName::Item_Bockwurst:
			// If item purchased or no stock left, skip check
			if (PurchasedFood || (*Iter)->GetQuantity() <= 0)
				break;

			// Get initial purchase chance
			purchaseChance = (IsEast) ? 80.0 : 50.0;

			// 1 DM over buy value is 1/1 modifier. 2DM over is 1/1.2 modifier, and so on.
			purchaseChance *= 1.0 / (0.8 + (0.2 * ((double)(*Iter)->GetSellPrice() - (double)(*Iter)->GetBuyPrice())));

			// If random lands below purchaseChance, buy item.
			if ((rand() % 100) + 1 <= (int)purchaseChance) {
				// Item type has been purchased
				PurchasedFood = true;

				// Add item to purchased vector
				Purchased->push_back(ItemName::Item_Bockwurst);
			}
			break;

		case ItemName::Item_Mettigel:
			// If item purchased or no stock left, skip check
			if (PurchasedFood || (*Iter)->GetQuantity() <= 0)
				break;

			// Get initial purchase chance
			purchaseChance = 70.0;

			// 1 DM over buy value is 1/1 modifier. 2DM over is 1/1.2 modifier, and so on.
			purchaseChance *= 1.0 / (0.8 + (0.2 * ((double)(*Iter)->GetSellPrice() - (double)(*Iter)->GetBuyPrice())));

			// If random lands below purchaseChance, buy item.
			if ((rand() % 100) + 1 <= (int)purchaseChance) {
				// Item type has been purchased
				PurchasedFood = true;

				// Add item to purchased vector
				Purchased->push_back(ItemName::Item_Mettigel);
			}
			break;

		case ItemName::Item_Currywurst:
			// If item purchased or no stock left, skip check
			if (PurchasedFood || (*Iter)->GetQuantity() <= 0)
				break;

			// Get initial purchase chance
			purchaseChance = (IsWest) ? 80.0 : 50.0;

			// 1 DM over buy value is 1/1 modifier. 2DM over is 1/1.2 modifier, and so on.
			purchaseChance *= 1.0 / (0.8 + (0.2 * ((double)(*Iter)->GetSellPrice() - (double)(*Iter)->GetBuyPrice())));

			// If random lands below purchaseChance, buy item.
			if ((rand() % 100) + 1 <= (int)purchaseChance) {
				// Item type has been purchased
				PurchasedFood = true;
				Purchased->push_back(ItemName::Item_Currywurst);
			}
			break;

		case ItemName::Item_StreetSheet:
			// If item purchased or no stock left, skip check
			if (PurchasedNews || (*Iter)->GetQuantity() <= 0)
				break;

			// Get initial purchase chance
			purchaseChance = (IsEast) ? 70.0 : 40.0;

			// 1 DM over buy value is 1/1 modifier. 2DM over is 1/1.2 modifier, and so on.
			purchaseChance *= 1.0 / (0.8 + (0.1 * ((double)(*Iter)->GetSellPrice() - (double)(*Iter)->GetBuyPrice())));

			// If random lands below purchaseChance, buy item.
			if ((rand() % 100) + 1 <= (int)purchaseChance) {
				// Item type has been purchased
				PurchasedNews = true;

				// Add item to purchased vector
				Purchased->push_back(ItemName::Item_StreetSheet);
			}
			break;

		case ItemName::Item_USADAY:
			// If item purchased or no stock left, skip check
			if (PurchasedNews || (*Iter)->GetQuantity() <= 0)
				break;

			// Get initial purchase chance
			purchaseChance = (IsWest) ? 70.0 : 40.0;

			// 1 DM over buy value is 1/1 modifier. 2DM over is 1/1.2 modifier, and so on.
			purchaseChance *= 1.0 / (0.8 + (0.1 * ((double)(*Iter)->GetSellPrice() - (double)(*Iter)->GetBuyPrice())));

			// If random lands below purchaseChance, buy item.
			if ((rand() % 100) + 1 <= (int)purchaseChance) {
				// Item type has been purchased
				PurchasedNews = true;

				// Add item to purchased vector
				Purchased->push_back(ItemName::Item_USADAY);
			}
			break;

		default:
			break;
		}
	}
}
