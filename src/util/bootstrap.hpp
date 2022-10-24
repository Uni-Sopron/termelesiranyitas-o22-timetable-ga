#ifndef __BOOTSTRAP_H__
#define __BOOTSTRAP_H__

#include "../io/config.hpp"
#include "../models/date.hpp"
#include "../models/timetable.hpp"

auto generate_dates(config const& config) -> date_vec;

auto generate_initial_tables(config const& config, date_vec const& dates)
  -> ind_vec;

auto is_early_exit(config const& config) -> std::tuple<bool, std::string>;

#endif // __BOOTSTRAP_H__