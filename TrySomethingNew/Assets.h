#pragma once
#ifndef _ASSETS_H_
#define _ASSETS_H_

#include <map>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "Graphics.h"

typedef std::map<std::string, SDL_Texture*> TextureList;
typedef std::map<std::string, TTF_Font*> FontList;

class Assets;
class ImageData;

class Assets {
private:
	static Assets* sInstance;
	TextureList	mTextures;
	FontList mFonts;
	Assets();
	~Assets();
public:
	static Assets* Instance();
	static void Release();
	void LoadAssets();
	SDL_Texture* GetTexture(std::string name);
	Mix_Chunk* GetSound(std::string name);
	Mix_Music* GetMusic(std::string name);
	TTF_Font* GetFont(std::string name, int size);

	struct Image {
		SDL_Texture* texture;
		SDL_Rect* rect;
	};

	struct Images {
		Image Title;
		Image Blank;
		// Placeholder Spritesheet images
		Image SpriteSheet1D[1];
		Image SpriteSheet2D[1][1];
	} images;

	struct Fonts {
		TTF_Font* PrintChar21_8;
	} fonts;

	struct Sounds {
		Mix_Chunk* ExampleSound;
	} sounds;

	struct Music {
		Mix_Music* TitleMusic;
		Mix_Music* ExampleMusic;
	} music;
};

class ImageData {
protected:
	Assets::Image*	mImage;
	SDL_Rect*		mDrawRect;
	double			DrawAngle;

public:
	void SetTexture(SDL_Texture* _texture) { this->mImage->texture = _texture; }
	void SetImage(Assets::Image* _image) {
		this->mImage = _image;
		this->mDrawRect = Graphics::CreateRect(this->mImage->rect->w, this->mImage->rect->h, 0, 0);
		this->DrawAngle = 0.0;
	}

	Assets::Image*	GetImage() { return this->mImage; }
	SDL_Rect*		GetDrawRect() { return this->mDrawRect; }
	double			GetDrawAngle() { return this->DrawAngle; }
	void			SetDrawRect(SDL_Rect* _drawrect) { this->mDrawRect = _drawrect; }
	void			SetDrawRectX(int _x) { this->mDrawRect->x = _x; }
	void			SetDrawRectY(int _y) { this->mDrawRect->y = _y; }
	void			SetDrawRectXY(int _x, int _y) { this->mDrawRect->x = _x; this->mDrawRect->y = _y; }
	void			SetDrawAngle(double _angle) { this->DrawAngle = _angle; }
};

#endif
