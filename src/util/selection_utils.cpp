#include "selection_utils.hpp"

#include "random_utils.hpp"
#include <algorithm>
#include <functional>
#include <numeric>
#include <ranges>

auto tournament_selection::operator()(ind_vec& tables) -> timetable& {
	auto const [first_idx, second_idx] =
	  rnd::get_numbers_between<size_t, 2, rnd::repetition::DISALLOWED>(
	    0, tables.size() - 1);
	auto& candidate = tables[first_idx];
	auto& candidate2 = tables[second_idx];

	return candidate > candidate2 ? candidate2 : candidate;
}

auto elitist_selection::operator()(ind_vec& tables) -> timetable& {
	if (preprocessed_tables != &tables) {
		std::partial_sort(std::begin(tables),
		                  std::begin(tables)
		                    + static_cast<long>(tables.size() / 3),
		                  std::end(tables));

		preprocessed_tables = &tables;
	}

	auto const winner_idx = rnd::get_number_between<size_t>(
	  0, static_cast<long>(tables.size() / 3) - 1);

	return tables[winner_idx];
}
