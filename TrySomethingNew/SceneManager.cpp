#include "Assets.h"
#include "Graphics.h"
#include "SceneManager.h"
#include "Scenes\DaySales.h"
#include "Scenes\Intro.h"
#include "Scenes\MainMenu.h"
#include "Scenes\Market.h"
#include "Scenes\SalesResults.h"
#include "Scenes\Scene.h"
#include "Scenes\SetPrices.h"
#include "Scenes\TitleScreen.h"

SceneManager::SceneManager() {
	// Set SDL Graphics NULL on creation
	this->mGraphics = NULL;
	this->mAssets = NULL;
	// We're not quitting yet
	this->quitGame = false;
	//Set fullscreen off
	this->Fullscreen = false;

	// Set timer values
	currTick = 0;
	lastTick = 0;
	timeStep = 0;
}

SceneManager::~SceneManager() {
	// Quit SDL_Mixer
	Mix_FreeChunk(mAssets->sounds.Blip);
	Mix_FreeChunk(mAssets->sounds.Buzz);
	Mix_FreeChunk(mAssets->sounds.Buy);
	Mix_FreeMusic(mAssets->music.TitleMusic);
	Mix_FreeMusic(mAssets->music.IntroMusic);
	Mix_FreeMusic(mAssets->music.DaySalesMusic);
	Mix_FreeMusic(mAssets->music.JazzyMusic);
	mAssets->sounds.Blip = NULL;
	mAssets->sounds.Buzz = NULL;
	mAssets->sounds.Buy = NULL;
	mAssets->music.TitleMusic = NULL;
	mAssets->music.IntroMusic = NULL;
	mAssets->music.DaySalesMusic = NULL;
	mAssets->music.JazzyMusic = NULL;
	Mix_Quit();
	// Release/free SDL Graphics
	Graphics::Release();
	Assets::Release();
	this->mGraphics = NULL;
	this->mAssets = NULL;
}

bool SceneManager::Init() {
	printf("Init\n");

	// Initialize SDL Graphics
	this->mGraphics = Graphics::Instance();
	this->mAssets = Assets::Instance();
	
	// Initialize sound
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_AllocateChannels(16);
	Mix_Volume(1, SDL_MIX_MAXVOLUME);
	Mix_Volume(2, SDL_MIX_MAXVOLUME);
	Mix_Volume(3, SDL_MIX_MAXVOLUME);
	Mix_VolumeMusic(SDL_MIX_MAXVOLUME);
	// Load assets
	mAssets->LoadAssets();

	// Initialize player data
	this->mPlayerData = new PlayerData();
	
	// Add scenes here
	this->AddScene(new TitleScreen());
	this->AddScene(new MainMenu());
	this->AddScene(new Intro());
	this->AddScene(new Market());
	this->AddScene(new SetPrices());
	this->AddScene(new DaySales());
	this->AddScene(new SalesResults());

	// Initialize scenes
	for (int i = 0; i < (int) this->sceneList.size(); i++)
		this->sceneList[i]->Init(this);

	// Set initial scene
	this->StartScene(Scene_TitleScreen);

	// Init succeeded
	return true;
}

void SceneManager::Event(SDL_Event* Event) {
	if (Event->type == SDL_QUIT) 
		this->quitGame = true;
	else
		this->GetScene()->HandleEvent(Event);
}

void SceneManager::Render() {
	// Clear draw buffer
	this->mGraphics->ClearBackBuffer();

	// Run scene rendering func
	this->mCurrent->Render();

	// Render buffer to scene
	this->mGraphics->Render();
}

void SceneManager::StartScene(SceneName scene) {
	// Cleanup current scene if one exists
	if(this->mCurrent != nullptr)
		this->mCurrent->Cleanup();

	// Get next scene
	this->mNext = this->sceneList[scene];
	
	// Run scene start func
	this->mNext->SceneStart();
	
	// Set next scene as current
	this->mCurrent = this->mNext;
	
	// Clear next scene
	this->mNext = nullptr;

	// Run scene update
	this->Update();
}

void SceneManager::UpdateTicks() {
	// Update timeStep by # of ticks from last cycle
	this->lastTick = this->currTick;
	this->currTick = SDL_GetTicks();
	this->timeStep = this->currTick - this->lastTick;
}
