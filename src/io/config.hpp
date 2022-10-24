#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <memory>
#include <string>
#include <vector>

#include "../models/group.hpp"
#include "../models/lecturer.hpp"
#include "../models/room.hpp"
#include "../models/subject.hpp"

#include "../util/crossover_utils.hpp"
#include "../util/exit_utils.hpp"
#include "../util/mutate_utils.hpp"
#include "../util/rotation_utils.hpp"
#include "../util/selection_utils.hpp"

class lecturer;
class subject;
class group;
class room;
class config {
public:
	double mutation_percent{0.05};
	std::unique_ptr<crossover> crossover_strategy;
	std::unique_ptr<selection> selection_strategy;
	std::unique_ptr<mutation> mutation_strategy;
	std::unique_ptr<rotation> rotation_strategy;
	std::unique_ptr<exit_satisfied> exit_strategy;
	size_t max_generation_count{0};
	size_t no_convergence_tolerance{0};
	size_t initial_population_size{10};
	size_t max_age{3};
	int starting_hour{8};
	int starting_minute{0};
	int ending_hour{18};
	int ending_minute{0};
	int break_time{15};
	size_t seed{0};
	int lecture_duration_in_minutes{45};
	std::vector<lecturer> lecturers;
	std::vector<subject> subjects;
	std::vector<group> groups;
	std::vector<room> rooms;

	static auto read_config(std::string const& path) -> config;

private:
	config() = default;
	enum class read_strategy {
		GLOBAL_SETTINGS,
		LECTURER,
		SUBJECT,
		GROUP,
		ROOM
	};

	static void read_subject(std::string&& line, config& config);

	static void set_global_param(std::string const& param_name,
	                             std::string const& param_value,
	                             config& cfg);

	static void handle_param(config& config, std::string& line,
	                         config::read_strategy strategy);

	static auto split(std::string const& line, char delimiter)
	  -> std::vector<std::string>;
};

#endif // __CONFIG_H__