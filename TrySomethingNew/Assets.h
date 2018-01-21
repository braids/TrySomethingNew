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
		Image JamLogo;
		Image ThemeLogo;
		Image Wall1;
		Image Wall2;
		Image Shack1;
		Image Shack2;
		Image Shack3;
		Image Customer;
		Image ECustomer[4];
		Image WCustomer[4];
	} images;

	struct Fonts {
		TTF_Font* PrintChar21_8;
	} fonts;

	struct Sounds {
		Mix_Chunk* ExampleSound;
	} sounds;

	struct Music {
		Mix_Music* TitleMusic;
		Mix_Music* IntroMusic;
	} music;
};

class ImageData {
protected:
	Assets::Image*	mImage;
	SDL_Rect*		mDrawRect;
	double*			DrawAngle;
	std::string*	ImageText;
	bool			Visible;
public:
	ImageData() {
		this->mImage = new Assets::Image();
		this->DrawAngle = new double(0.0);
		this->ImageText = new std::string("");
		this->Visible = true;
	}
	
	// Texture methods
	void SetTexture(SDL_Texture* _texture) { 
		// Set image texture
		this->mImage->texture = _texture; 
		// If no image rect exists, create one
		if (this->mImage->rect == nullptr)
			this->mImage->rect = Graphics::CreateRect(0, 0, 0, 0);
		// Set image rect
		SDL_QueryTexture(this->mImage->texture, NULL, NULL, &(this->mImage->rect->w), &(this->mImage->rect->h));
	}
	// Image methods
	void SetImage(Assets::Image* _image) {
		this->mImage = _image;
		this->mDrawRect = Graphics::CreateRect(this->mImage->rect->w, this->mImage->rect->h, 0, 0);
		*(this->DrawAngle) = 0.0;
	}
	Assets::Image*	GetImage() { return this->mImage; }
	// DrawRect methods
	SDL_Rect*		GetDrawRect() { return this->mDrawRect; }
	void			CreateDrawRect() { this->mDrawRect = Graphics::CreateRect(this->mImage->rect->w, this->mImage->rect->h, 0, 0); }
	void			UpdateDrawRect() { this->mDrawRect->w = this->mImage->rect->w; this->mDrawRect->h = this->mImage->rect->h; }
	void			SetDrawRect(SDL_Rect* _drawrect) { this->mDrawRect = _drawrect; }
	void			SetDrawRectX(int _x) { this->mDrawRect->x = _x; }
	void			SetDrawRectY(int _y) { this->mDrawRect->y = _y; }
	void			SetDrawRectXY(int _x, int _y) { this->mDrawRect->x = _x; this->mDrawRect->y = _y; }
	// DrawAngle methods
	double			GetDrawAngle() { return *(this->DrawAngle); }
	void			SetDrawAngle(double _angle) { *(this->DrawAngle) = _angle; }
	// ImageText methods
	std::string*	GetText() { return this->ImageText; }
	void			SetText(std::string _text) { 
		*(this->ImageText) = _text; 
		this->SetTexture(Graphics::Instance()->LoadText(
			Assets::Instance()->fonts.PrintChar21_8, _text, 255, 255, 255, 0));
		// Update draw rect
		this->UpdateDrawRect();
	}
	// Visibility methods
	bool			IsVisible() { return this->Visible; }
	void			SetVisible(bool _visible) { this->Visible = _visible; }

};

class TextBox : public ImageData {
protected:
	size_t MaxLength;
	bool Active;
public:
	TextBox() {
		this->SetDrawRect(Graphics::CreateRect(0, 0, 0, 0));
		this->MaxLength = 0;
		this->Active = false;
	}
	TextBox(int _size, int _x, int _y) {
		this->SetDrawRect(Graphics::CreateRect(0, 0, _x, _y));
		this->MaxLength = _size;
		this->Active = false;
	}
	void AppendText(std::string _text) {
		// If there is space in the text box and the box is active, append text and update
		if (this->ImageText->length() < this->MaxLength && this->IsActive()) {
			this->ImageText->append(_text);
			this->UpdateTextBox();
		}
	}
	void DeleteText() {
		// If the text box has characters and is active, delete the last character and update
		if (this->ImageText->length() > 0 && this->IsActive()) {
			this->ImageText->pop_back();
			this->UpdateTextBox();
		}
	}
	void UpdateTextBox() {
		// Get current image text
		std::string text = *this->GetText();
		// If less than max length and text box is active, append a *
		if (text.length() < this->MaxLength && this->IsActive())
			text.append("*");
		// Set image texture
		this->SetTexture(Graphics::Instance()->LoadText(
			Assets::Instance()->fonts.PrintChar21_8, text, 255,	255, 255, 0));
		// Update draw rect
		this->UpdateDrawRect();
	}
	bool IsActive() { return this->Active; }
	void SetActive(bool _active) { this->Active = _active; this->UpdateTextBox(); }
};

#endif
