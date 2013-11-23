#include "common/location.h"

float Location::to(const Location& other) const {
  int xdist = x - other.x;
  int ydist = y - other.y;
  return xdist*xdist + ydist*ydist;
}

bool Location::operator<(const Location& other) {
  return id < other.id;
}
