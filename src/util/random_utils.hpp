#ifndef __RANDOM_UTILS_H__
#define __RANDOM_UTILS_H__

#include <algorithm>
#include <array>
#include <random>
#include <type_traits>
template <class T>
concept number_type = std::is_integral_v<T> || std::is_floating_point_v<T>;

class rnd {
public:
	enum class repetition { ALLOWED, DISALLOWED };
	rnd() = delete;
	template <number_type T>
	static auto get_number_between(const T low, const T high) -> T {
		if constexpr (std::is_integral_v<T>) {
			return std::uniform_int_distribution<T>(low, high)(generator);
		} else {
			return std::uniform_real_distribution<T>(low, high)(generator);
		}
	}

	template <number_type T, std::size_t Count, repetition Rep>
	static auto get_numbers_between(const T low, const T high) {
		std::array<T, Count> numbers;
		std::ranges::generate(numbers, [&low, &high, &numbers]() {
			if constexpr (Rep == repetition::DISALLOWED) {
				T num = rnd::get_number_between<T>(low, high);
				while (std::ranges::count(numbers, num)) {
					num = rnd::get_number_between<T>(low, high);
				}
				return num;
			} else {
				return rnd::get_number_between<T>(low, high);
			}
		});
		return numbers;
	}

	template <typename T>
	static void shuffle_vector(std::vector<T>& vec) {
		std::ranges::shuffle(vec, generator);
	}

	template <typename T>
	static auto sample_vector(std::vector<T> const& vec, size_t const count) {
		std::vector<T> out;
		out.reserve(count);
		std::ranges::sample(vec, std::back_inserter(out), count, generator);
		return out;
	}

	static void seed(size_t const seed_num) {
		rnd::generator.seed(seed_num);
	}

private:
	static std::random_device random_device;
	static std::mt19937 generator;
};

inline std::random_device rnd::random_device;
inline std::mt19937 rnd::generator(rnd::random_device());

#endif // __RANDOM_UTILS_H__