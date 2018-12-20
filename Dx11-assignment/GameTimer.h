#pragma once
#include<Windows.h>

class GameTimer
{
public:
	static GameTimer* instance;
	static GameTimer* getInstance()
	{
		if (!instance)
		{
			instance = new GameTimer();
		}
		return instance;
	}
	GameTimer(GameTimer const&) = delete;
	void operator=(GameTimer const&) = delete;
	
	
	GameTimer();
	~GameTimer();

	float GameTime()const;// in seconds
	float DeltaTime() const; // in seconds#

	void Reset(); // call before message loop
	void Start(); // call when unpaused
	void Stop(); // call when paused;
	void Tick(); // call every frame

private:
	double mSecondsPerCount;
	double mDeltaTime;
	__int64 mBaseTime;
	__int64 mPausedTime;
	__int64 mStopTime;
	__int64 mPrevTime;
	__int64 mCurrTime;

	bool mStopped;
};

