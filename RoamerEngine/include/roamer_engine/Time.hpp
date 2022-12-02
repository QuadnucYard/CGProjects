#pragma once

namespace qy::cg {
	class Time {
	private:
		inline static float _time {0};
		inline static float _deltaTime {0};
		inline static float _unscaledTime {0};
		inline static float _unscaledDeltaTime {0};
		inline static float _realtimeSinceStartup {0};
		inline static float _timeScale {1};
		inline static unsigned _frameCount {0};

	public:

		static float time() { return _time; }

		static float deltaTime() { return _deltaTime; }

		static float unscaledTime() { return _unscaledTime; }

		static float unscaledDeltaTime() { return _unscaledDeltaTime; }

		static float realtimeSinceStartup() { return _realtimeSinceStartup; }

		static float timeScale() { return _timeScale; }
		static void timeScale(float s) { _timeScale = s; }

		static unsigned frameCount() { return _frameCount; }

		static void __update(float now) {
			_realtimeSinceStartup = now;
			_unscaledDeltaTime = now - _unscaledTime;
			_unscaledTime = now;
			_deltaTime = _unscaledDeltaTime * _timeScale;
			_time += _deltaTime;
			_frameCount++;
		}
	};
}