#ifndef COMMON_PATH_H_
#define COMMON_PATH_H_

class Location;

/**
 * A Path is an immutable list of locations, implicitly connected in the order 
 * they appear (with the last location connected to the first).
 *
 * A Path makes a copy of the Location* it's created with. It doesn't take
 * ownership.
 */
class Path {
 public:
  Path(Location* locs, int len);
  ~Path();

  Path(const Path& other);
  const Path& operator=(const Path& rhs);

  /**
   * Create a bogus path, with INT_MAX cost, but nonsensical other member
   * variables. This is only used as a convenience to iteratively find the
   * best path.
   */
  static Path longestPath();

  static Path deserialize(char* data);

  /**
   * Serialize this Path into a byte array. Caller owns the data.
   */
  char* serialize() const;
  int serializedLen() const;

  void print() const;

  // Getters
  const Location* locs() const { return locs_; }
  int len() const { return len_; }
  int cost() const { return cost_; }

 private:
  Path(Location* locs, int len, int cost);
  void destructPath();
  void copyPath(const Path& other);

  int calculateCost(Location* locs, int len);

  Location* locs_;
  int len_;
  int cost_;

};

#endif  // COMMON_PATH_H_
