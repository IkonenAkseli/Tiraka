// Datastructures.hh
//
// Student name:Akseli Ikonen
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
#include <queue>

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



struct Node
{
    TownID id;

    int status = 0;
    Node* from = nullptr;
    int cost = 999999;

    std::unordered_map<Node*, int> neighbours;

};


// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance:
    // Short rationale for estimate:
    unsigned int town_count();

    // Estimate of performance:
    // Short rationale for estimate:
    void clear_all();

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_town(TownID id, Name const& name, Coord coord, int tax);

    // Estimate of performance:
    // Short rationale for estimate:
    Name get_town_name(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    Coord get_town_coordinates(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    int get_town_tax(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> all_towns();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> find_towns(Name const& name);

    // Estimate of performance:
    // Short rationale for estimate:
    bool change_town_name(TownID id, Name const& newname);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> towns_alphabetically();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> towns_distance_increasing();

    // Estimate of performance:
    // Short rationale for estimate:
    TownID min_distance();

    // Estimate of performance:
    // Short rationale for estimate:
    TownID max_distance();

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_vassalship(TownID vassalid, TownID masterid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> get_town_vassals(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> taxer_path(TownID id);

    // Non-compulsory phase 1 operations

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_town(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> towns_nearest(Coord coord);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> longest_vassal_path(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    int total_net_tax(TownID id);


    // Phase 2 operations

    // Each operation that does a check for valid parameters can be constant
    // if parameters are invalid. unordered_map search by key, insertion
    // and deletion are treated as constant.

    // Estimate of performance: Linear
    // Short rationale for estimate: Cppreference claims umap.clear() is linear
    void clear_roads();

    // Estimate of performance: N*V where N is the amount of towns in road
    // network and V is the amount of roads between towns.
    // Short rationale for estimate: There are N*V constant operations
    // in the dual loop. Operations outside that are either linear or constant
    std::vector<std::pair<TownID, TownID>> all_roads();

    // Estimate of performance: N where N is the amount of roads town1 aklready
    // has.
    // Short rationale for estimate:There is a constant operation for each road
    // town1 has.
    bool add_road(TownID town1, TownID town2);

    // Estimate of performance: N where N is the amount of roads the given town
    // has.
    // Short rationale for estimate: Constant operation for each road the given
    // town has.
    std::vector<TownID> get_roads_from(TownID id);

    // Estimate of performance: N + V where N is the amount of towns in the
    // graph and V is the amount of roads.
    // Short rationale for estimate: Calls a function that has N + V complexity
    // There is of course a chance for a constant operation if start and end
    // are the same.
    std::vector<TownID> any_route(TownID fromid, TownID toid);

    // Non-compulsory phase 2 operations

    // Estimate of performance: N where N is the amount of roads town1 has
    // Short rationale for estimate: Checks every road that town one has.
    // Can be constant if there is only one road.
    bool remove_road(TownID town1, TownID town2);

    // Estimate of performance: N + V where N is the amount of towns with roads
    // and V is the amount of roads.
    // Short rationale for estimate: Calls a BFS algorithm.
    std::vector<TownID> least_towns_route(TownID fromid, TownID toid);

    // Estimate of performance: N + V where N is the amount of towns with roads
    // and V is the amount of roads.
    // Short rationale for estimate: Calls a dfs algorithm
    std::vector<TownID> road_cycle_route(TownID startid);

    // Estimate of performance: N log V where N is the amount of towns with
    // roads and V is the amount of roads.
    // Short rationale for estimate: Calls a function using dijkstras
    // algorithm.
    std::vector<TownID> shortest_route(TownID fromid, TownID toid);

    // Estimate of performance:
    // Short rationale for estimate:
    Distance trim_road_network();

private:
    // Add stuff needed for your class implementation here

    //Container for all towns
       std::unordered_map<TownID, TownData> TownContainer_;

       TownData* furthest_town_;

       int depth_;
       int running_depth_;

       std::unordered_map<TownID, Node> RoadContainer_;

       bool dest_found_;

       Node* last_from_cycle_;
       Node* cycle_node_;



       bool check_Road(TownID id);

       // Resets every node state and previus node, to be used before graph
       // algorithms.
       // complexity: worst case N^2, average linear
       void reset_nodes();

       // Estimate: N + V where N is the amount of towns in the graph and
       // V is the amount of roads.
       // Rationale: Works like DFS and DFS has a complexity of N + V
       void find_route(Node* node, TownID id);

       // Estimate: N + V where N is the amount of towns with roads and V
       // is the amount of roads.
       // Rationale: it's a DFS algorithm which has N + V complexity
       void find_cycle(Node* node);

       // Estimate: N + V where N is the amount of towns with roads and V
       // is the amount of roads.
       // Rationale: it's a BFS algorithm which has N + V complexity
       bool find_least_towns(Node* node, TownID id);

       // Estimate: N log V where N is the amount of towns with roads and V
       // is the amount of roads.
       // Rationale: Inner loop is operated N + V times and performs a log V
       // operation.
       bool find_shortest(Node* node, TownID id);

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
