#pragma once
#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "Graphics.h"
#include "Assets.h"

// Forward dec classes
class Position;
class ImageData;
class GameObject;

enum ObjectID {
	Object_SomeObject
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

class ImageData {
protected:
	Assets::Image*	mImage;
	SDL_Rect*		mDrawRect;
	double			DrawAngle;

public:
	void SetImage(Assets::Image* _image) { 
		this->mImage = _image; 
		this->mDrawRect = Graphics::CreateRect(0, 0, 0, 0);
		this->mDrawRect->w = this->mImage->rect->w;
		this->mDrawRect->h = this->mImage->rect->h;
	}
	
	Assets::Image*	GetImage() { return this->mImage; }
	SDL_Rect*		GetDrawRect() { return this->mDrawRect; }
	double			GetDrawAngle() { return this->DrawAngle; }
	void			SetDrawRect(SDL_Rect* _drawrect) { this->mDrawRect = _drawrect; }
	void			SetDrawAngle(double _angle) { this->DrawAngle = _angle; }
};

class GameObject {
protected:
	Position mPosition;
	ObjectID	mObjectId;
	ImageData	mImageData;
	
public:
	GameObject();
	~GameObject();

	Position* GetPosition() { return &this->mPosition; }
	ImageData* GetImageData() { return &this->mImageData; }
	ObjectID GetObjectID() { return this->mObjectId; }
	void SetDrawPos(Uint32 _x, Uint32 _y);
	
	virtual void Update() = 0;	
};

class SomeObject : public GameObject {
public:
	SomeObject();

	// Funcs
	void Update();
};

#endif