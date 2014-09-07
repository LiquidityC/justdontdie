#ifndef _TIMER_H
#define _TIMER_H

class Timer
{
	public:
		Timer();

		void start();
		void stop();
		void pause();
		void unpause();

		Uint32 getTicks();

		bool isStarted();
		bool isPaused();

	private:

		Uint32 startTicks, pausedTicks;

		bool paused, started;
};

#endif
