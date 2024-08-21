/**
 * @brief Orbital simulation
 * @author Marc S. Ressl
 * @modifiers Matteo Ginhson, Nicanor Otamendi
 * @copyright Copyright (c) 2022-2023
 */

#ifndef ORBITALSIM_H
#define ORBITALSIM_H

#include "raylib.h"
#include "raymath.h"

#define ASTEROIDS_COUNT 1000

/**
 * @brief OrbitalBody definition
 * A few physical parameters each body has.
 */
struct OrbitalBody
{
    
    double mass;            // [kg]
    double one_over_mass;   // 1/[kg]
    double radius;          // [m]
    Vector3 position;       // {[m],[m],[m]}, distance from origin on each axis
    Vector3 acceleration;   // {[m/s^2],[m/s^2],[m/s^2]}
    Vector3 velocity;       // {[m/s],[m/s],[m/s]}
    Vector3 applied_force;  // [N]
    Color color;            // raylib color
};

/**
 * @brief 
 * The object defining a simulation instance. It has a list of its bodies,
 * its timestep
 * This already feels like Outer Wilds!
 */
struct OrbitalSim
{
    OrbitalBody * bodies;
    unsigned int bodies_count;
    unsigned int planets_range;  
    double time_step;           //[s]
    double time_elapsed;        //[s], defining 0 seconds as the start of the simulation   

};

OrbitalSim *constructOrbitalSim(double timeStep);
OrbitalSim *constructOrbitalSim_BONUS(double timeStep);
void destroyOrbitalSim(OrbitalSim *sim);
void updateOrbitalSim(OrbitalSim *sim);
void updateOrbitalSimOptimized(OrbitalSim *sim);

#endif