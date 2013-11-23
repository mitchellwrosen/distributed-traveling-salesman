#include "common/path.h"

#include <cstring>
#include <iostream>
#include <limits>

#include "common/location.h"

using std::cout;
using std::endl;
using std::numeric_limits;

Path::Path(Location* locs, int len) 
    : locs_(locs)
    , len_(len)
    , cost_(calculateCost(locs, len)) 
    , raw_(new char[serializedLen()]) {
}

Path::Path(Location* locs, int len, int cost, char* raw)
    : locs_(locs)
    , len_(len)
    , cost_(cost)
    , raw_(raw) {
}

// static
Path Path::longestPath() {
  return Path(new Location[1], 1, numeric_limits<int>::max(), nullptr);
}

// static
Path Path::deserialize(char* data) {
  int len = *((int*) data);
  int cost = *((int*) (data + sizeof(len)));

  Location* locs = new Location[len];
  memcpy(locs, data + sizeof(len) + sizeof(cost), len*sizeof(Location));

  return Path(locs, len, cost, data);
}

Path::~Path() {
  delete[] locs_;
  delete[] raw_;
}

// First len, then cost, then locs.
char* Path::serialize() {
  memcpy(raw_,                                &len_,  sizeof(len_));
  memcpy(raw_ + sizeof(len_),                 &cost_, sizeof(cost_));
  memcpy(raw_ + sizeof(len_) + sizeof(cost_), locs_,  len_*sizeof(Location));
  return raw_;
}

int Path::serializedLen() {
  return sizeof(len_) + sizeof(cost_) + len_*sizeof(Location);
}

int Path::calculateCost(Location* locs, int len) {
  int cost = 0;
  for (int i = 0; i < len-1; ++i)
    cost += locs[i].to(locs[i+1]);
  cost += locs[len-1].to(locs[0]);
  return cost;
}

void Path::print() {
  for (int i = 0; i < len_; ++i) 
    cout << locs_[i].id << " ";
  cout << "(" << cost_ << ")" << endl;
}
