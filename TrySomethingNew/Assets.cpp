#include "Assets.h"

// Assets instance
Assets* Assets::sInstance = NULL;

// Assets instance ctor
Assets* Assets::Instance() {
	if (sInstance == NULL)
		sInstance = new Assets();

	return sInstance;
}

// Assets cleanup
void Assets::Release() {
	delete sInstance;
	sInstance = NULL;
}

void Assets::LoadAssets() {
	this->images.Title = { Assets::Instance()->GetTexture("title.png"), Graphics::FullscreenRect() };
	this->images.Blank = { Assets::Instance()->GetTexture("blank.png"), Graphics::FullscreenRect() };
	this->images.JamLogo = { Assets::Instance()->GetTexture("JamLogo.png"), Graphics::CreateRect(124, 70, 0, 0) };
	this->images.ThemeLogo = { Assets::Instance()->GetTexture("ThemeLogo.png"), Graphics::CreateRect(119, 70, 0, 0) };
	this->images.Wall1 = { Assets::Instance()->GetTexture("wall1.png"), Graphics::CreateRect(280, 162, 0, 0) };
	this->images.Wall2 = { Assets::Instance()->GetTexture("wall2.png"), Graphics::CreateRect(280, 162, 0, 0) };
	this->images.Shack1 = { Assets::Instance()->GetTexture("shack1.png"), Graphics::CreateRect(280, 96, 0, 0) };
	this->images.Shack2 = { Assets::Instance()->GetTexture("shack2.png"), Graphics::CreateRect(280, 96, 0, 0) };
	this->images.Shack3 = { Assets::Instance()->GetTexture("shack3.png"), Graphics::CreateRect(280, 96, 0, 0) };
	this->images.Customer = { Assets::Instance()->GetTexture("customer_stand.png"), Graphics::CreateRect(14, 27, 0, 0) };
	for (int i = 0; i < 4; i++) this->images.ECustomer[i] = { Assets::Instance()->GetTexture("ecustomer_walk.png"), Graphics::CreateRect(14, 27, i * 14, 0) };
	for (int i = 0; i < 4; i++) this->images.WCustomer[i] = { Assets::Instance()->GetTexture("wcustomer_walk.png"), Graphics::CreateRect(14, 27, i * 14, 0) };
	this->sounds.Buzz = Assets::Instance()->GetSound("buzz.wav");
	this->sounds.Blip = Assets::Instance()->GetSound("blip.wav");
	this->music.TitleMusic = Assets::Instance()->GetMusic("title.ogg");
	this->music.IntroMusic = Assets::Instance()->GetMusic("intro.ogg");
	this->fonts.PrintChar21_8 = Assets::Instance()->GetFont("PrintChar21.ttf", 8);
}

// Assets ctor
Assets::Assets() {

}

// Assets dtor. Frees all texture memory.
Assets::~Assets() {
	for (auto tex : mTextures) {
		if (tex.second != NULL)
			SDL_DestroyTexture(tex.second);
	}

	mTextures.clear();
}

// Load texture from file if not in texture map, then return pointer to texture in texture map.
SDL_Texture* Assets::GetTexture(std::string name) {
	std::string fullPath = SDL_GetBasePath();
	fullPath.append("res/img/" + name);

	if (mTextures[fullPath] == NULL)
		mTextures[fullPath] = Graphics::Instance()->LoadTexture(fullPath);

	return mTextures[fullPath];
}

Mix_Chunk* Assets::GetSound(std::string name) {
	std::string fullPath = SDL_GetBasePath();
	fullPath.append("res/snd/" + name);

	return Mix_LoadWAV(fullPath.c_str());
}

Mix_Music* Assets::GetMusic(std::string name) {
	std::string fullPath = SDL_GetBasePath();
	fullPath.append("res/snd/" + name);

	return Mix_LoadMUS(fullPath.c_str());
}

TTF_Font* Assets::GetFont(std::string name, int size) {
	std::string fullPath = SDL_GetBasePath();
	fullPath.append("res/font/" + name);
	std::string fullPathSize = fullPath;
	fullPathSize.append(std::to_string(size));

	if (mFonts[fullPathSize] == NULL)
		mFonts[fullPathSize] = Graphics::Instance()->LoadFont(fullPath, size);

	return mFonts[fullPathSize];
}