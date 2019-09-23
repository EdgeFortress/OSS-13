#pragma once

class ICopyable {
public:
	ICopyable() = default;
	virtual ~ICopyable() = default;

	ICopyable(const ICopyable &) = default;
	ICopyable &operator=(const ICopyable &) = default;

	ICopyable(ICopyable &&) = default;
	ICopyable &operator=(ICopyable &&) = default;
};
