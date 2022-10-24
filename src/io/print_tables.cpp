#include "print_tables.hpp"

#include "config.hpp"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

void print_tables(config const& cfg, timetable const& winner_table) {
	std::ofstream timetable_file;
	timetable_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		fs::create_directory(fs::current_path() /= "groups");
		fs::create_directory(fs::current_path() /= "lecturers");
	} catch (fs::filesystem_error const& ex) {
		throw std::runtime_error(ex.what());
	}

	for (auto const& group : cfg.groups) {
		auto const file_name = "groups/" + group.group_name + "_timetable";
		timetable_file.open(file_name);

		if (!timetable_file.is_open()) {
			throw std::runtime_error("Cannot open file to write: " + file_name);
		}
		group.draw_table(winner_table, timetable_file);
		timetable_file.close();
		timetable_file.clear();
	}

	for (auto const& lecturer : cfg.lecturers) {
		auto const file_name =
		  "lecturers/" + lecturer.lecturer_name + "_timetable";
		timetable_file.open(file_name);

		if (!timetable_file.is_open()) {
			throw std::runtime_error("Cannot open file to write: " + file_name);
		}
		lecturer.draw_table(winner_table, timetable_file);
		timetable_file.close();
		timetable_file.clear();
	}
}