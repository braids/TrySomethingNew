#pragma once
#ifndef _CUSTOMER_H_
#define _CUSTOMER_H_

#include <vector>
#include "Data\ItemData.h"

enum WallSide;
class Customer;
class EastBerliner;
class WestBerliner;

enum WallSide {
	EastBerlin,
	WestBerlin
};

class Customer {
private:
	WallSide Side;

public:
	Customer();

	WallSide GetSide() { return this->Side; }
	std::vector<ItemName>* PurchaseList(std::vector<ItemData*> _stock);

	template <typename Iterator>
	void PurchaseItems(Iterator Iter, Iterator End, std::vector<ItemName>* Purchased);
};

#endif