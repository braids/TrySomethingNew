#include "Assets.h"
#include "Graphics.h"
#include "Scene.h"

Scene::Scene() {
	this->mAssets = NULL;
	this->SetActive(false);
	this->SetManager(nullptr);
}

Scene::~Scene() {
	Assets::Release();
	mAssets = NULL;
}

void Scene::Init(SceneManager * manager) {
	this->mAssets = Assets::Instance();
	this->SetManager(manager);
}
