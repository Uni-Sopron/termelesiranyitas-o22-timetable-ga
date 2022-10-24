#ifndef __LECTURER_H__
#define __LECTURER_H__

#include <string>

class timetable;

class lecturer {
public:
	lecturer(std::string name);
	std::string lecturer_name;

	friend auto operator<<(std::ostream& stream, lecturer const& lecturer)
	  -> std::ostream&;

	void draw_table(timetable const& winner_table, std::ostream& stream) const;
};

#endif // __LECTURER_H__