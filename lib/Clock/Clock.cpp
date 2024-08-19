#include <Clock.h>

#include <time.h>
#include <sys/time.h>

Clock::Clock(int utcOffset, bool useDST)
{
  this->ntpUDP = new WiFiUDP();
  this->client = new NTPClient(*this->ntpUDP);
  this->utcOffset = utcOffset;
  this->useDST = useDST;
}

void Clock::begin()
{
  this->client->begin();
}

void Clock::update(struct tm *timeinfo)
{
  this->client->update();
  long unix_timestamp = this->client->getEpochTime();

  int dst_offset = 0;
  if (this->useDST) {
    dst_offset = this->isDST(unix_timestamp) ? 1 : 0;
  }

  unix_timestamp += (this->utcOffset + dst_offset) * 3600;

  gmtime_r(&unix_timestamp, timeinfo);
}

/*
  int	tm_sec;
  int	tm_min;
  int	tm_hour;
  int	tm_mday;
  int	tm_mon;
  int	tm_year;
  int	tm_wday;
  int	tm_yday;
  int	tm_isdst;
*/

int Clock::isDST(time_t unix_timestamp) {
    struct tm timeinfo;
    gmtime_r(&unix_timestamp, &timeinfo);
    
    // Calculate the local time with the standard timezone offset
    time_t local_time = unix_timestamp + this->utcOffset * 3600;
    gmtime_r(&local_time, &timeinfo);

    // Define the DST start and end rules (example: for North America)
    // DST starts on the second Sunday in March and ends on the first Sunday in November

    // DST start: second Sunday in March
    struct tm dst_start = { .tm_hour = 2, .tm_mday = 8, .tm_mon = 2, .tm_year = timeinfo.tm_year, };
    for (int i = 0; i < 7; i++) {
        if (gmtime_r(&(local_time = mktime(&dst_start) + i * 86400), &timeinfo)->tm_wday == 0) {
            break;  // Found the second Sunday in March
        }
    }

    // DST end: first Sunday in November
    struct tm dst_end = { .tm_hour = 2, .tm_mday = 1, .tm_mon = 10, .tm_year = timeinfo.tm_year, };
    for (int i = 0; i < 7; i++) {
        if (gmtime_r(&(local_time = mktime(&dst_end) + i * 86400), &timeinfo)->tm_wday == 0) {
            break;  // Found the first Sunday in November
        }
    }

    // Check if the current time is within the DST period
    time_t dst_start_time = mktime(&dst_start);
    time_t dst_end_time = mktime(&dst_end);

    return (unix_timestamp >= dst_start_time && unix_timestamp < dst_end_time);
}