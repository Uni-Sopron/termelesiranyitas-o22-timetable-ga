#include "lecturer.hpp"

#include "subject.hpp"
#include "timetable.hpp"
#include <vector>

lecturer::lecturer(std::string lecturer_name)
    : lecturer_name(std::move(lecturer_name)) {
}

void lecturer::draw_table(timetable const& winner_table,
                          std::ostream& stream) const {
	std::vector<subject> subjects;
	for (auto const& subject : winner_table.subjects) {
		if (subject.lecturer == this) {
			subjects.emplace_back(subject);
		}
	}
	stream << timetable(subjects);
}

auto operator<<(std::ostream& stream, lecturer const& lecturer)
  -> std::ostream& {
	stream << lecturer.lecturer_name;
	return stream;
}
