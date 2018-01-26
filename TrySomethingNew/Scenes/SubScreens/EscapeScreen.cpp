#include <string>
#include <vector>
#include "Scenes\Scene.h"
#include "Scenes\SubScreens\EscapeScreen.h"

EscapeScreen::EscapeScreen() {
	this->EscapeScreenVisible = false;
}

void EscapeScreen::AddEscapeText(Scene* _scene) {
	this->EscapeText.push_back(_scene->AddText("QUIT GAME?", 105, 63));
	this->EscapeText.push_back(_scene->AddText("   Y)ES", 105, 81));
	this->EscapeText.push_back(_scene->AddText("   N)O", 105, 99));
}

void EscapeScreen::ShowEscapeText() {
	for (std::vector<ImageData*>::iterator it = this->EscapeText.begin(); it != this->EscapeText.end(); it++)
		(*it)->SetVisible(true);
}

void EscapeScreen::HideEscapeText() {
	for (std::vector<ImageData*>::iterator it = this->EscapeText.begin(); it != this->EscapeText.end(); it++)
		(*it)->SetVisible(false);
}
