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
