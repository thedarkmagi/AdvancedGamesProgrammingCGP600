#include "GameTimer.h"

GameTimer * GameTimer::instance = nullptr;

GameTimer::GameTimer():
	mSecondsPerCount(0.0), mDeltaTime(-1.0),mBaseTime(0),mPausedTime(0), mCurrTime(0),mStopped(false)
{
	
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	mSecondsPerCount = 1.0 / (double)countsPerSec;
}


GameTimer::~GameTimer()
{
}

float GameTimer::GameTime() const
{
	if (mStopped)
	{
		return(float)(((mStopTime - mPausedTime) - mBaseTime)*mSecondsPerCount);
	}
	else
	{
		return (float)(((mCurrTime - mPausedTime) - mBaseTime)*mSecondsPerCount);
	}
}

float GameTimer::DeltaTime() const
{
	return (float)mDeltaTime;
}

void GameTimer::Reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	mBaseTime = currTime;
	mPrevTime = currTime;
	mStopTime = 0;
	mStopped = false;
}

void GameTimer::Start()
{
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	// accumulate the time elapsed betwen stop and start pairs

	if (mStopped)
	{
		//accumilate paused time
		mPausedTime += (startTime - mStopTime);
		mPrevTime = startTime;
		// no longer stopped 
		mStopTime = 0;
		mStopped = false;
	}
}

void GameTimer::Stop()
{
	if (!mStopped)
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		//otherwise st and set bool indicating is stopped
		mStopTime = currTime;
		mStopped = true;
	}
}

void GameTimer::Tick()
{
	if (mStopped)
	{
		mDeltaTime = 0.0;
		return;
	}

	//get the time this frame
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mCurrTime = currTime;

	// time difference between this frame and the previous
	mDeltaTime = (mCurrTime - mPrevTime) *mSecondsPerCount;

	// prepare for next frame
	mPrevTime = mCurrTime;

	// force nonnegative. the DXSDK's CDXUTTimer mentions that if the processor goes into power save model or we get shuffled to another processor then mDeltaTime can be negative
	if (mDeltaTime < 0.0)
	{
		mDeltaTime = 0.0;
	}
}
