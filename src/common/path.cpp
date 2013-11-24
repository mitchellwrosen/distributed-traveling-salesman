#include "common/path.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <limits>

#include "common/location.h"

using std::cout;
using std::endl;
using std::copy;
using std::numeric_limits;

Path::Path(Location* locs, int len) : len_(len) {
  locs_ = new Location[len];
  copy(locs, locs + len, locs_);
  cost_ =  calculateCost(locs_, len_);
}

Path::Path(Location* locs, int len, int cost)
    : locs_(locs)
    , len_(len)
    , cost_(cost) {
}

Path::Path(const Path& other) {
  copyPath(other);
}

const Path& Path::operator=(const Path& rhs) {
  destructPath();
  copyPath(rhs);
  return *this;
}

void Path::copyPath(const Path& other) {
  len_ = other.len();
  locs_ = new Location[len_];

  const Location* other_locs = other.locs();
  copy(other_locs, other_locs + len_, locs_);

  cost_ = calculateCost(locs_, len_);
}

Path::~Path() {
  destructPath();
}

void Path::destructPath() {
  if (locs_)
    delete[] locs_;
}

// static
Path Path::longestPath() {
  return Path(nullptr, 0, numeric_limits<int>::max());
}

// static
Path Path::deserialize(char* data) {
  int len = *((int*) data);
  int cost = *((int*) (data + sizeof(len)));

  Location* locs = new Location[len];
  memcpy(locs, data + sizeof(len) + sizeof(cost), len*sizeof(Location));

  return Path(locs, len, cost);
}

// First len, then cost, then locs.
char* Path::serialize() const {
  char* data = new char[serializedLen()];
  memcpy(data,                                &len_,  sizeof(len_));
  memcpy(data + sizeof(len_),                 &cost_, sizeof(cost_));
  memcpy(data + sizeof(len_) + sizeof(cost_), locs_,  len_*sizeof(Location));
  return data;
}

int Path::serializedLen() const {
  return sizeof(len_) + sizeof(cost_) + len_*sizeof(Location);
}

int Path::calculateCost(Location* locs, int len) {
  int cost = 0;
  for (int i = 0; i < len-1; ++i)
    cost += locs[i].to(locs[i+1]);
  cost += locs[len-1].to(locs[0]);
  return cost;
}

void Path::print() const {
  for (int i = 0; i < len_; ++i) 
    cout << locs_[i].id << " ";
  cout << "(" << cost_ << ")" << endl;
}
