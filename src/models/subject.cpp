#include "subject.hpp"

subject::subject(std::string name, class lecturer* const lecturer,
                 class group* const group)
    : lecturer(lecturer), group(group), name(std::move(name)) {
}

[[nodiscard]] auto subject::is_overlapping(subject const& other) const -> bool {
	return this->start_end_time.first <= other.start_end_time.second
	       && this->start_end_time.second >= other.start_end_time.first;
}
