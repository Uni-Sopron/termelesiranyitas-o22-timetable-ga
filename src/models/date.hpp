#ifndef __DATE_H__
#define __DATE_H__
#include <iostream>
#include <tuple>
#include <vector>

class date {
public:
	date(int day, int hour, int minute);

	date() = default;

	void add_minutes(int minutes_to_add);

	friend auto operator<<(std::ostream &stream, date const &date)
	  -> std::ostream &;

	inline auto operator<=>(const date &rhs) const {
		return std::tie(day, hour, minute)
		       <=> std::tie(rhs.day, rhs.hour, rhs.minute);
	};

	int day;
	int hour;
	int minute;
};

inline auto operator<<(std::ostream &stream, date const &date)
  -> std::ostream & {
	stream << (date.hour < 10 ? "0" : "") << date.hour << ":"
	       << (date.minute < 10 ? "0" : "") << date.minute;

	return stream;
}

using date_vec = std::vector<std::pair<date, date>>;

#endif // __DATE_H__