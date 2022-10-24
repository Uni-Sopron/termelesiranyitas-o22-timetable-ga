#include "timetable.hpp"

#include <algorithm>
#include <array>

#include "date.hpp"
#include "subject.hpp"

timetable::timetable(std::vector<subject> subjects)
    : subjects(std::move(subjects)) {
}

auto timetable::evaluate_subject_overlaps() -> size_t {
	size_t fitness{0};
	for (auto const& subject : subjects) {
		for (auto const& other_subject : subjects) {
			if (subject.name == other_subject.name) {
				continue;
			}
			if ((subject.lecturer == other_subject.lecturer
			     || subject.group == other_subject.group)
			    && subject.is_overlapping(other_subject)) {
				fitness++;
			}
			if (subject.is_overlapping(other_subject)
			    && subject.room == other_subject.room) {
				fitness++;
			}
		}
	}
	return fitness;
}

auto operator<<(std::ostream& stream, timetable const& timetable)
  -> std::ostream& {
	std::vector<subject> subjects(timetable.subjects);
	std::ranges::sort(
	  subjects, [](subject const& subject_one, subject const& subject_other) {
		  return subject_one.start_end_time < subject_other.start_end_time;
	  });

	static constexpr std::array<std::string_view, 5> days = {
	  "Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};

	size_t subject_idx = 0;
	for (size_t day_idx = 0; day_idx < days.size(); ++day_idx) {
		stream << days[day_idx] << "\n";
		stream << "*** ***"
		       << "\n\n";

		while (subject_idx < subjects.size()
		       && subjects[subject_idx].start_end_time.first.day
		            == static_cast<int>(day_idx)) {
			stream << subjects[subject_idx++] << "\n";
		}
		stream << "*** ***"
		       << "\n\n";
	}

	return stream;
}
