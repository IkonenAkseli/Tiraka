// Datastructures.hh
//
// Student name: Akseli Ikonen
// Student email: akseli.ikonen@tuni.fi
// Student number: K434701

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <set>

#include <unordered_set>

// Types for IDs
using TownID = std::string;
using Name = std::string;

// Return values for cases where required thing was not found
TownID const NO_TOWNID = "----------";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
Name const NO_NAME = "!!NO_NAME!!";

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};
struct TownData
{
    TownID id;
    Name name;
    Coord coord;
    int tax;
    std::unordered_set<TownData*> vassals;
    TownData* master;

};

// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate: cpp claims size is constant
    unsigned int town_count();

    // Estimate of performance: O(N)
    // Short rationale for estimate: cpp claims clear is linear
    void clear_all();

    // Estimate of performance: O(N) Theta 1
    // Short rationale for estimate: Umap insertion can be O(N) but is
    // constant on average
    bool add_town(TownID id, Name const& name, Coord coord, int tax);

    // Estimate of performance: O(N) Theta 1
    // Short rationale for estimate: Operations are linear in worst case but
    // constant on average
    Name get_town_name(TownID id);

    // Estimate of performance: O(N) Theta 1
    // Short rationale for estimate: Operations are linear in worst case but
    // constant on average
    Coord get_town_coordinates(TownID id);

    // Estimate of performance: O(N) Theta 1
    // Short rationale for estimate: Operations are linear in worst case but
    // constant on average
    int get_town_tax(TownID id);

    // Estimate of performance: O(N)
    // Short rationale for estimate: Linear amount of constant operations
    std::vector<TownID> all_towns();

    // Estimate of performance: O(N)
    // Short rationale for estimate: Linear amount of constant operations
    std::vector<TownID> find_towns(Name const& name);

    // Estimate of performance: O(N) Theta 1
    // Short rationale for estimate: Operations are done once, they are linear
    // in worst case and constant on average
    bool change_town_name(TownID id, Name const& newname);

    // Estimate of performance: NlogN
    // Short rationale for estimate: Inserting to and searching for key from
    // multimap is logN and that operation is done N times where N = town count
    std::vector<TownID> towns_alphabetically();

    // Estimate of performance: NlogN
    // Short rationale for estimate: because this only calls another method
    // which is NlogN
    std::vector<TownID> towns_distance_increasing();

    // Estimate of performance: NlogN
    // Short rationale for estimate: because this only calls another method
    // which is NlogN and does constant actions
    TownID min_distance();

    // Estimate of performance: NlogN
    // Short rationale for estimate: because this only calls another method
    // which is NlogN and does constant actions
    TownID max_distance();

    // Estimate of performance: O(N) Theta 1
    // Short rationale for estimate: Searching key from umap worst case linear
    // on average constant
    bool add_vassalship(TownID vassalid, TownID masterid);

    // Estimate of performance: O(N) where N = amount of vassals for given town
    // Short rationale for estimate: N amount of constant actions
    std::vector<TownID> get_town_vassals(TownID id);

    // Estimate of performance: O(N)
    // Short rationale for estimate: For calling add_masters
    std::vector<TownID> taxer_path(TownID id);

    // Non-compulsory phase 1 operations

    // Estimate of performance: Theta N
    // Short rationale for estimate: N insertions to unordered_set where N
    // equals amount of vassals the town had
    bool remove_town(TownID id);

    // Estimate of performance: NlogN
    // Short rationale for estimate: For calling get_distance_vector
    std::vector<TownID> towns_nearest(Coord coord);

    // Estimate of performance: O(N)
    // Short rationale for estimate: Calls linear methods and does linear
    // amount of constant operations.
    std::vector<TownID> longest_vassal_path(TownID id);

    // Estimate of performance: O(N)
    // Short rationale for estimate: For calling get_tax
    int total_net_tax(TownID id);

private:
    // Add stuff needed for your class implementation here

    //Container for all towns
    std::unordered_map<TownID, TownData> TownContainer_;

    TownData* furthest_town_;

    int depth_;
    int running_depth_;

    // Helper for checking if a town with a certain id exists
    // Takes town id as a parameter
    // Returns true if a town with the given id exists, false if not
    // Estimate for performance: O(N) Theta 1
    // Rationale: Umap.find is linear in worst case but constant in average
    bool check_Id(TownID id);

    // Calculates the eucledian distance between 2 coordinates
    // Parameters: Coordinates 1 and 2
    // Returns: Distance between the 2 coordinates rounded down
    // Estimate of performance: O(1)
    // Rationale: Does a constant calculation and returns the result.
    int get_distance(Coord coord1, Coord coord2);

    // Takes a vector of town ID:s as a parameter, adds every master and masters
    // master in to the given vector. Takes a pointer to a town from which to
    // start as a parameter and a pointer at which to stop. By default stops
    // when no masters remain.
    // This method does no checks for parameters.
    // Estimate for performance: O(N)
    // Rationale: Does push_back for each master above the node in it's branch
    // O(N) where N = Master count
    void add_masters(std::vector<TownID> &masters, TownData* town,
                     TownData* stop = nullptr);

    // This method is not used. Preserved only in case needed at some point.
    //std::vector<TownID> get_path_ids(TownData* child);

    // Gets tax for the given town, recursively calls itself to get tax from
    // every vassal.
    // Does no checks for the given parameter, returns int.
    // Estimate for permormance: O(N)
    // Rationale: Does O(1) operation for each vassal node, O(N) where
    // N = vassals
    int get_tax(TownData* town);

    // Returns a vector of TownID:s in an ascending order from a given Coord
    // Takes a Coord as a parameter, does no checks for parameter validity
    // Parameter must be valid.
    // Estimate for performance: NlogN
    // Rationale for estimate: Does nlogN operation for each town (N)
    // No loops inside loops, most complex operation done for each town
    // is logN
    std::vector<TownID> get_distance_vector(Coord coord1);

    // Finds the vassal furthest from a given town and sets it as
    // furthest_town_
    // Takes given town as a parameter, does no checks for validity of parameter
    // Estimate for performance: O(N) where N = amount of vassals
    // vassals vassals etc
    // Rationale: Every vassal is visited and linear operations are done for
    // each
    void set_furthest_vassal(TownData* town);

};

#endif // DATASTRUCTURES_HH
