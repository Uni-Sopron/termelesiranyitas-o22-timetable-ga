#include "exit_utils.hpp"

convergence_exit::convergence_exit(size_t const max_convergence)
    : max_convergence(max_convergence){};

[[nodiscard]] auto
convergence_exit::additional_check(timetable const* const timetable,
                                   ind_vec const& current_tables) -> bool {
	auto const avg_fitness =
	  std::reduce(std::begin(current_tables), std::end(current_tables), 0.0)
	  / static_cast<double>(current_tables.size());
	if (last_fitness_value.has_value()
	    && avg_fitness >= static_cast<double>(last_fitness_value.value())) {
		current_convergence_counter++;
	} else {
		current_convergence_counter = 0;
	}
	last_fitness_value = avg_fitness;
	return found_exit::additional_check(timetable, current_tables)
	       && current_convergence_counter <= max_convergence;
}

max_generation_exit::max_generation_exit(size_t const max_generation_count)
    : max_generation_count(max_generation_count){};