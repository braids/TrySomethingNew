#include "Data\CustomerData.h"
#include "GameObjects\GameObject.h"

CustomerObject::CustomerObject(WallSide _side) {
	this->mObjectId = ObjectID::Object_Customer;
	this->ShopPos = rand() % 40 + 120;
	this->Side = _side;

	if (this->Side == WallSide::EastBerlin) {
		this->StartPos = 294;
		this->EndPos = -14;
		this->mImageData.SetImage(Assets::Instance()->images.ECustomer);
	}
	else {
		this->StartPos = -14;
		this->EndPos = 294;
		this->mImageData.SetImage(Assets::Instance()->images.WCustomer);
	}

	this->mPosition.SetPosition(this->StartPos, 100);

	this->Active = false;
	this->Purchased = false;

	this->WalkIn = true;
	this->Shopping = false;
	this->WalkOut = false;

	this->WalkFrame = 0;
	this->ShopDuration = 0;

	this->WalkTimer.start();
}

void CustomerObject::SetActive(bool _active) {
	if (_active) {
		this->Active = true;
		this->mImageData.SetVisible(true);
	}
	else {
		this->Active = false;
		this->mImageData.SetVisible(false);
	}
}

void CustomerObject::SetPurchased(bool _purchase) {
	this->Purchased = _purchase;
}

bool CustomerObject::HasPurchased() {
	return this->Purchased;
}

bool CustomerObject::IsShopping()
{
	return this->Shopping;
}

void CustomerObject::Update(Uint32 ticks) {
	if (this->Active) {
		if (this->WalkIn) {
			this->mPosition.SetX(this->mPosition.GetX() + (((this->Side == WallSide::EastBerlin) ? -0.2 : 0.2)) * ticks);
			if (this->WalkTimer.getTicks() >= 100) this->UpdateAnim();
			if ((this->Side == WallSide::EastBerlin && this->mPosition.GetX() <= this->ShopPos) ||
				(this->Side == WallSide::WestBerlin && this->mPosition.GetX() >= this->ShopPos)) {
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
				this->mImageData.SetImage((this->Side == WallSide::EastBerlin) ? Assets::Instance()->images.ECustomer : Assets::Instance()->images.WCustomer);
			}
		}
		else if (this->WalkOut) {
			this->mPosition.SetX(this->mPosition.GetX() + (((this->Side == WallSide::EastBerlin) ? -0.2 : 0.2)) * ticks);
			if (this->WalkTimer.getTicks() >= 100) this->UpdateAnim();
			if ((this->Side == WallSide::EastBerlin && this->mPosition.GetX() <= this->EndPos) ||
				(this->Side == WallSide::WestBerlin && this->mPosition.GetX() >= this->EndPos)) {
				this->WalkTimer.stop();
				this->WalkOut = false;
				this->Active = false;
			}
		}

		SetDrawPos((Uint32) this->mPosition.GetX(), (Uint32) this->mPosition.GetY());
	}
}

void CustomerObject::UpdateAnim() {
	this->WalkFrame++;
	if (this->WalkFrame >= 4) this->WalkFrame = 0;
	(this->Side == WallSide::EastBerlin) ? this->mImageData.SetImage(&Assets::Instance()->images.ECustomer[this->WalkFrame]) : this->mImageData.SetImage(&Assets::Instance()->images.WCustomer[this->WalkFrame]);
	this->WalkTimer.stop();
	this->WalkTimer.start();
}

void CustomerObject::PauseWalkTimer() {
	if (this->WalkTimer.isStarted() && !this->WalkTimer.isPaused())
		this->WalkTimer.pause();
}

void CustomerObject::ResumeWalkTimer() {
	if (this->WalkTimer.isStarted() && this->WalkTimer.isPaused())
		this->WalkTimer.unpause();
}
