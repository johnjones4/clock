#ifndef TzDB_H
#define TzDB_H

typedef struct 
{
  int length;
  double *latitudes;
  double *longitudes;
  double offset;
} tz;

class TzDB 
{
public:
  bool start();
  double getOffset(double latitude, double longitude);
private:
  tz* timezones;
  int nTz;
};

#endif
