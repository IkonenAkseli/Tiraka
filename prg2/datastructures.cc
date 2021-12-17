// Datastructures.cc
//
// Student name: Akseli Ikonen
// Student email: akseli.ikonen@tuni.fi
// Student number: K434701

#include "datastructures.hh"

#include <random>

#include <cmath>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{
    // Write any initialization you need here
}

Datastructures::~Datastructures()
{
    // Write any cleanup you need here
}

unsigned int Datastructures::town_count()
{

    return TownContainer_.size();
}

void Datastructures::clear_all()
{

    TownContainer_.clear();
    RoadContainer_.clear();
}

bool Datastructures::add_town(TownID id, const Name &name, Coord coord, int tax)
{

    if(check_Id(id)){
        return false;
    }


    TownContainer_.insert({id, {id, name, coord, tax,
                                std::unordered_set<TownData*>{}, nullptr}});

    return true;
}

Name Datastructures::get_town_name(TownID id)
{


    if(!check_Id(id)){
        return NO_NAME;
    }
    return TownContainer_.at(id).name;
}

Coord Datastructures::get_town_coordinates(TownID id)
{


    if(!check_Id(id)){
        return NO_COORD;
    }
    return TownContainer_.at(id).coord;

}

int Datastructures::get_town_tax(TownID id)
{

    if(!check_Id(id)){
        return NO_VALUE;
    }
    return TownContainer_.at(id).tax;

}

std::vector<TownID> Datastructures::all_towns()
{


    std::vector<TownID> towns;

    for(auto const &pair : TownContainer_){
        towns.push_back(pair.first);
    }

    return towns;
}

std::vector<TownID> Datastructures::find_towns(const Name &name)
{

    std::vector<TownID> towns = {};

    for( auto const& pair : TownContainer_){
        if(pair.second.name == name){
            towns.push_back(pair.first);
        }
    }

    return towns;
}

bool Datastructures::change_town_name(TownID id, const Name &newname)
{


    if(!check_Id(id)){
        return false;
    }

    TownContainer_.at(id).name = newname;


    return true;
}

std::vector<TownID> Datastructures::towns_alphabetically()
{
    // Replace the line below with your implementation
    //throw NotImplemented("towns_alphabetically()");

    std::multimap<Name, TownID> temp;
    std::vector<TownID> towns;

    // Add name Id pairs to multimap
    for(auto const& pair : TownContainer_){
        temp.insert({pair.second.name, pair.first});
    }
    // Get id from multimap as it is in order by name
    for(auto const& pair : temp){
        towns.push_back(pair.second);
    }
    return towns;

}

std::vector<TownID> Datastructures::towns_distance_increasing()
{


    return get_distance_vector({0,0});

}

TownID Datastructures::min_distance()
{


    if(town_count() == 0){
        return NO_TOWNID;
    }
    TownID town;

    std::vector<TownID> temp = get_distance_vector({0,0});
    // Get element from first position because vector is in ascending order
    town = temp.at(0);


    return town;
}

TownID Datastructures::max_distance()
{
    // Replace the line below with your implementation
    //throw NotImplemented("max_distance()");
    if(town_count() == 0){
        return NO_TOWNID;
    }
    TownID town;

    std::vector<TownID> temp = get_distance_vector({0,0});
    // Get element from last position because vector is in ascending order
    town = (temp.back());


    return town;
}

bool Datastructures::add_vassalship(TownID vassalid, TownID masterid)
{


    if(!check_Id(vassalid) or !check_Id(masterid)){
        return false;
    }

    // Check for vassal already having a master
    if(TownContainer_.at(vassalid).master != nullptr){
        return false;
    }

    TownContainer_.at(vassalid).master = &(TownContainer_.at(masterid));

    TownContainer_.at(masterid).vassals.insert(&(TownContainer_.at(vassalid)));

    return true;
}

std::vector<TownID> Datastructures::get_town_vassals(TownID id)
{

    std::vector<TownID> towns;

    if(!check_Id(id)){
        towns.push_back(NO_TOWNID);
        return towns;
    }

    auto vassals = &TownContainer_.at(id).vassals;

    // Add every vassal to the return vector
    for(auto vassal : *vassals){
        towns.push_back(vassal->id);
    }


    return towns;
}

std::vector<TownID> Datastructures::taxer_path(TownID id)
{

    std::vector<TownID> towns;
    if(!check_Id(id)){
        towns.push_back(NO_TOWNID);
        return towns;
    }
    TownData* town = &TownContainer_.at(id);

    // Call for getting the correct path in towns
    add_masters(towns, town);


    return towns;
}

bool Datastructures::remove_town(TownID id)
{

    if(!check_Id(id)){
        return false;
    }


    auto master = TownContainer_.at(id).master;
    std::unordered_set<TownData*>* vassals = &TownContainer_.at(id).vassals;
    auto town = &TownContainer_.at(id);


    // Set new master for each of the vassals, if the removed town had a master
    // add the removed towns vassals as its vassals
    for(auto& vassal : *vassals){
        vassal->master = master;
        if(master != nullptr){

            master->vassals.insert(vassal);
        }

    }
    // Remove town from vassals set of master town if the town had one
    if(master != nullptr){
        master->vassals.erase(town);
    }

    TownContainer_.erase(id);

    return true;
}

std::vector<TownID> Datastructures::towns_nearest(Coord coord)
{

    return get_distance_vector(coord);
}

std::vector<TownID> Datastructures::longest_vassal_path(TownID id)
{

    std::vector<TownID> towns;
        if(!check_Id(id)){
            towns.push_back(NO_TOWNID);
            return towns;
        }

    depth_ = 0;
    running_depth_ = 0;

    auto town = &(TownContainer_.at(id));

    // Return just the town if it has no vassals
    if(town->vassals.empty()){
        towns.push_back(id);
        return towns;
    }
    // Get the vassal that has most steps to it and store it in furthest_town_
    set_furthest_vassal(town);
    // Get masters of the furthest town to the town given
    // I.E fetches a vector containing all of the vassals from the longest
    // path in reverse order.
    add_masters(towns, furthest_town_, town);


    std::vector<TownID> towns_in_order;

    // Town itself is not in yet so add it to the front of the vector in order
    towns_in_order.push_back(id);

    // Reverse the order of the other vector and add it to the vector in order
    // To get the correct vector
    for(auto i = towns.rbegin(); i != towns.rend(); i++){
        towns_in_order.push_back(*i);
    }



    return towns_in_order;
}

int Datastructures::total_net_tax(TownID id)
{

    if(!check_Id(id)){
        return NO_VALUE;
    }

    int tax;

    auto town = &TownContainer_.at(id);

    // Get tax by calling a method
    tax = get_tax(town);

    // Reduce amount of taxes going to the master if town has a master
    if(town->master != nullptr){
        tax = tax - (tax/10);
    }

    return tax;
}


//
// Phase 2 operations
//


void Datastructures::clear_roads()
{
    // Replace the line below with your implementation
    //throw NotImplemented("clear_roads()");
    RoadContainer_.clear();
}

std::vector<std::pair<TownID, TownID>> Datastructures::all_roads()
{
    // Replace the line below with your implementation
    //throw NotImplemented("all_roads()");
    std::string str_to_hash;
    std::pair<std::string, std::string> id_pair;


    std::unordered_map<std::string, std::pair<TownID, TownID>> roads;


    for (auto &node : RoadContainer_){

        for(auto &neighbour : node.second.neighbours){

            // Combine town Id's in such a way that any two towns should
            // create a unique string to act as a key for the unordered map
            // this allows for easy duplicate detection
            str_to_hash = "";

            if (node.first < neighbour.first->id){
                id_pair.first = node.first;
                id_pair.second = neighbour.first->id;

                str_to_hash = id_pair.first + "abcdkopl" + id_pair.first +
                        id_pair.second;


            }
            else{
                id_pair.first = neighbour.first->id;
                id_pair.second = node.first;

                str_to_hash = id_pair.first + "abcdkopl" + id_pair.first
                        + id_pair.second;

            }
            if(roads.find(str_to_hash) == roads.end()){

                roads[str_to_hash] = id_pair;
            }


        }
    }
    std::vector<std::pair<TownID, TownID>> road_vector;

    for(auto &road : roads){
        road_vector.push_back(road.second);
    }

    return road_vector;


}

bool Datastructures::add_road(TownID town1, TownID town2)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    //throw NotImplemented("add_road()");

    if(!check_Id(town1) or !check_Id(town2)){
        return false;
    }
    int distance;
    Node node;
    node.neighbours = {};

    // Adding towns to data structure if no road leads to them yet
    if(!check_Road(town1)){
        node.id = town1;
        RoadContainer_.insert({town1,node});
    }
    if(!check_Road(town2)){
        node.id = town2;
        RoadContainer_.insert({town2,node});
    }



    // Checking if the towns already have a road between them
    for(auto &neighbour : RoadContainer_.at(town1).neighbours){
        if(neighbour.first->id == town2){
            return false;
        }
    }
    distance = get_distance(TownContainer_.at(town1).coord,
                            TownContainer_.at(town2).coord);

    auto ptr_1 = &RoadContainer_.at(town1);
    auto ptr_2 = &RoadContainer_.at(town2);

    RoadContainer_.at(town1).neighbours.insert({ptr_2,distance});
    RoadContainer_.at(town2).neighbours.insert({ptr_1,distance});





    return true;
}

std::vector<TownID> Datastructures::get_roads_from(TownID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    //throw NotImplemented("get_roads_from()");

    std::vector<TownID> towns;

    if(!check_Id(id)){
        towns.push_back(NO_TOWNID);
        return towns;
    }
    if (check_Road(id)){
        for (auto &neighbor : RoadContainer_.at(id).neighbours){
            towns.push_back(neighbor.first->id);
        }
    }

    return towns;

}

std::vector<TownID> Datastructures::any_route(TownID fromid, TownID toid)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    //throw NotImplemented("any_route()");
    std::vector<TownID> towns;
    std::vector<TownID> towns_reverse;

//    if(!check_Id(fromid) or !check_Id(toid)){
//        towns.push_back(NO_TOWNID);
//        return towns;
//    }

    if(!check_Road(fromid) or !check_Road(toid)){
        towns.push_back(NO_TOWNID);
        return towns;
    }



//    for(auto& neighbour : RoadContainer_.at(fromid).neighbours){
//        find_route(neighbour.first, toid);
//    }

    reset_nodes();
    dest_found_ = false;
    RoadContainer_.at(toid).status = 1;

    find_route(&RoadContainer_.at(toid), fromid);

    auto ptr = RoadContainer_.at(fromid).from;

    towns_reverse.push_back(fromid);

    while (ptr != nullptr){
        towns_reverse.push_back(ptr->id);
        ptr = ptr->from;
    }

    //towns_reverse.push_back(fromid);

    for(auto i = towns_reverse.rbegin(); i != towns_reverse.rend(); i++){
        towns.push_back(*i);
    }

    return towns_reverse;
}

bool Datastructures::remove_road(TownID town1, TownID town2)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    //throw NotImplemented("remove_road()");
    if(!check_Road(town1) or !check_Road(town2)){
        return false;
    }
    bool road_found = false;
    auto ptr_1 = &RoadContainer_.at(town1);

    for(auto &neighbour : ptr_1->neighbours){
        if(neighbour.first->id == town2){
            road_found = true;
        }
    }
    if(!road_found){
        return false;
    }


    auto ptr_2 = &RoadContainer_.at(town2);


    // Remove towns from graph if they're left with no roads after removal
    // if not then remove roads from neighbours.
    if( ptr_1->neighbours.size() == 1){
        RoadContainer_.erase(town1);
    }
    else {
         ptr_1->neighbours.erase(ptr_2);
    }
    if( ptr_2->neighbours.size() == 1){
        RoadContainer_.erase(town2);
    }
    else {
         ptr_2->neighbours.erase(ptr_1);
    }

    return true;
}

std::vector<TownID> Datastructures::least_towns_route(TownID fromid, TownID toid)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    //throw NotImplemented("least_towns_route()");
    std::vector<TownID> towns;

    if(!check_Road(fromid) or !check_Road(toid)){
        towns.push_back(NO_TOWNID);
        return towns;
    }

    reset_nodes();
//    dest_found_ = false;
//    RoadContainer_.at(fromid).cost = 0;
    // Call from end to beginning since the result comes backwards
    if(!find_least_towns(&RoadContainer_.at(toid), fromid)){
        return towns;
    }
    //std::vector<TownID> towns_reverse;
    auto ptr = &RoadContainer_.at(fromid);

    while (ptr != nullptr){
        towns.push_back(ptr->id);
        ptr = ptr->from;
    }

//    for(auto i = towns_reverse.rbegin(); i != towns_reverse.rend(); i++){
//        towns.push_back(*i);
//    }

    return towns;

}

std::vector<TownID> Datastructures::road_cycle_route(TownID startid)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    //throw NotImplemented("road_cycle_route()");
    std::vector<TownID> towns;
    std::vector<TownID> towns_reverse;
//    if(!check_Id(fromid) or !check_Id(toid)){
//        towns.push_back(NO_TOWNID);
//        return towns;
//    }
    if(!check_Road(startid)){
        towns.push_back(NO_TOWNID);
        return towns;
    }

    reset_nodes();

    //RoadContainer_.at(startid).status = 1;



//    for(auto& neighbour : RoadContainer_.at(startid).neighbours){
//        neighbour.first->from = &RoadContainer_.at(startid);
//        find_cycle(neighbour.first);
//    }

    dest_found_ = false;
    last_from_cycle_ = nullptr;

    find_cycle(&RoadContainer_.at(startid));

    if(!dest_found_){
        return towns;
    }

    auto ptr = last_from_cycle_;

    towns_reverse.push_back(cycle_node_->id);

    while (ptr != nullptr){
        towns_reverse.push_back(ptr->id);
        ptr = ptr->from;
    }



    for(auto i = towns_reverse.rbegin(); i != towns_reverse.rend(); i++){
        towns.push_back(*i);
    }
    //towns.push_back(cycle_node_->id);

    return towns;
}

std::vector<TownID> Datastructures::shortest_route(TownID fromid, TownID toid)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    //throw NotImplemented("shortest_route()");
    std::vector<TownID> towns;

    if(!check_Road(fromid) or !check_Road(toid)){
        towns.push_back(NO_TOWNID);
        return towns;
    }

    reset_nodes();
//    dest_found_ = false;
//    RoadContainer_.at(fromid).cost = 0;

    // Calling backwards so so that the result comes in the right order
    if(!find_shortest(&RoadContainer_.at(toid), fromid)){
        return towns;
    }
    //std::vector<TownID> towns_reverse;
    auto ptr = &RoadContainer_.at(fromid);

    while (ptr != nullptr){
        towns.push_back(ptr->id);
        ptr = ptr->from;
    }

//    for(auto i = towns_reverse.rbegin(); i != towns_reverse.rend(); i++){
//        towns.push_back(*i);
//    }

    return towns;


}

Distance Datastructures::trim_road_network()
{
    // Replace the line below with your implementation
    throw NotImplemented("trim_road_network()");
}

bool Datastructures::check_Road(TownID id)
{
    if (RoadContainer_.find(id) == RoadContainer_.end()){
        return false;
    }
    return true;
}

void Datastructures::reset_nodes()
{
    for(auto &node : RoadContainer_){
        node.second.from = nullptr;
        node.second.status = 0;
        node.second.cost = 999999;
    }
}

void Datastructures::find_route(Node *node, TownID id)
{

    if (dest_found_){
        return;
    }
    // Flagging that the destination is found
    if(node->id == id){
        dest_found_ = true;
        return;
    }
    node->status = 1;

    // Keeps going depth first and setting previous nodes until destination
    // is found or there are no more nodes left.
    for(auto neighbour : node->neighbours){

        if(!dest_found_ and neighbour.first->status == 0){
            neighbour.first->from = node;

            find_route(neighbour.first, id);
        }


        if(dest_found_){
            break;
        }
    }
}

void Datastructures::find_cycle(Node *node)
{
    // This code is not needed but is preserved just in case.
    // Upon arrival to a node for the second time destination is set as found
//    if (node->status == 1){
//        dest_found_ = true;
//    }

    node->status = 1;

    for(auto neighbour : node->neighbours){

        // Keeping track of which node is the last before one is entered again
        // and which node was entered twice.
        last_from_cycle_ = node;
        cycle_node_ = neighbour.first;

        if(dest_found_){
            break;
        }

        // A cycle is found when the next node to be accesed has been accessed
        // before but isn't the previous node.
        if(neighbour.first->status != 0 and neighbour.first
                != node->from){
            dest_found_ = true;
        }

        if(!dest_found_ and neighbour.first->status == 0){
            neighbour.first->from = node;



            find_cycle(neighbour.first);
        }


        if(dest_found_){
            break;
        }
    }

}

bool Datastructures::find_least_towns(Node *node, TownID id)
{


    std::queue<Node*> Q;
    Q.push(node);

    while(!Q.empty()){

        auto u = Q.front();
        Q.pop();

        for (auto &neighbour : u->neighbours){
            if (neighbour.first->status == 0){

                neighbour.first->status = 1;
                neighbour.first->cost = u->cost + 1;
                // Keep track of the route traversed
                neighbour.first->from = u;

                if (neighbour.first->id == id){
                    return true;
                }
                Q.push(neighbour.first);
            }
        }
        u->status = 3;
    }
    return false;


}

bool Datastructures::find_shortest(Node *node, TownID id)
{
    std::set<std::pair<int, Node*>> Q;
    node->cost = 0;
    node->status = 1;
    Q.insert({0,node});
    bool cost_changed = false;
    int temp_cost = 0;

    while(!Q.empty()){
        auto u = *Q.begin();
        Q.erase(Q.begin());

        if(u.second->id == id){
            return true;
        }

        for (auto &neighbour : u.second->neighbours){

            auto cost1 = neighbour.first->cost;
            auto cost2 = u.second->cost;

            cost_changed = false;

            // Change adjacent node cost if a cheaper route is found
            if(cost1 > cost2 + neighbour.second){

                temp_cost = neighbour.first->cost;
                neighbour.first->cost = cost2 + neighbour.second;
                neighbour.first->from = u.second;
                cost_changed = true;
            }


            if (neighbour.first->status == 0){


                neighbour.first->status = 1;



                Q.insert({neighbour.first->cost,neighbour.first});
            }
            // Update the changed cost
            else if (cost_changed){
                Q.erase({temp_cost,neighbour.first});
                Q.insert({neighbour.first->cost,neighbour.first});
            }
        }
        u.second->status = 3;
    }
    return false;
}

// Start of phase one helper methods

bool Datastructures::check_Id(TownID id)
{

    if(TownContainer_.find(id)==TownContainer_.end()){
        return false;
    }
    return true;
}

int Datastructures::get_distance(Coord coord1, Coord coord2)
{

    // Calculation for eucledic distance between coord1 and coord2
    // cast to int for rounding down
    float temp = sqrt(pow(coord1.x-coord2.x,2)+pow(coord1.y-coord2.y,2)*1.0);
    int dist = temp;

    return dist;
}

void Datastructures::add_masters(std::vector<TownID> &masters, TownData* town,
                                 TownData* stop)
{
    masters.push_back(town->id);

    if(town->master != stop){

        add_masters(masters, town->master, stop);
    }
}

// This method isn't used, but is preserved in case it ends up being needed
/*
std::vector<TownID> Datastructures::get_path_ids(TownData *child)
{
    std::vector<TownID> towns;
    if(child->vassals.size() == 0){
        towns.push_back(child->id);
        return towns;
    }


    for(auto vassal : child->vassals){

        std::vector<TownID> loop_temp;

        loop_temp = get_path_ids(vassal);
        loop_temp.push_back(child->id);

        if(loop_temp.size() > towns.size()){
            towns.clear();
            for(auto &id : loop_temp){
                towns.push_back(id);
            }
        }
    }

    return towns;
}*/

int Datastructures::get_tax(TownData *town)
{
    int tax = town->tax;

    for(auto vassal : town->vassals){
        tax += get_tax(vassal)/10;

    }

    return tax;
}

std::vector<TownID> Datastructures::get_distance_vector(Coord coord1)
{
    std::multimap<int, TownID> temp;
    std::vector<TownID> towns;

    Coord location;
    int dist;

    // Get distance and id pairs and insert them to a multimap
    for(auto const& pair : TownContainer_){
        location = pair.second.coord;

        // Call get_distance for calculating the distance between
        // Town location and the given point
        dist = get_distance(location, coord1);

        temp.insert({dist, pair.first});
    }
    // Get ID:s from multimap and add them to a vector
    for(auto const& pair : temp){
        towns.push_back(pair.second);
    }
    return towns;
}

void Datastructures::set_furthest_vassal(TownData *town)
{
    // Increase depth when moving further down the "tree" of vassals
    running_depth_++;

    // Adds current town as furthest vassal away if current town has no vassals
    // and is further away than a town already found.
    if(town->vassals.empty() && running_depth_ > depth_){
        depth_ = running_depth_;
        furthest_town_ = town;
        // Reduce depth when moving back
        running_depth_--;
        return;
    }

    // Go through all vassals down the tree
    for(auto vassal : town->vassals){
        set_furthest_vassal(vassal);
    }

    // Reduce depth when moving back
    running_depth_--;
}

// End of phase one helper methods
