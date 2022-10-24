#include "rotation_utils.hpp"

#include <algorithm>

same_count_best_fit_rotation::same_count_best_fit_rotation(
  size_t const rotated_array_size)
    : rotated_array_size(rotated_array_size){};
auto same_count_best_fit_rotation::operator()(ind_vec& tables) -> ind_vec& {
	std::ranges::sort(tables);
	tables.erase(std::begin(tables) + static_cast<long>(rotated_array_size),
	             std::end(tables));
	return tables;
}

aging_rotation::aging_rotation(size_t const max_age) : max_age(max_age){};

auto aging_rotation::operator()(ind_vec& tables) -> ind_vec& {
	for (size_t table_idx = 0; table_idx < tables.size(); ++table_idx) {
		if (table_age.find(&tables[table_idx]) == std::end(table_age)) {
			table_age.insert({&tables[table_idx], 0});
		}
		table_age[&tables[table_idx]]++;

		if (table_age[&tables[table_idx]] > max_age) {
			tables.erase(std::begin(tables) + static_cast<long>(table_idx));
			table_age.erase(&tables[table_idx]);
		}
	}
	return tables;
}