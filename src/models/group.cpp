#include "group.hpp"

#include "subject.hpp"
#include "timetable.hpp"

group::group(std::string group_name) : group_name(std::move(group_name)) {
}
void group::draw_table(timetable const& winner_table,
                       std::ostream& stream) const {
	std::vector<subject> subjects;
	for (auto const& subject : winner_table.subjects) {
		if (subject.group == this) {
			subjects.emplace_back(subject);
		}
	}
	stream << timetable(subjects);
}

auto operator<<(std::ostream& stream, group const& group) -> std::ostream& {
	stream << group.group_name;

	return stream;
}
