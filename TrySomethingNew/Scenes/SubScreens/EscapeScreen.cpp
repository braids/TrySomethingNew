#include <string>
#include <vector>
#include "Scenes\Scene.h"
#include "Scenes\SubScreens\EscapeScreen.h"

EscapeScreen::EscapeScreen() {
	this->EscapeScreenVisible = false;
}

void EscapeScreen::AddEscapeText(Scene* _scene) {
	this->EscapeScreenVisible = false;
	this->EscapeImagesText.push_back(_scene->AddImage(&Assets::Instance()->images.Blank, 0, 0));
	this->EscapeImagesText.push_back(_scene->AddText("QUIT GAME?", 105, 63));
	this->EscapeImagesText.push_back(_scene->AddText("   Y)ES", 105, 81));
	this->EscapeImagesText.push_back(_scene->AddText("   N)O", 105, 99));
}

void EscapeScreen::ShowEscapeText() {
	for (std::vector<ImageData*>::iterator it = this->EscapeImagesText.begin(); it != this->EscapeImagesText.end(); it++)
		(*it)->SetVisible(true);
}

void EscapeScreen::HideEscapeText() {
	for (std::vector<ImageData*>::iterator it = this->EscapeImagesText.begin(); it != this->EscapeImagesText.end(); it++)
		(*it)->SetVisible(false);
}
