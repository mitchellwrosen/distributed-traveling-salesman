#ifndef LOCATION_H_
#define LOCATION_H_

struct Location {
  int id;
  float x;
  float y;

  float to(const Location& other) const;
  bool operator<(const Location& other);
};

#endif  // LOCATION_H_
