#include "bootstrap.hpp"
#include "random_utils.hpp"

auto generate_dates(config const& config) -> date_vec {
	std::vector<std::pair<date, date>> lecture_date_pairs;

	for (int day = 0; day < 5; ++day) {
		date daily_min(day, config.starting_hour, config.starting_minute);
		date daily_max(day, config.ending_hour, config.ending_minute);
		date start_time = daily_min;

		bool daily_first = true;
		while (start_time < daily_max) {
			if (!daily_first) {
				start_time.add_minutes(config.break_time);
			}
			auto end_time = start_time;
			end_time.add_minutes(config.lecture_duration_in_minutes);
			lecture_date_pairs.emplace_back(
			  std::make_pair<>(start_time, end_time));
			start_time.add_minutes(config.lecture_duration_in_minutes);
			daily_first = false;
		}
	}

	return lecture_date_pairs;
}

auto generate_initial_tables(config const& config, date_vec const& dates)
  -> ind_vec {
	ind_vec tables(config.initial_population_size, config.subjects);
	std::unordered_map<std::string, std::vector<subject*>> group_subjects(
	  config.groups.size());

	for (auto& table : tables) {
		for (auto& subject : table.subjects) {
			if (group_subjects.find(subject.group->group_name)
			    == std::end(group_subjects)) {
				group_subjects.insert({subject.group->group_name, {}});
			}

			group_subjects[subject.group->group_name].emplace_back(&subject);
		}

		for (auto& group_pair : group_subjects) {
			auto const sample =
			  rnd::sample_vector(dates, group_pair.second.size());
			size_t sample_idx = 0;
			for (auto* subject : group_pair.second) {
				subject->start_end_time = sample.at(sample_idx++);
				auto const room_idx =
				  rnd::get_number_between<size_t>(0, config.rooms.size() - 1);
				subject->room = &config.rooms[room_idx];
			}
		}
		group_subjects.clear();
	}

	for (auto& table : tables) {
		table.evaluate_fitness();
	}

	return tables;
}

auto is_early_exit(config const& config) -> std::tuple<bool, std::string> {
	auto const dates = generate_dates(config);

	std::unordered_map<std::string, std::vector<subject const*>> group_subjects(
	  config.groups.size());

	std::unordered_map<std::string, std::vector<subject const*>>
	  lecturer_subjects(config.lecturers.size());

	for (auto const& subject : config.subjects) {
		if (group_subjects.find(subject.group->group_name)
		    == std::end(group_subjects)) {
			group_subjects.insert({subject.group->group_name, {}});
		}
		if (lecturer_subjects.find(subject.lecturer->lecturer_name)
		    == std::end(lecturer_subjects)) {
			lecturer_subjects.insert({subject.lecturer->lecturer_name, {}});
		}

		lecturer_subjects[subject.lecturer->lecturer_name].emplace_back(
		  &subject);
		group_subjects[subject.group->group_name].emplace_back(&subject);
	}

	for (auto& group_subject_pair : group_subjects) {
		if (group_subject_pair.second.size() > dates.size()) {
			return std::make_tuple(
			  true,
			  "There are more subjects for group (" + group_subject_pair.first
			    + " ) (" + std::to_string(group_subject_pair.second.size())
			    + ")" + "than the available possible date count ("
			    + std::to_string(dates.size()) + ".");
		}
	}

	for (auto& lecturer_subject_pair : lecturer_subjects) {
		if (lecturer_subject_pair.second.size() > dates.size()) {
			return std::make_tuple(
			  true,
			  "There are more subjects for lecturer ("
			    + lecturer_subject_pair.first + " ) ("
			    + std::to_string(lecturer_subject_pair.second.size()) + ")"
			    + "than the available possible date count ("
			    + std::to_string(dates.size()) + ".");
		}
	}

	return std::make_tuple<>(false, "All well...");
}