# time_point a adjustable time calculator

本项目提供了一个可以便利游戏使用的时间戳计算组件，可以方便的设置当前时间，用来调试。目前提供的接口有：
1. `this_day(std::uint32_t hour = 0, std::uint32_t minute = 0, std::uint32_t second = 0) ` 计算当天特定时间点的时间戳
2. `this_week(std::uint32_t day_in_week = 1, std::uint32_t hour = 0, std::uint32_t minute = 0, std::uint32_t second = 0)` 计算本周的特定时间点的时间戳
3. `this_month(std::uint32_t day_in_month = 1, std::uint32_t hour = 0, std::uint32_t minute = 0, std::uint32_t second = 0)` 计算本月的特定时间点的时间戳
4. `this_year(std::uint32_t month = 1, std::uint32_t day_in_month = 1, std::uint32_t hour = 0, std::uint32_t minute = 0, std::uint32_t second = 0)` 计算本年的特定月的特定日期的时间戳
5. `this_year_days(std::uint32_t day_in_year = 1, std::uint32_t hour = 0, std::uint32_t minute = 0, std::uint32_t second = 0)` 计算本年的第N天的时间戳
类似的计算下一天、下个月、下一年的接口都有提供，这里再提供一组计算即将发生的特定时间点的函数：
1. `pending_day(std::uint32_t hour = 0, std::uint32_t minute = 0, std::uint32_t second = 0)` 返回下一个(hour, minute, second)出现的时间点，可能是今天，也可能是明天
2. `pending_week(std::uint32_t day_in_week = 1, std::uint32_t hour = 0, std::uint32_t minute = 0, std::uint32_t second = 0) ` 返回下一个特定weekday的(hour, minute, second)出现的时间点，可能是本周，也可能是下一周
等等五个函数


下面这个函数可以用来设置当前时间， 后续对于当前时间的引用都以此时的当前时间偏移为准：
`set_now(std::uint32_t year, std::uint32_t month, std::uint32_t day, std::uint32_t hour = 0, std::uint32_t minute = 0, std::uint32_t second = 0)`