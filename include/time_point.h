#include <chrono>
#include <cstdint>
#include <ctime>
#include <utility>
#include <tuple>
namespace spiritsaway::datetime
{
	using time_point = std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds>;
	
	class time_point_mgr
	{
	private:
		const std::chrono::hours timezone_offset; // for hours
		std::chrono::seconds manual_offset; // for seconds

	public:
		time_point_mgr();
		time_point now() const;


		time_point this_day(std::uint32_t hour = 0, std::uint32_t minute = 0, std::uint32_t second = 0) const;
		time_point this_week(std::uint32_t day_in_week = 1, std::uint32_t hour = 0, std::uint32_t minute = 0, std::uint32_t second = 0) const;
		time_point this_month(std::uint32_t day_in_month = 1, std::uint32_t hour = 0, std::uint32_t minute = 0, std::uint32_t second = 0) const;
		time_point this_year(std::uint32_t month = 1, std::uint32_t day_in_month = 1, std::uint32_t hour = 0, std::uint32_t minute = 0, std::uint32_t second = 0) const;
		time_point this_year_days(std::uint32_t day_in_year = 1, std::uint32_t hour = 0, std::uint32_t minute = 0, std::uint32_t second = 0) const;


		time_point next_day(std::uint32_t hour = 0, std::uint32_t minute = 0, std::uint32_t second = 0) const;
		time_point next_week(std::uint32_t day_in_week = 1, std::uint32_t hour = 0, std::uint32_t minute = 0, std::uint32_t second = 0) const;
		time_point next_month(std::uint32_t day_in_month = 1, std::uint32_t hour = 0, std::uint32_t minute = 0, std::uint32_t second = 0) const;
		time_point next_year(std::uint32_t month = 1, std::uint32_t day_in_month = 1, std::uint32_t hour = 0, std::uint32_t minute = 0, std::uint32_t second = 0) const;
		time_point next_year_days(std::uint32_t day_in_year = 1, std::uint32_t hour = 0, std::uint32_t minute = 0, std::uint32_t second = 0) const;



		// pending time_point may be this week or next week
		time_point pending_day(std::uint32_t hour = 0, std::uint32_t minute = 0, std::uint32_t second = 0) const;
		time_point pending_week(std::uint32_t day_in_week = 1, std::uint32_t hour = 0, std::uint32_t minute = 0, std::uint32_t second = 0) const;
		time_point pending_month(std::uint32_t day_in_month = 1, std::uint32_t hour = 0, std::uint32_t minute = 0, std::uint32_t second = 0) const;
		time_point pending_year(std::uint32_t month, std::uint32_t day_in_month = 1, std::uint32_t hour = 0, std::uint32_t minute = 0, std::uint32_t second = 0) const;
		time_point pending_year_days(std::uint32_t day_in_year = 1, std::uint32_t hour = 0, std::uint32_t minute = 0, std::uint32_t second = 0) const;

		bool is_this_day(time_point ts) const;
		bool is_this_week(time_point ts) const;
		bool is_this_month(time_point ts) const;
		bool is_this_year(time_point ts) const;

		time_point day_start(time_point ts) const;
		time_point week_start(time_point ts) const;
		time_point month_start(time_point ts) const;
		time_point year_start(time_point ts) const;


		bool is_same_day(time_point ts1, time_point ts2) const;
		bool is_same_week(time_point ts1, time_point ts2) const;
		bool is_same_month(time_point ts1, time_point ts2) const;
		bool is_same_year(time_point ts1, time_point ts2) const;
		

		

		int day_diff(time_point ts1, time_point ts2) const;
		int week_diff(time_point ts1, time_point ts2) const;
		int month_diff(time_point ts1, time_point ts2) const;
		int year_diff(time_point ts1, time_point ts2) const;

		// split a timepoint to year month day miliseconds_in_day
		std::tuple<int, int, int, std::chrono::milliseconds> split_to_tuple(time_point ts) const;

		std::uint32_t day_in_week(time_point ts) const;
		

		void set_now(std::uint32_t year, std::uint32_t month, std::uint32_t day, std::uint32_t hour = 0, std::uint32_t minute = 0, std::uint32_t second = 0);

		time_point get_ts(std::uint32_t year, std::uint32_t month, std::uint32_t day, std::uint32_t hour = 0, std::uint32_t minute = 0, std::uint32_t second = 0) const;
		time_point floor_to_second(time_point ts) const;

		static int current_tz_offset();

	};
}