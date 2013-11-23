#ifndef COMMON_PATH_H_
#define COMMON_PATH_H_

class Location;

class Path {
 public:
  Path(Location* locs, int len);
  ~Path();

  static Path longestPath();

  static Path deserialize(char* data);

  char* serialize();
  int serializedLen();

  void print();

  // Getters
  int cost() const { return cost_; }

 private:
  Path(Location* locs, int len, int cost, char* raw);

  int calculateCost(Location* locs, int len);

  Location* locs_;
  int len_;
  int cost_;

  char* raw_;

};

#endif  // COMMON_PATH_H_
