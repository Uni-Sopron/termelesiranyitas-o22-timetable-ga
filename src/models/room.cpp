#include "room.hpp"

room::room(std::string room_name) : room_name(std::move(room_name)) {
}

auto operator<<(std::ostream& stream, room const& room) -> std::ostream& {
	stream << room.room_name;
	return stream;
}
