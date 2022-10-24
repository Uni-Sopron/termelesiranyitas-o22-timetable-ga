#ifndef __MUTATE_UTILS_H__
#define __MUTATE_UTILS_H__

#include "../models/date.hpp"

class timetable;

class mutation {
public:
	virtual auto operator()(timetable& candidate, date_vec const& dates) const
	  -> timetable& = 0;
	virtual ~mutation() = default;
};

class swap_mutate : public mutation {
	auto operator()(timetable& candidate, date_vec const& dates) const
	  -> timetable& override;
};

class rotate_mutate : public mutation {
	auto operator()(timetable& candidate, date_vec const& /*dates*/) const
	  -> timetable& override;
};

#endif // __MUTATE_UTILS_H__