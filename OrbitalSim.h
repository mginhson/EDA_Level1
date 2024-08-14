/**
 * @brief Orbital simulation
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

#ifndef ORBITALSIM_H
#define ORBITALSIM_H

#include "raylib.h"
/**
 * @brief 
 * Each body has 
 */

struct OrbitalBody
{
    
    double mass;   //[kg]
    double radius; //[m]
    Vector3 position; //{[m],[m],[m]}, distance from origin on each axis
    Vector3 acceleration; //{[m/s^2],[m/s^2],[m/s^2]}
    Vector3 velocity; //{[m/s],[m/s],[m/s]}
    Color color;    // raylib color
    
    void (*action)(void);
};

/**
 * @brief Orbital simulation definition
 * This already feels like Outer Wilds!
 */
struct OrbitalSim
{
    // Fill in your code here...
    OrbitalBody * bodies;
    double time_step;
    double time_elapsed;
    uint32_t bodies_count;
};

OrbitalSim *constructOrbitalSim(double timeStep);
void destroyOrbitalSim(OrbitalSim *sim);

void updateOrbitalSim(OrbitalSim *sim);

#endif
