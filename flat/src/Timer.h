#ifndef _TIMER_H
#define _TIMER_H

namespace flat2d 
{
	class Timer
	{
		public:
			Timer();

			void start();
			void stop();
			void pause();
			void unpause();

			Uint32 getTicks() const;

			bool isStarted() const;
			bool isPaused() const;

		private:

			Uint32 startTicks, pausedTicks;

			bool paused, started;
	};
}

#endif
