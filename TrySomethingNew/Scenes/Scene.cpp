#include "Assets.h"
#include "Graphics.h"
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
}

ImageData* Scene::AddText(std::string _text, int _x, int _y) {
	// Init Image and ImageData
	Assets::Image* textImage = new Assets::Image();
	ImageData* textImageData = new ImageData();
	
	// Generate text image
	textImage->texture = this->mManager->GetGraphics()->LoadText(
			this->mManager->GetAssets()->fonts.PrintChar21_8,
			_text,
			255,
			255,
			255,
			0);
	textImage->rect = Graphics::CreateRect(0, 0, 0, 0);
	SDL_QueryTexture(textImage->texture, NULL, NULL, &(textImage->rect->w), &(textImage->rect->h));
	
	// Store text image in image data
	textImageData->SetImage(textImage);
	textImageData->SetDrawRectXY(_x, _y);
	
	// Store in scene image vector
	this->mImages.push_back(textImageData);
}

