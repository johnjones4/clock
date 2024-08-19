#ifndef Clock_H
#define Clock_H

#include <NTPClient.h>
#include <WiFiUdp.h>
#include <sys/time.h>


class Clock
{
public:
  Clock(int utcOffset, bool useDST);
  void begin();
  void update(struct tm *timeinfo);
private:
  int isDST(time_t unix_timestamp);

  WiFiUDP *ntpUDP;
  NTPClient *client;
  int utcOffset;
  bool useDST;
};

#endif Clock_H