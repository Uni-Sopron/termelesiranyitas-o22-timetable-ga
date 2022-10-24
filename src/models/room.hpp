#ifndef __ROOM_H__
#define __ROOM_H__

#include <string>

class room {
public:
	room(std::string room_name);

	friend auto operator<<(std::ostream& stream, room const& room)
	  -> std::ostream&;

	std::string room_name;
};

#endif // __ROOM_H__