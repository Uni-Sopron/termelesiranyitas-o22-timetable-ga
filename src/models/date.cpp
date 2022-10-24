#include "date.hpp"

date::date(int const day, int const hour, int const minute)
    : day(day), hour(hour), minute(minute) {
	if (day > 4 || day < 0) {
		throw std::invalid_argument(
		  "Invalid day argument during date generation! day number must be "
		  "between 0 and 5, but "
		  + std::to_string(day) + " was given.");
	}
	if (hour < 0 || hour > 23) {
		throw std::invalid_argument(
		  "Invalid hour argument during date generation! Hour number must "
		  "be between 0 and 24, but "
		  + std::to_string(hour) + " was given.");
	}
	if (minute < 0 || minute > 59) {
		throw std::invalid_argument(
		  "Invalid minute argument during date generation! Minute number "
		  "must "
		  "be between 0 and 60, but "
		  + std::to_string(minute) + " was given.");
	}
}

void date::add_minutes(const int minutes_to_add) {
	minute += minutes_to_add;
	while (minute > 59) {
		hour++;
		minute -= 60;
		if (hour > 23) {
			hour = 0;
			day++;
			if (day > 4) {
				day = 0;
			}
		}
	}
}
