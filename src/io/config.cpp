#include "config.hpp"

#include <algorithm>
#include <charconv>
#include <fstream>
#include <sstream>

#define PARSE_INTO(property, value, cfg, res) \
	res =                                     \
	  std::from_chars(value.data(), value.data() + value.size(), cfg.property)

auto config::read_config(std::string const& path) -> config {
	std::ifstream cfg_file(path);
	if (!cfg_file.is_open()) {
		throw std::runtime_error("The configuration file cannot be opened.");
	}
	using read_strategy = config::read_strategy;
	read_strategy read_mode = read_strategy::GLOBAL_SETTINGS;
	config config;

	for (std::string line; std::getline(cfg_file, line);) {
		if (line.empty() || line.starts_with(";")) {
			continue;
		}
		if (line.starts_with("[")) {
			if (line == "[global_settings]") {
				read_mode = read_strategy::GLOBAL_SETTINGS;
			} else if (line == "[lecturers]") {
				read_mode = read_strategy::LECTURER;
			} else if (line == "[subjects]") {
				read_mode = read_strategy::SUBJECT;
			} else if (line == "[groups]") {
				read_mode = read_strategy::GROUP;
			} else if (line == "[rooms]") {
				read_mode = read_strategy::ROOM;
			}
			continue;
		}

		handle_param(config, line, read_mode);
	}
	return config;
}

void config::read_subject(std::string&& line, config& config) {
	auto parts = config::split(line, ';');
	if (parts.size() < 4) {
		throw std::invalid_argument(
		  "Not enough required argument for creating a subject!");
	}
	size_t occasions = std::stoi(parts[1]);
	lecturer* lecturer = &config.lecturers[std::stoi(parts[2])];
	group* group = &config.groups[std::stoi(parts[3])];
	std::string name = parts[0];
	for (size_t s_idx = 0; s_idx < occasions; ++s_idx) {
		config.subjects.emplace_back(name, lecturer, group);
	}
}

void config::set_global_param(std::string const& param_name,
                              std::string const& param_value,
                              config& cfg) {
	std::from_chars_result res;

	if (param_name == "max_generation_count") {
		PARSE_INTO(max_generation_count, param_value, cfg, res);
	} else if (param_name == "initial_population_size") {
		PARSE_INTO(initial_population_size, param_value, cfg, res);
	} else if (param_name == "break_time") {
		PARSE_INTO(break_time, param_value, cfg, res);
	} else if (param_name == "lecture_duration") {
		PARSE_INTO(lecture_duration_in_minutes, param_value, cfg, res);
	} else if (param_name == "daily_starting_hour") {
		auto const parts = split(param_value, ':');
		if (parts.size() != 2) {
			throw std::invalid_argument(
			  "starting_hour must be in a hour:minute format!");
		}

		PARSE_INTO(starting_hour, parts[0], cfg, res);
		if (res.ec == std::errc::invalid_argument
		    || res.ec == std::errc::result_out_of_range) {
			throw std::invalid_argument("Invalid value (" + param_value
			                            + ") for the parameter \"" + param_name
			                            + "\"");
		}
		PARSE_INTO(starting_minute, parts[1], cfg, res);

	} else if (param_name == "daily_ending_hour") {
		auto const parts = split(param_value, ':');
		if (parts.size() != 2) {
			throw std::invalid_argument(
			  "ending_hour must be in a hour:minute format!");
		}

		PARSE_INTO(ending_hour, parts[0], cfg, res);
		if (res.ec == std::errc::invalid_argument
		    || res.ec == std::errc::result_out_of_range) {
			throw std::invalid_argument("Invalid value (" + param_value
			                            + ") for the parameter \"" + param_name
			                            + "\"");
		}
		PARSE_INTO(ending_minute, parts[1], cfg, res);

	} else if (param_name == "mutation_percent") {
		try {
			cfg.mutation_percent = std::stod(param_value);
		} catch (const std::invalid_argument&) {
			res.ec = std::errc::invalid_argument;
		}
	} else if (param_name == "max_age") {
		PARSE_INTO(max_age, param_value, cfg, res);
	} else if (param_name == "no_convergence_tolerance") {
		PARSE_INTO(no_convergence_tolerance, param_value, cfg, res);
	} else if (param_name == "seed") {
		PARSE_INTO(seed, param_value, cfg, res);
	} else if (param_name == "mutation_strategy") {
		if (param_value == "SWAP") {
			cfg.mutation_strategy = std::make_unique<swap_mutate>();
		} else if (param_value == "ROTATE") {
			cfg.mutation_strategy = std::make_unique<rotate_mutate>();
		}
	} else if (param_name == "rotation_strategy") {
		if (param_value == "AGING") {
			cfg.rotation_strategy =
			  std::make_unique<aging_rotation>(cfg.max_age);
		} else if (param_value == "SAME_COUNT_BEST_FIT") {
			cfg.rotation_strategy =
			  std::make_unique<same_count_best_fit_rotation>(
			    cfg.initial_population_size);
		}
	} else if (param_name == "selection_strategy") {
		if (param_value == "TOURNAMENT") {
			cfg.selection_strategy = std::make_unique<tournament_selection>();
		} else if (param_value == "ELITIST") {
			cfg.selection_strategy = std::make_unique<elitist_selection>();
		}
	} else if (param_name == "crossover_strategy") {
		if (param_value == "ONE_POINT") {
			cfg.crossover_strategy = std::make_unique<one_point_crossover>();
		} else if (param_value == "UNIFORM") {
			cfg.crossover_strategy = std::make_unique<uniform_crossover>();
		}
	} else if (param_name == "exit_strategy") {
		if (param_value == "FOUND") {
			cfg.exit_strategy = std::make_unique<found_exit>();
		} else if (param_value == "MAX_GENERATION_COUNT") {
			cfg.exit_strategy =
			  std::make_unique<max_generation_exit>(cfg.max_generation_count);
		} else if (param_value == "NO_CONVERGENCE") {
			cfg.exit_strategy =
			  std::make_unique<convergence_exit>(cfg.no_convergence_tolerance);
		}
	}

	if (res.ec == std::errc::invalid_argument
	    || res.ec == std::errc::result_out_of_range) {
		throw std::invalid_argument("Invalid value (" + param_value
		                            + ") for the parameter \"" + param_name
		                            + "\"");
	}
}

void config::handle_param(config& config, std::string& line,
                          const config::read_strategy strategy) {
	switch (strategy) {
	case config::read_strategy::GLOBAL_SETTINGS: {
		line.erase(std::remove_if(std::begin(line), std::end(line), isspace),
		           std::end(line));
		std::istringstream s_stream(std::move(line));
		std::string param_name;
		std::string param_value;
		std::getline(s_stream, param_name, '=');
		std::getline(s_stream, param_value);
		set_global_param(param_name, param_value, config);
		break;
	}
	case config::read_strategy::LECTURER: {
		config.lecturers.emplace_back(std::move(line));
		break;
	}
	case config::read_strategy::GROUP: {
		config.groups.emplace_back(std::move(line));
		break;
	}
	case config::read_strategy::ROOM: {
		config.rooms.emplace_back(std::move(line));
		break;
	}
	case config::read_strategy::SUBJECT: {
	}
		read_subject(std::move(line), config);
	}
}

auto config::split(std::string const& line, char const delimiter = ';')
  -> std::vector<std::string> {
	std::stringstream s_stream(line);
	std::vector<std::string> words;
	std::string tmp;

	while (std::getline(s_stream, tmp, delimiter)) {
		words.emplace_back(tmp);
	}

	return words;
}

#undef PARSE_INTO