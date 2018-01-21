#include "Assets.h"
#include "Camera.h"
#include "Data\ItemData.h"
#include "Data\PlayerData.h"
#include "GameObjects\GameObject.h"
#include "Graphics.h"
#include "Scenes\Scene.h"
#include "SceneManager.h"

Market::Market() {
	// Set current scene name
	this->SetSceneName(Scene_Market);
}

void Market::ResetFlags() {
	// Set flags to false
	this->EventFlags.ExitToTitleScreen = false;
	this->EventFlags.MainSelection = false;
	this->EventFlags.SelectBuyItem = false;
	this->EventFlags.EnterItemQty = false;
	this->EventFlags.ShowForecast = false;
	this->EventFlags.SelectGuideItem = false;
	this->EventFlags.ShowGuide = false;
}

void Market::LoadGameObjects() {
	this->mGameObjects.clear();
}

void Market::LoadEventTimers() {
	this->mEventTimers.clear();

	// Init event timers
}

void Market::LoadImagesText() {
	// Clear any existing drawn text.
	this->mImages.clear();
	this->MarketText.clear();

	// Image objects

	// Text objects
	// Market Title
	this->TextObjects.MarketTitle = this->AddMarketText("DER MORGENMARKT", 91, 9);
	// Purchase signs
	this->TextObjects.MultSigns = this->AddMarketText("x\nx\nx\nx\n\nx\nx\n\nx\nx\nx", 196, 36);
	this->TextObjects.EqualSigns = this->AddMarketText("=\n=\n=\n=\n\n=\n=\n\n=\n=\n=", 231, 36);
	// Items
	this->TextObjects.Foods = this->AddMarketText("-FOODS-", 21, 27);
	this->TextObjects.Cost = this->AddMarketText("COST", 161, 27);
	this->TextObjects.Qty = this->AddMarketText("QTY", 203, 27);
	this->TextObjects.Total = this->AddMarketText("TOTAL", 238, 27);
	this->TextObjects.BierName = this->AddMarketText("1) BIER", 7, 36);
	this->TextObjects.BierCost = this->AddMarketText("DM 4", 161, 36);
	this->TextObjects.BierTotal = this->AddMarketText("0", 245, 36);
	this->TextObjects.BockwurstName = this->AddMarketText("2) BOCKWURST", 7, 45);
	this->TextObjects.BockwurstCost = this->AddMarketText("DM 2", 161, 45);
	this->TextObjects.BockwurstTotal = this->AddMarketText("0", 245, 45);
	this->TextObjects.MettigelName = this->AddMarketText("3) METTIGEL", 7, 54);
	this->TextObjects.MettigelCost = this->AddMarketText("DM 4", 161, 54);
	this->TextObjects.MettigelTotal = this->AddMarketText("0", 245, 54);
	this->TextObjects.CurrywurstName = this->AddMarketText("4) CURRYWURST", 7, 63);
	this->TextObjects.CurrywurstCost = this->AddMarketText("DM 6", 161, 63);
	this->TextObjects.CurrywurstTotal = this->AddMarketText("0", 245, 63);
	this->TextObjects.Newspapers = this->AddMarketText("-NEWSPAPERS-", 21, 72);
	this->TextObjects.StreetSheetName = this->AddMarketText("5) DIE STRAßENZEITUNG", 7, 81);
	this->TextObjects.StreetSheetCost = this->AddMarketText("DM 3", 161, 81);
	this->TextObjects.StreetSheetTotal = this->AddMarketText("0", 245, 81);
	this->TextObjects.USADAYName = this->AddMarketText("6) USA DAY", 7, 90);
	this->TextObjects.USADAYCost = this->AddMarketText("DM 7", 161, 90);
	this->TextObjects.USADAYTotal = this->AddMarketText("0", 245, 90);
	// Ads
	this->TextObjects.Ads = this->AddMarketText("-ADS-", 21, 99);
	this->TextObjects.SignName = this->AddMarketText("7) WOOD SIGN", 7, 108);
	this->TextObjects.SignCost = this->AddMarketText("DM 5", 161, 108);
	this->TextObjects.SignTotal = this->AddMarketText("0", 245, 108);
	this->TextObjects.PosterName = this->AddMarketText("8) WALL POSTER", 7, 117);
	this->TextObjects.PosterCost = this->AddMarketText("DM10", 161, 117);
	this->TextObjects.PosterTotal = this->AddMarketText("0", 245, 117);
	this->TextObjects.NewsAdName = this->AddMarketText("9) NEWSPAPER AD", 7, 126);
	this->TextObjects.NewsAdCost = this->AddMarketText("DM15", 161, 126);
	this->TextObjects.NewsAdTotal = this->AddMarketText("0", 245, 126);
	// Total
	this->TextObjects.PlayerMoneyText = this->AddMarketText("MONEY:", 182, 144);
	this->TextObjects.PlayerMoneyAmount = this->AddMarketText("0", 231, 144);
	this->TextObjects.TotalText = this->AddMarketText("- TOTAL:", 168, 153);
	this->TextObjects.TotalAmount = this->AddMarketText("0", 231, 153);
	this->TextObjects.MoneySubTotalText = this->AddMarketText("=", 217, 162);
	this->TextObjects.MoneySubTotalAmount = this->AddMarketText("0", 231, 162);
	// Selection
	this->TextObjects.SelectItem = this->AddText("- SELECT ITEM #", 7, 153);
	this->TextObjects.EnterQty = this->AddText("- ENTER QUANTITY", 7, 153);
	// Options
	this->TextObjects.BuyOption = this->AddMarketText("B)UY", 7, 180);
	this->TextObjects.ForecastOption = this->AddMarketText("F)ORECAST", 49, 180);
	this->TextObjects.GuideOption = this->AddMarketText("G)UIDE", 126, 180);
	this->TextObjects.LeaveOption = this->AddMarketText("L)EAVE", 182, 180);
	this->TextObjects.SaveOption = this->AddMarketText("S)AVE", 238, 180);
	// Press Return
	this->TextObjects.PressReturn = this->AddText("- PRESS RETURN -", 84, 180);
	// Forecast
	this->TextObjects.WeatherHeader = this->AddText("WEATHER FORECAST:", 84, 27);
	this->TextObjects.WeatherInfo = this->AddText("", 7, 45);
	this->TextObjects.EventHeader = this->AddText("LOCAL EVENTS:", 97, 90);
	this->TextObjects.EventInfo = this->AddText("", 7, 108);
	// Guide Descriptions
	this->TextObjects.BierDesc = this->AddText("                 BIER\n\n    GERMANS OF ALL WALKS OF LIFE ENJOY THIS ALCOHOLIC DRINK. YOU CAN BE SURE THAT ANYONE PASSING BY WILL ENJOY THIS FROSTY COLD BEVERAGE.\n\nPREFERRED BY :\n    EAST & WEST BERLINERS", 7, 9);
	this->TextObjects.BockwurstDesc = this->AddText("               BOCKWURST\n\n    A SIMMERED SAUSAGE MADE WITH VEAL AND PORK, POPULAR IN EAST GERMANY AND OFTEN SERVED WITH A SMALL PIECE OF BREAD AND A DOLLOP OF MUSTARD. A CHEAP AND TASTY SNACK.\n\nPREFERRED BY:\n    EAST BERLINERS", 7, 9);
	this->TextObjects.MettigelDesc = this->AddText("               METTIGEL\n\n    METT IS A MIXTURE OF RAW MINCED PORK, SALT, PEPPER, AND CHOPPED ONION AND IS USUALLY SERVED ON A BREAD ROLL. METTIGEL IS A FUN TAKE WHERE THE METT IS SHAPED LIKE A HEDGEHOG AND  QUARTERED ONION SLICES ARE USED AS SPIKES.\n\nPREFERRED BY:\n    EAST & WEST BERLINERS", 7, 9);
	this->TextObjects.CurrywurstDesc = this->AddText("              CURRYWURST\n\n    A STEAMED, FRIED BRATWURST TOPPED WITH CURRYWURST SAUCE (TYPICALLY CURRY POWDER AND KETCHUP) SERVED WITH FRENCH FRIES. THIS IS A POPULAR SNACK BOTH DURING THE DAY AND LATE AT NIGHT.  COMES WITH ONE CURRYWURST FORK.\n\nPREFERRED BY:\n    WEST BERLINERS", 7, 9);
	this->TextObjects.StreetSheetDesc = this->AddText("          DIE STRAßENZEITUNG\n\n    \"THE STREET SHEET\" WAS WIDELY READ IN EAST GERMANY AND PROVIDED INFO ON UNDERGROUND EVENTS FROWNED ON BY THE GDR. WITH THE FALL OF THE WALL, THIS PAPER NOW REPORTS ON ALL MANNER OF  LIFE IN EAST AND WEST GERMANY.\n\nPREFERRED BY:\n    EAST BERLINERS", 7, 9);
	this->TextObjects.USADAYDesc = this->AddText("               USA DAY\n\n    THIS GERMAN TABLOID REPORTS ON THE RICH AND FAMOUS IN HOLLYWOOD. ITS WILD STORIES AND SALACIOUS DETAILS HAVE  MADE IT POPULAR WITH PEOPLE WHO WANT TO KNOW WHO'S WHO.\n\nPREFERRED BY:\n    WEST BERLINERS", 7, 9);
	this->TextObjects.SignDesc = this->AddText("               WOOD SIGN\n\n    A SCRAP PIECE OF WOOD WITH YOUR SHOP'S NAME AND AN ARROW SCRAWLED ON IT. PERFECT FOR CHEAP MARKETING. NOT PERFECT FOR GETTING THE WORD OUT.\n\nVISIBILITY:\n    LOW", 7, 9);
	this->TextObjects.PosterDesc = this->AddText("              WALL POSTER\n\n    A PAPER POSTER THAT CAN BE AFFIXED TO A WALL WITH SOME GLUE. THE SHOP'S NAME IS PROMINENTLY DISPLAYED ALONG WITH THE ADDRESS IN A VERY TASTEFUL TYPEFACE.\n\nVISIBILITY:\n    MEDIUM", 7, 9);
	this->TextObjects.NewsAdDesc = this->AddText("             NEWSPAPER AD\n\n    GET MAXIMUM EXPOSURE WITH AN AD IN THE PAPER! YOUR SHOP WILL BE FEATURED NEXT TO THE POPULAR COMIC \"DIE FETTE ORANGE KATZE\" AND WILL BE SEEN BY ALL BERLINERS.\n\nVISIBILITY:\n    HIGH", 7, 9);
	// Buy quantity text boxes
	this->TextBoxObjects.BierQty = this->AddMarketTextBox(2, 210, 36);
	this->TextBoxObjects.BockwurstQty = this->AddMarketTextBox(2, 210, 45);
	this->TextBoxObjects.MettigelQty = this->AddMarketTextBox(2, 210, 54);
	this->TextBoxObjects.CurrywurstQty = this->AddMarketTextBox(2, 210, 63);
	this->TextBoxObjects.StreetSheetQty = this->AddMarketTextBox(2, 210, 81);
	this->TextBoxObjects.USADAYQty = this->AddMarketTextBox(2, 210, 90);
	this->TextBoxObjects.SignQty = this->AddMarketTextBox(2, 210, 108);
	this->TextBoxObjects.PosterQty = this->AddMarketTextBox(2, 210, 117);
	this->TextBoxObjects.NewsAdQty = this->AddMarketTextBox(2, 210, 126);

	for (std::vector<ImageData*>::iterator it = this->mImages.begin(); it != this->mImages.end(); it++)
		(*it)->SetVisible(false);
}

void Market::SceneStart() {
	// Play Market music
	Mix_HaltMusic();
	
	// Reset Flags
	this->ResetFlags();

	// Start with text entry off
	SDL_StopTextInput();

	// Load Game Objects
	this->LoadGameObjects();
	// Load Event Timers
	this->LoadEventTimers();
	// Load Images and Text Images
	this->LoadImagesText();

	// Display main market text
	this->SEvent_ShowMarketText();
	this->EventFlags.MainSelection = true;

	// Initialize buy vector
	this->BuyData = *GetInitialItemVector();
	this->BuyTotal = 0;

	// Set player money text
	this->TextObjects.PlayerMoneyAmount->SetText(std::to_string(this->mPlayerData->GetMoney()));

	// Set Forecast
	this->mPlayerData->GenerateForecast();
}

void Market::HandleEvent(SDL_Event * Event) {
	switch (Event->type) {
	case SDL_KEYDOWN:
		//// Exit event
		if (Event->key.keysym.sym == SDLK_ESCAPE) 
			this->EventFlags.ExitToTitleScreen = true;
		
		//// Main selection
		if (this->EventFlags.MainSelection) {
			if (Event->key.keysym.sym == SDLK_b)
				this->SEvent_SelectBuy();
			if (Event->key.keysym.sym == SDLK_f)
				this->SEvent_SelectForecast();
			if (Event->key.keysym.sym == SDLK_g)
				this->SEvent_SelectGuide();
			if (Event->key.keysym.sym == SDLK_l)
				this->SEvent_Leave();
		}

		//// Buy
		// Selecting item to buy
		if (this->EventFlags.SelectBuyItem) {
			this->SEvent_SetBuyItem(Event->key.keysym.sym);
		}
		// Enterting item quantity
		if (this->EventFlags.EnterItemQty) {
			if(Event->key.keysym.sym == SDLK_BACKSPACE)
				this->ActiveBuySelection->DeleteText();
			if (Event->key.keysym.sym == SDLK_RETURN)
				this->SEvent_EndItemQtyEntry();
		}

		//// Forecast
		// Exiting Forecast screen
		if (this->EventFlags.ShowForecast) {
			if (Event->key.keysym.sym == SDLK_RETURN)
				this->SEvent_ExitForecast();
		}

		//// Guide
		// Selecting item to inspect
		if (this->EventFlags.SelectGuideItem) {
			this->SEvent_SetGuideItem(Event->key.keysym.sym);
		}
		// Exiting Guide screen
		if (this->EventFlags.ShowGuide) {
			if (Event->key.keysym.sym == SDLK_RETURN)
				this->SEvent_ExitGuide();
		}

		break;

	case SDL_KEYUP:
		break;

	case SDL_TEXTINPUT:
		if (this->EventFlags.EnterItemQty) {
			this->ActiveBuySelection->AppendText(Event->text.text);
		}
		break;

	case SDL_TEXTEDITING:
		break;

	default:
		break;
	}
}

void Market::Update(Uint32 timeStep) {
	// Update timers
	this->UpdateEventTimers();

	// Return to title screen if quitting
	if (this->EventFlags.ExitToTitleScreen) {
		// Stop current music
		Mix_HaltMusic();
		// Go to title.
		this->mManager->StartScene(Scene_TitleScreen);
	}
}

void Market::Render() {
	// Render graphics to buffer
	// If I find any game logic in here, I'll slap myself silly
	for (int i = 0; i < (int) this->mImages.size(); i++) {
		if (this->mImages[i]->IsVisible())
			this->mManager->GetGraphics()->DrawTextureAtLocation(
				this->mImages[i]->GetImage()->texture,
				this->mImages[i]->GetImage()->rect,
				this->mImages[i]->GetDrawRect(),
				this->mImages[i]->GetDrawAngle()
			);
	}
}

void Market::SEvent_ShowMarketText() {
	for (std::vector<ImageData*>::iterator it = this->MarketText.begin(); it != this->MarketText.end(); it++)
		(*it)->SetVisible(true);
}

void Market::SEvent_HideMarketText() {
	for (std::vector<ImageData*>::iterator it = this->MarketText.begin(); it != this->MarketText.end(); it++)
		(*it)->SetVisible(false);
}

void Market::SEvent_ShowForecast() {
	this->TextObjects.WeatherHeader->SetVisible(true);
	this->TextObjects.WeatherInfo->SetText(GetWeatherDesc(this->mPlayerData->GetWeatherForecast()));
	this->TextObjects.WeatherInfo->SetVisible(true);
	this->TextObjects.EventHeader->SetVisible(true);
	this->TextObjects.EventInfo->SetText(GetEventDesc(this->mPlayerData->GetEventForecast()));
	this->TextObjects.EventInfo->SetVisible(true);
	this->TextObjects.PressReturn->SetVisible(true);
}

void Market::SEvent_HideForecast() {
	this->TextObjects.WeatherHeader->SetVisible(false);
	this->TextObjects.WeatherInfo->SetVisible(false);
	this->TextObjects.EventHeader->SetVisible(false);
	this->TextObjects.EventInfo->SetVisible(false);
	this->TextObjects.PressReturn->SetVisible(false);
}

void Market::SEvent_ShowGuide() {
	this->ActiveGuideText->SetVisible(true);
	this->TextObjects.PressReturn->SetVisible(true);
}

void Market::SEvent_HideGuide() {
	this->ActiveGuideText->SetVisible(false);
	this->TextObjects.PressReturn->SetVisible(false);
}

void Market::SEvent_SelectBuy() {
	this->EventFlags.MainSelection = false;
	this->TextObjects.SelectItem->SetVisible(true);
	this->EventFlags.SelectBuyItem = true;
}

void Market::SEvent_SetBuyItem(SDL_Keycode _key) {
	// Select item to purchase
	switch (_key) {
	case SDLK_1:
		this->ActiveBuySelection = this->TextBoxObjects.BierQty;
		this->ActiveItemData = GetItemFromVector(ItemName::Item_Bier, &this->BuyData);
		this->ActiveBuySubTotal = this->TextObjects.BierTotal;
		break;
	case SDLK_2:
		this->ActiveBuySelection = this->TextBoxObjects.BockwurstQty;
		this->ActiveItemData = GetItemFromVector(ItemName::Item_Bockwurst, &this->BuyData);
		this->ActiveBuySubTotal = this->TextObjects.BockwurstTotal;
		break;
	case SDLK_3:
		this->ActiveBuySelection = this->TextBoxObjects.MettigelQty;
		this->ActiveItemData = GetItemFromVector(ItemName::Item_Mettigel, &this->BuyData);
		this->ActiveBuySubTotal = this->TextObjects.MettigelTotal;
		break;
	case SDLK_4:
		this->ActiveBuySelection = this->TextBoxObjects.CurrywurstQty;
		this->ActiveItemData = GetItemFromVector(ItemName::Item_Currywurst, &this->BuyData);
		this->ActiveBuySubTotal = this->TextObjects.CurrywurstTotal;
		break;
	case SDLK_5:
		this->ActiveBuySelection = this->TextBoxObjects.StreetSheetQty;
		this->ActiveItemData = GetItemFromVector(ItemName::Item_StreetSheet, &this->BuyData);
		this->ActiveBuySubTotal = this->TextObjects.StreetSheetTotal;
		break;
	case SDLK_6:
		this->ActiveBuySelection = this->TextBoxObjects.USADAYQty;
		this->ActiveItemData = GetItemFromVector(ItemName::Item_USADAY, &this->BuyData);
		this->ActiveBuySubTotal = this->TextObjects.USADAYTotal;
		break;
	case SDLK_7:
		this->ActiveBuySelection = this->TextBoxObjects.SignQty;
		this->ActiveItemData = GetItemFromVector(ItemName::Item_Sign, &this->BuyData);
		this->ActiveBuySubTotal = this->TextObjects.SignTotal;
		break;
	case SDLK_8:
		this->ActiveBuySelection = this->TextBoxObjects.PosterQty;
		this->ActiveItemData = GetItemFromVector(ItemName::Item_Poster, &this->BuyData);
		this->ActiveBuySubTotal = this->TextObjects.PosterTotal;
		break;
	case SDLK_9:
		this->ActiveBuySelection = this->TextBoxObjects.NewsAdQty;
		this->ActiveItemData = GetItemFromVector(ItemName::Item_NewsAd, &this->BuyData);
		this->ActiveBuySubTotal = this->TextObjects.NewsAdTotal;
		break;
	case SDLK_RETURN:
		// If enter is pressed, return to main selection
		this->EventFlags.SelectBuyItem = false;
		this->TextObjects.SelectItem->SetVisible(false);
		this->EventFlags.MainSelection = true;
		return;
		break;
	default:
		// Ignore all other input
		return;
		break;
	}
	
	// Move into quantity entry state
	this->EventFlags.SelectBuyItem = false;
	this->EventFlags.EnterItemQty = true;
	
	// Show ENTER QUANITITY text
	this->TextObjects.SelectItem->SetVisible(false);
	this->TextObjects.EnterQty->SetVisible(true);
	
	// Flush buffered text input
	SDL_PumpEvents();

	// Enable text entry for item quantity
	SDL_StartTextInput();
	this->ActiveBuySelection->SetActive(true);
}

void Market::SEvent_EndItemQtyEntry() {
	// Get quantity entered. Invalid input will convert to 0.
	this->ActiveItemData->SetQuantity(std::atoi(this->ActiveBuySelection->GetText()->c_str()));

	// Set text to sanitized number.
	this->ActiveBuySelection->SetText(std::to_string(this->ActiveItemData->GetQuantity()));

	// Set subtotal price
	int subtotal = this->ActiveItemData->GetBuyPrice() * this->ActiveItemData->GetQuantity();
	this->ActiveBuySubTotal->SetText(std::to_string(subtotal));
	
	// Update grand total
	this->UpdateTotal();
	
	// Stop text entry and return to main selection
	SDL_StopTextInput();
	this->ActiveBuySelection->SetActive(false);
	this->EventFlags.EnterItemQty = false;
	this->TextObjects.EnterQty->SetVisible(false);
	this->EventFlags.MainSelection = true;
}

void Market::SEvent_SelectForecast() {
	this->EventFlags.MainSelection = false;
	this->SEvent_HideMarketText();
	this->EventFlags.ShowForecast = true;
	this->SEvent_ShowForecast();
}

void Market::SEvent_ExitForecast() {
	this->EventFlags.ShowForecast = false;
	this->SEvent_HideForecast();
	this->EventFlags.MainSelection = true;
	this->SEvent_ShowMarketText();
}

void Market::SEvent_SelectGuide() {
	this->EventFlags.MainSelection = false;
	this->TextObjects.SelectItem->SetVisible(true);
	this->EventFlags.SelectGuideItem = true;
}

void Market::SEvent_SetGuideItem(SDL_Keycode _key) {
	// Select item to purchase
	switch (_key) {
	case SDLK_1:
		this->ActiveGuideText = this->TextObjects.BierDesc;
		break;
	case SDLK_2:
		this->ActiveGuideText = this->TextObjects.BockwurstDesc;
		break;
	case SDLK_3:
		this->ActiveGuideText = this->TextObjects.MettigelDesc;
		break;
	case SDLK_4:
		this->ActiveGuideText = this->TextObjects.CurrywurstDesc;
		break;
	case SDLK_5:
		this->ActiveGuideText = this->TextObjects.StreetSheetDesc;
		break;
	case SDLK_6:
		this->ActiveGuideText = this->TextObjects.USADAYDesc;
		break;
	case SDLK_7:
		this->ActiveGuideText = this->TextObjects.SignDesc;
		break;
	case SDLK_8:
		this->ActiveGuideText = this->TextObjects.PosterDesc;
		break;
	case SDLK_9:
		this->ActiveGuideText = this->TextObjects.NewsAdDesc;
		break;
	case SDLK_RETURN:
		// If enter is pressed, return to main selection
		this->EventFlags.SelectGuideItem = false;
		this->TextObjects.SelectItem->SetVisible(false);
		this->EventFlags.MainSelection = true;
		return;
		break;
	default:
		// Ignore all other input
		return;
		break;
	}

	// Move into guide display state
	this->EventFlags.SelectGuideItem = false;
	this->EventFlags.ShowGuide = true;

	// Show Guide text
	this->SEvent_HideMarketText();
	this->TextObjects.SelectItem->SetVisible(false);
	this->SEvent_ShowGuide();

	// Flush buffered text input
	SDL_PumpEvents();
}

void Market::SEvent_ExitGuide() {
	this->EventFlags.ShowGuide = false;
	this->SEvent_HideGuide();
	this->EventFlags.MainSelection = true;
	this->SEvent_ShowMarketText();
}

void Market::SEvent_Leave() {
	if (this->mPlayerData->GetMoney() >= this->BuyTotal) {
		// Set player inventory equal to items purchased
		for (std::vector<ItemData*>::iterator it = this->BuyData.begin(); it != this->BuyData.end(); it++)
			this->mPlayerData->GetInventoryItem((*it)->GetName())->SetQuantity((*it)->GetQuantity());

		// Set player money
		this->mPlayerData->SetMoney(this->mPlayerData->GetMoney() - this->BuyTotal);
		
		// Leave market
		this->mManager->StartScene(Scene_SetPrices);
	}
}

void Market::UpdateTotal() {
	this->BuyTotal = 0;
	
	// Get all buy subtotals
	std::vector<ItemData*>::iterator it = this->BuyData.begin();
	for (; it != this->BuyData.end(); it++)
		this->BuyTotal += (*it)->GetBuyPrice() * (*it)->GetQuantity();
	
	// Update subtotal text
	this->TextObjects.TotalAmount->SetText(std::to_string(this->BuyTotal));

	// Update remaining money text
	this->TextObjects.MoneySubTotalAmount->SetText(std::to_string(this->mPlayerData->GetMoney() - this->BuyTotal));
}