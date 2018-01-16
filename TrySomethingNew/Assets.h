#pragma once
#ifndef _ASSETS_H_
#define _ASSETS_H_

#include <map>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "Graphics.h"

typedef std::map<std::string, SDL_Texture*> TextureList;
typedef std::map<std::string, TTF_Font*> FontList;

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

#endif
