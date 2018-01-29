#include "Data\CustomerData.h"
#include "GameObjects\GameObject.h"

Customer::Customer(ForecastEvent _event) {
	// Set game object ID
	this->mObjectId = ObjectID::Object_Customer;
	
	// Generate new customer data (based on day's event)
	this->Data = new CustomerData(_event);

	// Set random X position when stopping at shop.
	this->ShopPos = rand() % 40 + 120;

	// Set start/end positions and image based on wall side
	if (this->GetData()->GetSide() == WallSide::EastBerlin) {
		this->mPosition.SetPosition(294.0, 100.0); 
		this->EndPos = -14.0;
		this->mImageData.SetImage(Assets::Instance()->images.ECustomer);
	}
	else {
		this->mPosition.SetPosition(-14.0, 100.0);
		this->EndPos = 294.0;
		this->mImageData.SetImage(Assets::Instance()->images.WCustomer);
	}

	// Customer is not active at start
	this->Active = false;

	// Customer has not yet made a purchase
	this->Purchased = false;

	// Set initial walk state
	this->WalkIn = true;
	this->Shopping = false;
	this->WalkOut = false;
	
	// Set walk animation frame
	this->WalkFrame = 0;

	// Initialize time spent at shop at 0
	this->ShopDuration = 0;
	
	// Start walk timer
	this->WalkTimer.start();
}

void Customer::SetActive(bool _active) {
	if (_active) {
		this->Active = true;
		this->mImageData.SetVisible(true);
	}
	else {
		this->Active = false;
		this->mImageData.SetVisible(false);
	}
}

void Customer::SetPurchased(bool _purchase) {
	this->Purchased = _purchase;
}

bool Customer::HasPurchased() {
	return this->Purchased;
}

bool Customer::IsShopping()
{
	return this->Shopping;
}

void Customer::Update(Uint32 ticks) {
	if (this->Active) {
		if (this->WalkIn) {
			this->mPosition.SetX(this->mPosition.GetX() + (((this->GetData()->GetSide() == WallSide::EastBerlin) ? -0.2 : 0.2)) * (double) ticks);
			if (this->WalkTimer.getTicks() >= 100) this->UpdateAnim();
			if ((this->GetData()->GetSide() == WallSide::EastBerlin && this->mPosition.GetX() <= this->ShopPos) ||
				(this->GetData()->GetSide() == WallSide::WestBerlin && this->mPosition.GetX() >= this->ShopPos)) {
				this->WalkTimer.stop();
				this->WalkIn = false;
				this->Shopping = true;
				this->mImageData.SetImage(&Assets::Instance()->images.Customer);
			}
		}
		else if (this->Shopping) {
			this->ShopDuration += 1 * ticks;
			if (this->ShopDuration >= this->ShopTime) {
				this->WalkTimer.start();
				this->Shopping = false;
				this->WalkOut = true;
				this->mImageData.SetImage((this->GetData()->GetSide() == WallSide::EastBerlin) ? Assets::Instance()->images.ECustomer : Assets::Instance()->images.WCustomer);
			}
		}
		else if (this->WalkOut) {
			this->mPosition.SetX(this->mPosition.GetX() + (((this->GetData()->GetSide() == WallSide::EastBerlin) ? -0.2 : 0.2)) * (double) ticks);
			if (this->WalkTimer.getTicks() >= 100) this->UpdateAnim();
			if ((this->GetData()->GetSide() == WallSide::EastBerlin && this->mPosition.GetX() <= this->EndPos) ||
				(this->GetData()->GetSide() == WallSide::WestBerlin && this->mPosition.GetX() >= this->EndPos)) {
				this->WalkTimer.stop();
				this->WalkOut = false;
				this->Active = false;
			}
		}

		SetDrawPos((Uint32) this->mPosition.GetX(), (Uint32) this->mPosition.GetY());
	}
}

void Customer::UpdateAnim() {
	this->WalkFrame++;
	if (this->WalkFrame >= 4) this->WalkFrame = 0;
	(this->GetData()->GetSide() == WallSide::EastBerlin) ? this->mImageData.SetImage(&Assets::Instance()->images.ECustomer[this->WalkFrame]) : this->mImageData.SetImage(&Assets::Instance()->images.WCustomer[this->WalkFrame]);
	this->WalkTimer.stop();
	this->WalkTimer.start();
}

void Customer::PauseWalkTimer() {
	if (this->WalkTimer.isStarted() && !this->WalkTimer.isPaused())
		this->WalkTimer.pause();
}

void Customer::ResumeWalkTimer() {
	if (this->WalkTimer.isStarted() && this->WalkTimer.isPaused())
		this->WalkTimer.unpause();
}
