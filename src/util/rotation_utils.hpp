#ifndef __ROTATION_UTILS_H__
#define __ROTATION_UTILS_H__

#include "../models/timetable.hpp"
#include <unordered_map>

class rotation {
public:
	virtual auto operator()(ind_vec& tables) -> ind_vec& = 0;
	virtual ~rotation() = default;
};

class same_count_best_fit_rotation : public rotation {
public:
	same_count_best_fit_rotation(size_t rotated_array_size);
	auto operator()(ind_vec& tables) -> ind_vec& override;

private:
	size_t rotated_array_size;
};

class aging_rotation : public rotation {
public:
	aging_rotation(size_t max_age);
	auto operator()(ind_vec& tables) -> ind_vec& override;

private:
	size_t max_age;
	std::unordered_map<timetable*, size_t> table_age;
};

#endif // __ROTATION_UTILS_H__