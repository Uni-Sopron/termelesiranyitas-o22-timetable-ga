#ifndef __SUBJECT_H__
#define __SUBJECT_H__

#include "date.hpp"

#include "lecturer.hpp"
#include "room.hpp"
class group;

class subject {
public:
	subject(std::string name, lecturer* lecturer, group* group);

	[[nodiscard]] auto is_overlapping(subject const& other) const -> bool;

	friend auto operator<<(std::ostream& stream, date const& date)
	  -> std::ostream&;

	std::pair<date, date> start_end_time;
	lecturer const* lecturer;
	group const* group;
	room const* room;
	std::string name;
};

inline auto operator<<(std::ostream& stream, subject const& subject)
  -> std::ostream& {
	stream << subject.name << " - " << subject.start_end_time.first << ":"
	       << subject.start_end_time.second << " - " << *subject.lecturer
	       << " - " << *subject.room;

	return stream;
}

#endif // __SUBJECT_H__