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