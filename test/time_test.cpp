#include <time_point.h>
#include <iostream>
#include <ctime>
#include <vector>
#include <string>

using namespace spiritsaway::datetime;
using namespace std;
std::string format_timepoint(time_point cur_ts)
{
	auto milliseconds_since_epoch = cur_ts.time_since_epoch().count();
	auto epoch_begin = std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds>();
	auto cur_timepoint = epoch_begin + std::chrono::milliseconds(milliseconds_since_epoch);
	auto cur_time_t = std::chrono::system_clock::to_time_t(cur_timepoint);

	tm timeinfo;
	char buffer[80];

	localtime_s(&timeinfo, &cur_time_t);

	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S ", &timeinfo);
	return std::string(buffer) + std::to_string(milliseconds_since_epoch % 1000) + "ms";
}

void test_case(const time_point_mgr& tm_mgr)
{
	vector<pair<string, time_point>> test_cases = {
		{"now", tm_mgr.now()},
	{"this day start", tm_mgr.this_day()},
	{"this week start", tm_mgr.this_week()},
	{"this_month_start", tm_mgr.this_month()},
	{"this year start", tm_mgr.this_year()},
	{"next_day", tm_mgr.next_day()},
	{"next_week", tm_mgr.next_week()},
	{"next_month", tm_mgr.next_month()},
	{"next_year", tm_mgr.next_year()},
	
	};
	for (auto one_case : test_cases)
	{
		cout << one_case.first << " is " << format_timepoint(one_case.second) << endl;
	}
	for (int i = 0; i < 24; i++)
	{
		cout << "pending hour " << i << " is " << format_timepoint(tm_mgr.pending_day(i)) << endl;
	}
	for (int i = 1; i < 8; i++)
	{
		cout << "pending weekday "<<i<<" is "<< format_timepoint(tm_mgr.pending_week(i)) << endl;
	}
	for (int i = 1; i < 13; i++)
	{
		cout << "pending month " << i << " is " << format_timepoint(tm_mgr.pending_month(i)) << endl;

	}
}
int main()
{
	time_point_mgr cur_tm_mgr_1;
	//test_case(cur_tm_mgr_1);
	cur_tm_mgr_1.set_now(2020, 5, 1, 4, 30, 15);
	test_case(cur_tm_mgr_1);
}