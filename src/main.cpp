#include "io/config.hpp"
#include "io/print_tables.hpp"
#include "models/date.hpp"
#include "models/timetable.hpp"
#include "util/bootstrap.hpp"
#include "util/random_utils.hpp"

auto run_simulation(config const& cfg, ind_vec& tables, date_vec const& dates)
  -> timetable* {
	// Mutation

	for (auto& table : tables) {
		const auto rand = rnd::get_number_between<double>(0.0, 1.0);
		if (rand <= cfg.mutation_percent) {
			cfg.mutation_strategy->operator()(table, dates);
			table.evaluate_fitness();
		}
	}

	// Selection
	ind_vec selection;
	selection.reserve(tables.size());

	for (size_t tournament_idx = 0; tournament_idx < tables.size() / 2;
	     tournament_idx++) {
		auto& winner = cfg.selection_strategy->operator()(tables);
		selection.emplace_back(winner);
	}

	// Crossover
	auto const selection_size = selection.size();

	for (size_t selection_iterator = 0; selection_iterator < selection_size;
	     selection_iterator++) {
		auto const [first_idx, second_idx] =
		  rnd::get_numbers_between<size_t, 2, rnd::repetition::DISALLOWED>(
		    0, selection.size() - 1);
		auto new_table = cfg.crossover_strategy->operator()(
		  selection[first_idx], selection[second_idx]);
		selection.emplace_back(std::move(new_table));
	}

	// Rotation
	selection = cfg.rotation_strategy->operator()(selection);

	tables = std::move(selection);
	std::cout << "Current fitness indicator: "
	          << std::to_string(tables.back().fitness) << "\n";

	return &tables.back();
}

auto main() -> int {
	try {
		auto const config = config::read_config("config.ini");
		auto const [is_impossible_run, message] = is_early_exit(config);
		if (is_impossible_run) {
			std::cerr << message;
			return -2;
		}

		if (config.seed != 0) {
			rnd::seed(config.seed);
		}
		auto const dates = generate_dates(config);
		auto tables = generate_initial_tables(config, dates);

		auto const* last_table = tables.data();

		while (config.exit_strategy->operator()(last_table, tables)) {
			last_table = run_simulation(config, tables, dates);
		}

		try {
			print_tables(config, *last_table);
		} catch (std::runtime_error const& ex) {
			std::cerr
			  << "Error while printing out tables to the files. Reason:\n";
			std::cerr << ex.what();
			return -3;
		}
	} catch (std::exception const& ex) {
		std::cerr << ex.what();
		return -1;
	}
}
