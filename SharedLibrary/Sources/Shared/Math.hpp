#pragma once

namespace uf {

	template <typename T> int sgn(T val) {
		return (T(0) < val) - (val < T(0));
	}

	template <typename T> T abs(T val) {
		return (T(0) < val) ? val : -val;
	}

}