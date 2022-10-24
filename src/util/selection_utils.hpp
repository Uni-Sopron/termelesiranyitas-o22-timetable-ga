#ifndef __SELECTION_UTILS_H__
#define __SELECTION_UTILS_H__

#include "../models/timetable.hpp"

class selection {
public:
	virtual auto operator()(ind_vec& tables) -> timetable& = 0;
	virtual ~selection() = default;
};

class tournament_selection : public selection {
	auto operator()(ind_vec& tables) -> timetable& override;
};

class elitist_selection : public selection {
	auto operator()(ind_vec& tables) -> timetable& override;

private:
	ind_vec const* preprocessed_tables = nullptr;
};

#endif // __SELECTION_UTILS_H__