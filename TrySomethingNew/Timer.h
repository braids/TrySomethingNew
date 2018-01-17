#pragma once
#ifndef _TIMER_H_
#define _TIMER_H_

#include <SDL2/SDL.h>

class Timer {
private:
	Uint32 mStartTicks;
	Uint32 mPausedTicks;

	bool mPaused;
	bool mStarted;

public:
	Timer();

	void start();
	void stop();
	void pause();
	void unpause();

	Uint32 getTicks();

	bool isStarted();
	bool isPaused();
};


class EventTimer : public Timer {
protected:
	EventTimer** NextTimer;
	Uint32		EventFireTime;
	bool		Loop;
	bool*		flag;
public:
	EventTimer() {
		this->SetNextTimer(nullptr);
		this->SetEventFireTime(0);
		this->SetLoop(false);
		this->SetFlagTarget(nullptr);
	}
	EventTimer(bool* _flag, Uint32 _eventTime) {
		this->SetNextTimer(nullptr);
		this->SetEventFireTime(_eventTime);
		this->SetLoop(false);
		this->SetFlagTarget(_flag);
	}
	EventTimer(bool* _flag, Uint32 _eventTime, bool _loop) {
		this->SetNextTimer(nullptr);
		this->SetEventFireTime(_eventTime);
		this->SetLoop(_loop);
		this->SetFlagTarget(_flag);
	}
	EventTimer(bool* _flag, Uint32 _eventTime, EventTimer** _next) {
		this->SetNextTimer(_next);
		this->SetEventFireTime(_eventTime);
		this->SetLoop(false);
		this->SetFlagTarget(_flag);
	}
	EventTimer(bool* _flag, Uint32 _eventTime, EventTimer** _next, bool _loop) {
		this->SetNextTimer(_next);
		this->SetEventFireTime(_eventTime);
		this->SetLoop(_loop);
		this->SetFlagTarget(_flag);
	}
	
	void SetNextTimer(EventTimer** _next) { this->NextTimer = _next; }
	EventTimer** GetNextTimer() { return this->NextTimer; }
	
	void SetEventFireTime(Uint32 _eventTime) { this->EventFireTime = _eventTime; }
	Uint32 GetEventFireTime() { return this->EventFireTime; }
	
	bool IsLooping() { return this->Loop; }
	void SetLoop(bool _loop) { this->Loop = _loop; }

	void SetFlagTarget(bool* _flag) { this->flag = _flag; }
	void SetFlagState(bool _flag) {
		if (this->flag != nullptr)
			*(this->flag) = _flag;
	}

	void StartEventTimer() {
		this->SetFlagState(true);
		this->start();
	}

	void Update() {
		if (this->getTicks() >= this->GetEventFireTime()) {
			// Stop timer
			this->stop();
			this->SetFlagState(false);
			// If timer loops, restart timer
			if (this->IsLooping()) {
				this->SetFlagState(true);
				this->start();
			}
			// If another timer starts after this, start the next timer
			if (this->GetNextTimer() != nullptr)
				(*(this->GetNextTimer()))->StartEventTimer();
		}
	}
};
#endif
