#ifndef _DELTATIME_MONITOR_H
#define _DELTATIME_MONITOR_H

namespace flat2d
{
	class DeltatimeMonitor
	{
		private:
			float deltaTime = 1.0;
			int currentTime = 0;
			int oldTime = 0;

		public:
			void updateDeltaTime();
			float getDeltaTime() const;
	};
}

#endif
