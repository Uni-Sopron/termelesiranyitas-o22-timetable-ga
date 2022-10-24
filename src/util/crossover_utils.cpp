#include "crossover_utils.hpp"

#include "../models/date.hpp"
#include "../models/subject.hpp"
#include "../models/timetable.hpp"
#include "random_utils.hpp"

auto one_point_crossover::operator()(
  timetable const& __restrict__ parent_one,
  timetable const& __restrict__ parent_two) const -> timetable {
	timetable new_table(parent_one.subjects);
	auto const [first_idx, second_idx] =
	  rnd::get_numbers_between<size_t, 2, rnd::repetition::DISALLOWED>(
	    0, new_table.subjects.size() - 1);
	auto const [min_idx, max_idx] = std::minmax(first_idx, second_idx);
	auto const dice = rnd::get_number_between<int>(0, 1) != 0;

	// First half
	for (size_t subjects_idx = 0; subjects_idx < min_idx; subjects_idx++) {
		new_table.subjects[subjects_idx].start_end_time =
		  dice ? parent_one.subjects[subjects_idx].start_end_time :
		         parent_two.subjects[subjects_idx].start_end_time;
		new_table.subjects[subjects_idx].room =
		  dice ? parent_one.subjects[subjects_idx].room :
		         parent_two.subjects[subjects_idx].room;
	}
	// Second half
	for (size_t subjects_idx = min_idx; subjects_idx < max_idx;
	     subjects_idx++) {
		new_table.subjects[subjects_idx].start_end_time =
		  !dice ? parent_one.subjects[subjects_idx].start_end_time :
		          parent_two.subjects[subjects_idx].start_end_time;
		new_table.subjects[subjects_idx].room =
		  !dice ? parent_one.subjects[subjects_idx].room :
		          parent_two.subjects[subjects_idx].room;
	}
	new_table.evaluate_fitness();
	return new_table;
}

auto uniform_crossover::operator()(
  timetable const& __restrict__ parent_one,
  timetable const& __restrict__ parent_two) const -> timetable {
	timetable new_table(parent_one.subjects);

	for (size_t sub_idx = 0; sub_idx < parent_one.subjects.size(); sub_idx++) {
		auto const dice = rnd::get_number_between<double>(0, 1);
		if (dice < 0.5) {
			new_table.subjects[sub_idx].start_end_time =
			  parent_one.subjects[sub_idx].start_end_time;
			new_table.subjects[sub_idx].room =
			  parent_one.subjects[sub_idx].room;
		} else {
			new_table.subjects[sub_idx].start_end_time =
			  parent_two.subjects[sub_idx].start_end_time;
			new_table.subjects[sub_idx].room =
			  parent_two.subjects[sub_idx].room;
		}
	}
	new_table.evaluate_fitness();
	return new_table;
}
