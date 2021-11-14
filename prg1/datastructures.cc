// Datastructures.cc
//
// Student name:
// Student email:
// Student number:

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
    // Replace the line below with your implementation
    //throw NotImplemented("town_count()");
    return TownContainer_.size();
}

void Datastructures::clear_all()
{
    // Replace the line below with your implementation
    //throw NotImplemented("clear_all()");
    TownContainer_.clear();
}

bool Datastructures::add_town(TownID id, const Name &name, Coord coord, int tax)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    //throw NotImplemented("add_town()");
    if(check_Id(id)){
        return false;
    }

    TownData temp;
    temp.id = id;
    temp.name = name;
    temp.coord = coord;
    temp.tax = tax;
    temp.master = "NaN";
    temp.vassals = {};

    //TownContainer_.insert({id, {id, name, coord, tax, "none", std::vector<TownID>}});
    TownContainer_.insert({id, temp});


    return true;
}

Name Datastructures::get_town_name(TownID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    //throw NotImplemented("get_town_name()");

    if(!check_Id(id)){
        return NO_NAME;
    }
    return TownContainer_.at(id).name;
}

Coord Datastructures::get_town_coordinates(TownID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    //throw NotImplemented("get_town_coordinates()");

    if(!check_Id(id)){
        return NO_COORD;
    }
    return TownContainer_.at(id).coord;

}

int Datastructures::get_town_tax(TownID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    //throw NotImplemented("get_town_tax()");
    if(!check_Id(id)){
        return NO_VALUE;
    }
    return TownContainer_.at(id).tax;

}

std::vector<TownID> Datastructures::all_towns()
{
    // Replace the line below with your implementation
    //throw NotImplemented("all_towns()");

    std::vector<TownID> towns;

    for(auto const &pair : TownContainer_){
        towns.push_back(pair.first);
    }

    return towns;
}

std::vector<TownID> Datastructures::find_towns(const Name &name)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    //throw NotImplemented("find_towns()");
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
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    //throw NotImplemented("change_town_name()");

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

    for(auto const& pair : TownContainer_){
        temp.insert({pair.second.name, pair.first});
    }
    for(auto const& pair : temp){
        towns.push_back(pair.second);
    }
    return towns;

}

std::vector<TownID> Datastructures::towns_distance_increasing()
{
    // Replace the line below with your implementation
    //throw NotImplemented("towns_distance_increasing()");
    std::multimap<int, TownID> temp;
    std::vector<TownID> towns;
    Coord origo = {0,0};
    Coord location;
    int dist;

    for(auto const& pair : TownContainer_){
        location = pair.second.coord;
        dist = get_distance(location, origo);
        temp.insert({dist, pair.first});
    }
    for(auto const& pair : temp){
        towns.push_back(pair.second);
    }
    return towns;

}

TownID Datastructures::min_distance()
{
    // Replace the line below with your implementation
    //throw NotImplemented("min_distance()");

    if(town_count() == 0){
        return NO_TOWNID;
    }
    TownID town;

    std::vector<TownID> temp = towns_distance_increasing();
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

    std::vector<TownID> temp = towns_distance_increasing();
    town = (temp.back());


    return town;
}

bool Datastructures::add_vassalship(TownID vassalid, TownID masterid)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    //throw NotImplemented("add_vassalship()");

    if(!check_Id(vassalid) or !check_Id(masterid)){
        return false;
    }
    if(TownContainer_.at(vassalid).master != "NaN"){
        return false;
    }
    TownContainer_.at(vassalid).master = masterid;
    TownContainer_.at(masterid).vassals.push_back(vassalid);

    return true;
}

std::vector<TownID> Datastructures::get_town_vassals(TownID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    //throw NotImplemented("get_town_vassals()");
    std::vector<TownID> towns;
    if(!check_Id(id)){
        towns.push_back(NO_TOWNID);
        return towns;
    }

    towns = TownContainer_.at(id).vassals;

    return towns;
}

std::vector<TownID> Datastructures::taxer_path(TownID id)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    //throw NotImplemented("taxer_path()");
    std::vector<TownID> towns;
    if(!check_Id(id)){
        towns.push_back(NO_TOWNID);
        return towns;
    }
    add_master(towns, id);


    return towns;
}

bool Datastructures::remove_town(TownID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("remove_town()");
}

std::vector<TownID> Datastructures::towns_nearest(Coord /*coord*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("towns_nearest()");
}

std::vector<TownID> Datastructures::longest_vassal_path(TownID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("longest_vassal_path()");
}

int Datastructures::total_net_tax(TownID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("total_net_tax()");
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

    float temp = sqrt(pow(coord1.x-coord2.x,2)+pow(coord1.y-coord2.y,2)*1.0);
    int dist = temp;

    return dist;
}

void Datastructures::add_master(std::vector<TownID> &masters, TownID id)
{
    if(TownContainer_.at(id).master != "NaN"){
        masters.push_back(TownContainer_.at(id).master);
        add_master(masters, TownContainer_.at(id).master);
    }
}
