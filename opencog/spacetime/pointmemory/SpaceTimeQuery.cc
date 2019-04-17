
#include <opencog/atoms/value/LinkValue.h>
#include <opencog/spacetime/octomap/OctoMapNode.h>
#include <opencog/spacetime/octomap/TimeOctomap.h>

using namespace opencog;

template <class Object>
struct SpaceTimeQuery
{
    static TimeOctomapPtr time_octomap;
    using ObjectSeq = std::vector<Object>;
    enum class Nearness
    {
        VERY_FAR,
        FAR,
        NEAR,
        VERY_NEAR
    };
    enum class Position
    {
        LEFT_SIDE,
        RIGHT_SIDE,
        INFRONT,
        BEHIND,
        TOP,
        BELOW
    };

#define E 0.001
    static bool is_near(const Object& object1, const Object& object2)
    {

        point3d_list pl1 = get_locations_of_atom(object1);
        point3d_list pl2 = get_locations_of_atom(object2);

        point3d p1 = *pl1.rbegin();
        point3d p2 = *pl2.rbegin();

        bool is_near = false;
        if ( sqrt(pow(p1.x()-p2.x(),2) + pow(p1.y()-p2.y(),2) + pow(p1.x()-p2.y(),2)) < E)  
            is_near = true;

        return is_near;
    }

    static Object& nearest(const Object& object)
    {
        point3d location = get_locations_of_atom(object2).rbegin();
        
            //for point3d location in ObjectList:
            dist =  distance(get_location(obj), get_location(object) 
                    if( dist <  obj_dist.first) :
                    obj_dist = make_pair(obj, dist)
    }

    static Nearness nearness(const Object& object1, const Object& object2)
    {
        /*
         *  location loc1 = get_location(object1)
         *  location loc2 = get_location(object2)
         *
         *  distance : distance(loc1,loc2)
         *
         *  return  distance < e1 ?  Nearness::VERY_NEAR :
         *          distance < e2 ?  Nearness::NEAR :
         *          distance < e3 and distance > e2 ? Nearness::FAR :
         *          Nearness::VERY_FAR;
         */      
    }

    static bool is_left_of(const Object& object1, const Object& object2)
    {
        /*
           location loc1 = get_location(object1)
           location loc2 = get_location(object2)

           return loc1.x - loc2.x < 0 ? true : false;
           */
    }

    static ObjectSeq entered_same_time(const Object& object)
    {
        // Query the time_octomap.
        /*objectSeq
          for_each slots : time_octomap:
          objs =   get_all_objects_at_time(slot)
          objectSeq.push_back(objes)

          return objectSeq*/

    }

    static ObjectSeq entered_shortly_after(const Object& object)
    {
        // Query the time_octomap.
        /*objectSeq
          for_each slots_next_to_objects_slot : time_octomap:
          objs =   get_all_objects_at_time(slot)
          objectSeq.push_back(objes)

          return objectSeq*/

    }

};


