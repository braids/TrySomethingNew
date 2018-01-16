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
	ImageData* textImageData = new ImageData();
	
	// Generate text image
	textImageData->GetImage()->texture = this->mManager->GetGraphics()->LoadText(
			this->mManager->GetAssets()->fonts.PrintChar21_8,
			_text,
			255,
			255,
			255,
			0);
	textImageData->GetImage()->rect = Graphics::CreateRect(0, 0, 0, 0);
	SDL_QueryTexture(textImageData->GetImage()->texture, NULL, NULL, &(textImageData->GetImage()->rect->w), &(textImageData->GetImage()->rect->h));
	
	// Store text image in image data
	textImageData->CreateDrawRect();
	textImageData->SetDrawRectXY(_x, _y);
	textImageData->SetDrawAngle(0.0);
	textImageData->SetText(_text);
	// Store in scene image vector
	this->mImages.push_back(textImageData);

	return textImageData;
}

void Scene::UpdateText(ImageData * textImage) {
	// Update texture with new text
	textImage->SetTexture(this->mManager->GetGraphics()->LoadText(
		this->mManager->GetAssets()->fonts.PrintChar21_8,
		*(textImage->GetText()),
		255,
		255,
		255,
		0));
	// Set new image rect size
	SDL_QueryTexture(textImage->GetImage()->texture, NULL, NULL, &(textImage->GetImage()->rect->w), &(textImage->GetImage()->rect->h));
	textImage->UpdateDrawRect();
}

