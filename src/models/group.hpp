#ifndef __GROUP_H__
#define __GROUP_H__

#include <iosfwd>
#include <string>

class timetable;

class group {
public:
	group(std::string group_name);

	friend auto operator<<(std::ostream& stream, group const& group)
	  -> std::ostream&;

	std::string group_name;

	void draw_table(timetable const& winner_table, std::ostream& stream) const;
};

#endif // __GROUP_H__