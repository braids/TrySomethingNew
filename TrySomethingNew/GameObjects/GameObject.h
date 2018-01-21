#pragma once
#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "Assets.h"
#include "Data\Customer.h"
#include "Graphics.h"

// Forward dec classes
class Position;
class ImageData;
class GameObject;

enum ObjectID {
	Object_Customer
};

class Position {
protected:
	double x, y, dx, dy, ang, dang;

public:
	void SetPosition(double _x, double _y) { this->x = _x; this->y = _y; }
	void SetdPosition(double _dx, double _dy) { this->dx = _dx; this->dy = _dy; }
	void SetX(double _x) { this->x = _x; }
	void SetY(double _y) { this->y = _y; }
	void SetdX(double _dx) { this->dx = _dx; }
	void SetdY(double _dy) { this->dy = _dy; }
	void SetAngle(double _ang) { this->ang = _ang; }
	void SetdAngle(double _dang) { this->dang = _dang; }
	double GetX() { return this->x; }
	double GetY() { return this->y; }
	double GetdX() { return this->dx; }
	double GetdY() { return this->dy; }
	double GetAngle() { return this->ang; }
	double GetdAngle() { return this->dang; }
};

class GameObject {
protected:
	Position	mPosition;
	ObjectID	mObjectId;
	ImageData	mImageData;
	
public:
	GameObject();
	~GameObject();

	Position* GetPosition() { return &this->mPosition; }
	ImageData* GetImageData() { return &this->mImageData; }
	ObjectID GetObjectID() { return this->mObjectId; }
	void SetDrawPos(Uint32 _x, Uint32 _y);
	
	virtual void Update(Uint32 ticks) = 0;	
};

class CustomerObject : public GameObject {
protected:
	WallSide Side;

	int StartPos;
	int ShopPos;
	int EndPos;
	int ShopTime = 500;
	int ShopDuration;

	bool Active;
	bool Purchased;
	bool WalkIn;
	bool Shopping;
	bool WalkOut;

public:
	CustomerObject(WallSide _side);
	void SetActive(bool _active);
	void SetPurchased(bool _purchase);
	bool HasPurchased();
	bool IsShopping();
	// Funcs
	void Update(Uint32 ticks);
};

#endif