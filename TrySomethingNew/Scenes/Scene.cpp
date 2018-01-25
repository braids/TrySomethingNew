#include "Assets.h"
#include "Graphics.h"
#include "Data\PlayerData.h"
#include "Scene.h"
#include "SceneManager.h"

Scene::Scene() {
	this->SetActive(false);
	this->SetManager(nullptr);
}

Scene::~Scene() {
}

void Scene::Init(SceneManager * manager) {
	this->SetManager(manager);
	this->SetPlayerData(mManager->GetPlayerData());
}

ImageData* Scene::AddText(std::string _text, int _x, int _y) {
	// Init Image and ImageData
	ImageData* textImageData = new ImageData();
	
	// Generate text image
	textImageData->SetTexture(Graphics::Instance()->LoadText(
		this->mManager->GetAssets()->fonts.PrintChar21_8,
		_text,
		255,
		255,
		255,
		0));

	// Store text image in image data
	textImageData->CreateDrawRect();
	textImageData->SetDrawRectXY(_x, _y);
	textImageData->SetDrawAngle(0.0);
	textImageData->SetText(_text);
	// Store in scene image vector
	this->mImages.push_back(textImageData);

	return textImageData;
}

TextBox* Scene::AddTextBox(size_t _size, int _x, int _y)
{
	TextBox* textBox = new TextBox(_size, _x, _y);
	this->mImages.push_back(textBox);
	return textBox;
}

EventTimer* Scene::AddEventTimer(EventTimer* _eventTimer)
{
	this->mEventTimers.push_back(_eventTimer);
	return _eventTimer;
}

void Scene::UpdateEventTimers() {
	std::vector<EventTimer*>::iterator iter = this->mEventTimers.begin();
	for (; iter != this->mEventTimers.end(); iter++)
		if ((*iter)->isStarted()) (*iter)->Update();
}

int Scene::KeycodeNumValue(SDL_Keycode _key) {
	return (_key >= SDLK_0 && _key <= SDLK_9) ? (int)_key - (int)SDLK_0 : -1;
}