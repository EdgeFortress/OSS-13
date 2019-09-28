#pragma once

#include <Shared/IFaces/ICopyable.h>

namespace uf {

template<class T>
class Range: public ICopyable {
	const T _begin;
	const T _end;
public:
	Range(T begin, T end): _begin(std::move(begin)), _end(std::move(end)) {}
	const T& begin() const {
		return _begin;
	}
	const T& end() const {
		return _end;
	}
};

template<class T>
auto MakeRange(T& container) {
	return Range<typename std::conditional<std::is_const<T>::value, class T::const_iterator, class T::iterator>::type>(container.begin(), container.end());
}

} // namespace uf
