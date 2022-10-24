#ifndef __EXIT_UTILS_H__
#define __EXIT_UTILS_H__

#include "../models/timetable.hpp"
#include <numeric>
#include <optional>

class exit_satisfied {
public:
	inline auto operator()(timetable const* const table,
	                       ind_vec const& current_tables) -> bool {
		return additional_check(table, current_tables);
	}
	virtual ~exit_satisfied() = default;

	[[nodiscard]] virtual auto additional_check(timetable const* table,
	                                            ind_vec const& current_tables)
	  -> bool = 0;
};

class found_exit : public exit_satisfied {
protected:
	[[nodiscard]] inline auto
	additional_check(timetable const* const timetable,
	                 ind_vec const& /*current_tables*/) -> bool override {
		return timetable->fitness != 0;
	};
};

class convergence_exit : public found_exit {
public:
	convergence_exit(size_t max_convergence);

protected:
	[[nodiscard]] auto additional_check(timetable const* timetable,
	                                    ind_vec const& current_tables)
	  -> bool override;

private:
	size_t max_convergence;
	size_t current_convergence_counter{};
	std::optional<size_t> last_fitness_value{std::nullopt};
};

class max_generation_exit : public found_exit {
public:
	max_generation_exit(size_t max_generation_count);

protected:
	[[nodiscard]] inline auto additional_check(timetable const* table,
	                                           ind_vec const& current_tables)
	  -> bool override {
		current_generation_count++;
		return found_exit::additional_check(table, current_tables)
		       && current_generation_count <= max_generation_count;
	}

private:
	size_t max_generation_count;
	size_t current_generation_count{};
};

#endif // __EXIT_UTILS_H__