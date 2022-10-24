#ifndef __TIMETABLE_H__
#define __TIMETABLE_H__

#include <iosfwd>
#include <vector>

#include "subject.hpp"

class timetable {
public:
	timetable(std::vector<subject> subjects);

	inline auto operator<=>(const timetable& rhs) const {
		return fitness <=> rhs.fitness;
	};
	inline operator std::size_t() const {
		return fitness;
	}
	inline void evaluate_fitness() {
		fitness = evaluate_subject_overlaps();
	}

	friend auto operator<<(std::ostream& stream, timetable const& timetable)
	  -> std::ostream&;

	std::vector<subject> subjects;
	size_t fitness{0};

private:
	auto evaluate_subject_overlaps() -> size_t;
};

using ind_vec = std::vector<timetable>;

#endif // __TIMETABLE_H__