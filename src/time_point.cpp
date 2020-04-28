#include <time_point.h>

using namespace spiritsaway::datetime;

int time_point_mgr::current_tz_offset()
{
	auto now_ts = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
	auto cur_time_t = std::chrono::system_clock::to_time_t(now_ts);
	struct tm  timeinfo_gm;
	struct tm timeinfo_local;
	gmtime_s(&timeinfo_gm, &cur_time_t);
	localtime_s(&timeinfo_local, &cur_time_t);
	auto tz_offset = timeinfo_local.tm_mday * 24 + timeinfo_local.tm_hour - timeinfo_gm.tm_mday * 24 - timeinfo_gm.tm_hour;
	return tz_offset;
}
time_point_mgr::time_point_mgr()
	: timezone_offset(time_point_mgr::current_tz_offset())
{

}


time_point time_point_mgr::now() const
{
	auto result = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
	result += std::chrono::seconds(manual_offset);
	return result;
}

time_point time_point_mgr::floor_to_second(time_point ts) const
{
	return ts - std::chrono::milliseconds(ts.time_since_epoch().count() % 1000);
}

// functions for this 
time_point time_point_mgr::this_day(std::uint32_t hour, std::uint32_t minute, std::uint32_t second) const
{
	auto day_diff = std::chrono::hours(hour) + std::chrono::minutes(minute) + std::chrono::seconds(second);
	struct tm timeinfo_local;
	auto now_ts = floor_to_second(now());
	auto cur_time_t = std::chrono::system_clock::to_time_t(now_ts);
	localtime_s(&timeinfo_local, &cur_time_t);
	auto this_day_begin = now_ts - std::chrono::seconds(timeinfo_local.tm_hour * 3600 + timeinfo_local.tm_min * 60 + timeinfo_local.tm_sec);

	return this_day_begin + day_diff;
}

time_point time_point_mgr::this_week(std::uint32_t tm_wday, std::uint32_t hour, std::uint32_t minute, std::uint32_t second) const
{
	// week begin is monday
	auto day_diff = std::chrono::hours(hour) + std::chrono::minutes(minute) + std::chrono::seconds(second);
	struct tm timeinfo_local;
	auto now_ts = floor_to_second(now());
	auto cur_time_t = std::chrono::system_clock::to_time_t(now_ts);
	localtime_s(&timeinfo_local, &cur_time_t);
	time_point this_week_begin;
	if (timeinfo_local.tm_wday != 0)
	{
		this_week_begin = now_ts - std::chrono::seconds((timeinfo_local.tm_wday - 1) * 86400 + timeinfo_local.tm_hour * 3600 + timeinfo_local.tm_min * 60 + timeinfo_local.tm_sec);
	}
	else
	{
		this_week_begin = now_ts - std::chrono::seconds( 6 * 86400 + timeinfo_local.tm_hour * 3600 + timeinfo_local.tm_min * 60 + timeinfo_local.tm_sec);
	}


	return this_week_begin + std::chrono::hours((tm_wday - 1) * 24) + day_diff;

}
time_point time_point_mgr::this_month(std::uint32_t tm_mday, std::uint32_t hour, std::uint32_t minute, std::uint32_t second) const
{
	struct tm timeinfo_local;
	auto now_ts = floor_to_second(now());
	auto cur_time_t = std::chrono::system_clock::to_time_t(now_ts);
	localtime_s(&timeinfo_local, &cur_time_t);
	timeinfo_local.tm_mday = tm_mday;
	timeinfo_local.tm_hour = hour;
	timeinfo_local.tm_min = minute;
	timeinfo_local.tm_sec = second;
	auto dest_time_t = std::mktime(&timeinfo_local);
	return std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::from_time_t(dest_time_t));
}

time_point time_point_mgr::this_year(std::uint32_t month, std::uint32_t tm_mday, std::uint32_t hour, std::uint32_t minute, std::uint32_t second) const
{
	struct tm timeinfo_local;
	auto now_ts = now();
	auto cur_time_t = std::chrono::system_clock::to_time_t(now_ts);
	localtime_s(&timeinfo_local, &cur_time_t);
	timeinfo_local.tm_mon = month - 1;
	timeinfo_local.tm_mday = tm_mday;
	timeinfo_local.tm_hour = hour;
	timeinfo_local.tm_min = minute;
	timeinfo_local.tm_sec = second;
	auto dest_time_t = std::mktime(&timeinfo_local);
	return std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::from_time_t(dest_time_t));
	
}

time_point time_point_mgr::this_year_days(std::uint32_t day_in_year, std::uint32_t hour, std::uint32_t minute, std::uint32_t second) const
{
	struct tm timeinfo_local;
	auto now_ts = now();
	auto cur_time_t = std::chrono::system_clock::to_time_t(now_ts);
	localtime_s(&timeinfo_local, &cur_time_t);
	timeinfo_local.tm_mon = 0;
	timeinfo_local.tm_mday = 1;
	timeinfo_local.tm_hour = 0;
	timeinfo_local.tm_min = 0;
	timeinfo_local.tm_sec = 0;
	auto this_year_begin = std::mktime(&timeinfo_local);

	return  std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::from_time_t(this_year_begin)) + std::chrono::hours(day_in_year * 24 + hour) + std::chrono::minutes(minute) + std::chrono::seconds(second);
}


// functions for next
time_point time_point_mgr::next_day(std::uint32_t hour, std::uint32_t minute, std::uint32_t second) const
{
	return this_day(hour, minute, second) + std::chrono::hours(24) ;
}

time_point time_point_mgr::next_week(std::uint32_t day_in_week, std::uint32_t hour, std::uint32_t minute, std::uint32_t second) const
{

	return this_week(day_in_week, hour, minute, second) + std::chrono::hours(7 * 24 ) ;
}
time_point time_point_mgr::next_month(std::uint32_t tm_mday, std::uint32_t hour, std::uint32_t minute, std::uint32_t second) const
{
	struct tm timeinfo_local;
	auto now_ts = now();
	auto cur_time_t = std::chrono::system_clock::to_time_t(now_ts);
	localtime_s(&timeinfo_local, &cur_time_t);
	timeinfo_local.tm_mon ++;
	timeinfo_local.tm_mday = tm_mday;
	timeinfo_local.tm_hour = hour;
	timeinfo_local.tm_min = minute;
	timeinfo_local.tm_sec = second;
	if (timeinfo_local.tm_mon > 11)
	{
		timeinfo_local.tm_mon = 0;
		timeinfo_local.tm_year++;
	}
	return  std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::from_time_t(std::mktime(&timeinfo_local)));
}

time_point time_point_mgr::next_year(std::uint32_t month, std::uint32_t tm_mday, std::uint32_t hour, std::uint32_t minute, std::uint32_t second) const
{
	struct tm timeinfo_local;
	auto now_ts = now();
	auto cur_time_t = std::chrono::system_clock::to_time_t(now_ts);
	localtime_s(&timeinfo_local, &cur_time_t);
	timeinfo_local.tm_year++;

	timeinfo_local.tm_mon = month - 1;
	timeinfo_local.tm_mday = tm_mday;
	timeinfo_local.tm_hour = hour;
	timeinfo_local.tm_min = minute;
	timeinfo_local.tm_sec = second;

	return  std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::from_time_t(std::mktime(&timeinfo_local)));
}

time_point time_point_mgr::next_year_days(std::uint32_t day_in_year, std::uint32_t hour, std::uint32_t minute, std::uint32_t second) const
{
	struct tm timeinfo_local;
	auto now_ts = now();
	auto cur_time_t = std::chrono::system_clock::to_time_t(now_ts);
	localtime_s(&timeinfo_local, &cur_time_t);
	timeinfo_local.tm_year++;

	timeinfo_local.tm_mon = 0;
	timeinfo_local.tm_mday = 1;
	timeinfo_local.tm_hour = hour;
	timeinfo_local.tm_min = minute;
	timeinfo_local.tm_sec = second;

	return  std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::from_time_t(std::mktime(&timeinfo_local))) + std::chrono::seconds(day_in_year * 86400);
}


// functions for pending

time_point time_point_mgr::pending_day(std::uint32_t hour, std::uint32_t minute, std::uint32_t second) const
{
	auto now_ts = now();
	auto this_day_ts = this_day(hour, minute, second);
	if (this_day_ts > now_ts)
	{
		return this_day_ts;
	}
	else
	{
		return next_day(hour, minute, second);
	}
}

time_point time_point_mgr::pending_week(std::uint32_t day_in_week, std::uint32_t hour, std::uint32_t minute, std::uint32_t second) const
{
	auto now_ts = now();
	auto this_week_ts = this_week(day_in_week, hour, minute, second);
	if (this_week_ts > now_ts)
	{
		return this_week_ts;
	}
	else
	{
		return next_week(day_in_week, hour, minute, second);
	}
}

time_point time_point_mgr::pending_month(std::uint32_t tm_mday, std::uint32_t hour, std::uint32_t minute, std::uint32_t second) const
{
	auto now_ts = now();
	auto this_month_ts = this_month(tm_mday, hour, minute, second);
	if (this_month_ts > now_ts)
	{
		return this_month_ts;
	}
	else
	{
		return next_month(tm_mday, hour, minute, second);
	}
}

time_point time_point_mgr::pending_year(std::uint32_t month, std::uint32_t tm_mday, std::uint32_t hour, std::uint32_t minute, std::uint32_t second) const
{
	auto now_ts = now();
	auto this_year_ts = this_year(month, tm_mday, hour, minute, second);
	if (this_year_ts > now_ts)
	{
		return this_year_ts;
	}
	else
	{
		return next_year(month, tm_mday, hour, minute, second);
	}
}

time_point time_point_mgr::pending_year_days( std::uint32_t day_in_year, std::uint32_t hour, std::uint32_t minute, std::uint32_t second) const
{
	auto now_ts = now();
	auto this_year_ts = this_year_days(day_in_year, hour, minute, second);
	if (this_year_ts > now_ts)
	{
		return this_year_ts;
	}
	else
	{
		return next_year_days(day_in_year, hour, minute, second);
	}
}


bool time_point_mgr::is_this_day(time_point ts) const
{
	auto this_day_begin = this_day();
	return ts >= this_day_begin && ts < (this_day_begin + std::chrono::hours(24));
}

bool time_point_mgr::is_this_week(time_point ts) const
{
	auto this_week_begin = this_week();
	return ts >= this_week_begin && ts < (this_week_begin + std::chrono::hours(7 * 24));
}

bool time_point_mgr::is_this_month(time_point ts) const
{

	return ts >= this_month() && ts < next_month();
}

bool time_point_mgr::is_this_year(time_point ts) const
{
	return ts >= this_year() && ts < next_year();

}


time_point time_point_mgr::day_start(time_point ts) const
{

	tm timeinfo_local;
	auto cur_time_t = std::chrono::system_clock::to_time_t(ts);
	localtime_s(&timeinfo_local, &cur_time_t);
	auto this_day_begin = floor_to_second(ts) - std::chrono::seconds(timeinfo_local.tm_hour * 3600 + timeinfo_local.tm_min * 60 + timeinfo_local.tm_sec);
	return this_day_begin;
}

time_point time_point_mgr::week_start(time_point ts) const
{
	tm timeinfo_local;
	auto cur_time_t = std::chrono::system_clock::to_time_t(ts);
	localtime_s(&timeinfo_local, &cur_time_t);
	auto this_week_begin = floor_to_second(ts) - std::chrono::seconds(timeinfo_local.tm_wday * 86400 + timeinfo_local.tm_hour * 3600 + timeinfo_local.tm_min * 60 + timeinfo_local.tm_sec);
	return this_week_begin;
}

time_point time_point_mgr::month_start(time_point ts) const
{
	tm timeinfo_local;
	auto cur_time_t = std::chrono::system_clock::to_time_t(ts);
	localtime_s(&timeinfo_local, &cur_time_t);
	auto this_month_begin = floor_to_second(ts) - std::chrono::seconds((timeinfo_local.tm_mday - 1) * 86400 + timeinfo_local.tm_hour * 3600 + timeinfo_local.tm_min * 60 + timeinfo_local.tm_sec);
	return this_month_begin;
}

time_point time_point_mgr::year_start(time_point ts) const
{
	tm timeinfo_local;
	auto cur_time_t = std::chrono::system_clock::to_time_t(ts);
	localtime_s(&timeinfo_local, &cur_time_t);
	auto this_year_begin = floor_to_second(ts) - std::chrono::seconds((timeinfo_local.tm_year) * 86400 + timeinfo_local.tm_hour * 3600 + timeinfo_local.tm_min * 60 + timeinfo_local.tm_sec);
	return this_year_begin;
}

bool time_point_mgr::is_same_day(time_point ts1, time_point ts2) const
{
	return day_start(ts1) == day_start(ts2);
}

bool time_point_mgr::is_same_week(time_point ts1, time_point ts2) const
{
	return week_start(ts1) == week_start(ts2);
}
bool time_point_mgr::is_same_month(time_point ts1, time_point ts2) const
{
	return month_start(ts1) == month_start(ts2);

}
bool time_point_mgr::is_same_year(time_point ts1, time_point ts2) const
{
	return year_start(ts1) == year_start(ts2);

}


// cacl diffs
int time_point_mgr::day_diff(time_point ts1, time_point ts2) const
{
	return (day_start(ts2) - day_start(ts1)).count() / (86400 * 1000);
}

int time_point_mgr::week_diff(time_point ts1, time_point ts2) const
{
	if (ts1 > ts2)
	{
		return -1 * week_diff(ts2, ts1);
	}
	tm timeinfo_local_1, timeinfo_local_2;
	auto cur_time_t_1 = std::chrono::system_clock::to_time_t(ts1);
	localtime_s(&timeinfo_local_1, &cur_time_t_1);
	auto cur_time_t_2 = std::chrono::system_clock::to_time_t(ts2);
	localtime_s(&timeinfo_local_2, &cur_time_t_2);
	return (week_start(ts2) - week_start(ts1)).count() / (86400 * 1000 * 7) + (timeinfo_local_1.tm_wday > timeinfo_local_2.tm_wday ? 1: 0);

}
int time_point_mgr::month_diff(time_point ts1, time_point ts2) const
{
	tm timeinfo_local_1, timeinfo_local_2;
	auto cur_time_t_1 = std::chrono::system_clock::to_time_t(ts1);
	localtime_s(&timeinfo_local_1, &cur_time_t_1);
	auto cur_time_t_2 = std::chrono::system_clock::to_time_t(ts2);
	localtime_s(&timeinfo_local_2, &cur_time_t_2);
	return (timeinfo_local_2.tm_year - timeinfo_local_1.tm_year) * 12 + timeinfo_local_2.tm_mon - timeinfo_local_1.tm_mon;

}
int time_point_mgr::year_diff(time_point ts1, time_point ts2) const
{
	tm timeinfo_local_1, timeinfo_local_2;
	auto cur_time_t_1 = std::chrono::system_clock::to_time_t(ts1);
	localtime_s(&timeinfo_local_1, &cur_time_t_1);
	auto cur_time_t_2 = std::chrono::system_clock::to_time_t(ts2);
	localtime_s(&timeinfo_local_2, &cur_time_t_2);
	return timeinfo_local_2.tm_year - timeinfo_local_1.tm_year;
}

std::tuple<int, int, int, std::chrono::milliseconds> time_point_mgr::split_to_tuple(time_point ts) const
{
	//ts -= timezone_offset;
	tm timeinfo_local;
	auto cur_time_t = std::chrono::system_clock::to_time_t(ts);
	localtime_s(&timeinfo_local, &cur_time_t);
	auto this_day_begin = floor_to_second(ts) - std::chrono::seconds(timeinfo_local.tm_hour * 3600 + timeinfo_local.tm_min * 60 + timeinfo_local.tm_sec);
	return std::make_tuple(timeinfo_local.tm_year, timeinfo_local.tm_mon, timeinfo_local.tm_mday, ts - this_day_begin);

}

std::uint32_t time_point_mgr::day_in_week(time_point ts) const
{

	auto cur_week_start = week_start(ts);
	auto cur_day_start = day_start(ts);
	return (cur_day_start - cur_week_start).count() / (86400 * 1000);
}
void time_point_mgr::set_now(std::uint32_t year, std::uint32_t month, std::uint32_t day, std::uint32_t hour, std::uint32_t minute, std::uint32_t second)
{
	auto dest_ts = get_ts(year, month, day, hour, minute, second);
	manual_offset = std::chrono::duration_cast<std::chrono::seconds>(dest_ts - std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()));
}
time_point time_point_mgr::get_ts(std::uint32_t year, std::uint32_t month, std::uint32_t day, std::uint32_t hour, std::uint32_t minute, std::uint32_t second) const
{
	tm timeinfo_local{};
	timeinfo_local.tm_year = year- 1900;
	timeinfo_local.tm_mon = month - 1;
	timeinfo_local.tm_mday = day;

	auto dest_time_t = std::mktime(&timeinfo_local);
	auto dest_day_begin = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::from_time_t(dest_time_t));
	return dest_day_begin + std::chrono::milliseconds(( hour * 3600 + minute * 60 + second) * 1000) ;
	
}
