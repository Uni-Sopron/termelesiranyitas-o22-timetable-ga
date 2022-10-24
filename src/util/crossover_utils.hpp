#ifndef __CROSSOVER_UTILS_H__
#define __CROSSOVER_UTILS_H__

class timetable;

class crossover {
public:
	virtual auto operator()(timetable const& __restrict__ parent_one,
	                        timetable const& __restrict__ parent_two) const
	  -> timetable = 0;
	virtual ~crossover() = default;
};

class one_point_crossover : public crossover {
public:
	auto operator()(timetable const& __restrict__ parent_one,
	                timetable const& __restrict__ parent_two) const
	  -> timetable override;
};

class uniform_crossover : public crossover {
public:
	auto operator()(timetable const& __restrict__ parent_one,
	                timetable const& __restrict__ parent_two) const
	  -> timetable override;
};

#endif // __CROSSOVER_UTILS_H__