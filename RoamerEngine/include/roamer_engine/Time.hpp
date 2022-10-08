#pragma once

namespace qy::cg {
	class Time {
	private:
		inline static double _time {0};
		inline static double _deltaTime {0};
		inline static double _unscaledTime {0};
		inline static double _unscaledDeltaTime {0};
		inline static double _realtimeSinceStartup {0};
		inline static double _timeScale {1};
		inline static unsigned _frameCount {0};

	public:

		static double time() { return _time; }

		static double deltaTime() { return _deltaTime; }

		static double unscaledTime() { return _unscaledTime; }

		static double unscaledDeltaTime() { return _unscaledDeltaTime; }

		static double realtimeSinceStartup() { return _realtimeSinceStartup; }

		static double timeScale() { return _timeScale; }
		static void timeScale(double s) { _timeScale = s; }

		static unsigned frameCount() { return _frameCount; }

		static void __update(double now) {
			_realtimeSinceStartup = now;
			_unscaledDeltaTime = now - _unscaledTime;
			_unscaledTime = now;
			_deltaTime = _unscaledDeltaTime * _timeScale;
			_time += _deltaTime;
			_frameCount++;
		}
	};
}