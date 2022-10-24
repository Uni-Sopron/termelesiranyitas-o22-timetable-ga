#include "mutate_utils.hpp"

#include "../models/timetable.hpp"
#include "random_utils.hpp"

auto swap_mutate::operator()(timetable& candidate, date_vec const& dates) const
  -> timetable& {
	const auto subject_idx =
	  rnd::get_number_between<std::size_t>(0, candidate.subjects.size() - 1);
	const auto date_idx =
	  rnd::get_number_between<std::size_t>(0, dates.size() - 1);
	candidate.subjects[subject_idx].start_end_time = dates[date_idx];
	return candidate;
}

auto rotate_mutate::operator()(timetable& candidate,
                               date_vec const& /*dates*/) const -> timetable& {
	auto const [first_index, second_index] =
	  rnd::get_numbers_between<std::size_t, 2, rnd::repetition::DISALLOWED>(
	    0, candidate.subjects.size() - 1);
	auto const [lower, higher] = std::minmax(first_index, second_index);
	auto partition_upper_limit = higher;
	for (size_t subject_idx = lower; subject_idx < partition_upper_limit;
	     ++subject_idx) {
		std::swap(candidate.subjects[subject_idx].start_end_time,
		          candidate.subjects[partition_upper_limit--].start_end_time);
	}

	return candidate;
}