#pragma once


#include <wrl/client.h>

namespace Gimme
{
	public ref class BasicTimer sealed
	{
	public:
		BasicTimer()
		{
			if (!QueryPerformanceFrequency(&_frequency))
			{
				throw ref new Platform::FailureException();
			}
		}

		void Start()
		{
			if (!QueryPerformanceCounter(&_startTime))
			{
				throw ref new Platform::FailureException();
			}
			_currentTime = _startTime;
		}

		void Stop()
		{
			if (!QueryPerformanceCounter(&_currentTime))
			{
				throw ref new Platform::FailureException();
			}
		}

		property double Duration
		{
			double get()
			{
				return  static_cast<double>(_currentTime.QuadPart-_startTime.QuadPart) / static_cast<double>(_frequency.QuadPart);
			}
		}

	private:
		LARGE_INTEGER _frequency;
		LARGE_INTEGER _currentTime;
		LARGE_INTEGER _startTime;
	};

}

