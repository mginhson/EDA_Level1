/**
 * @brief Orbital simulation
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

// Enables M_PI #define in Windows
//effects on unix?
#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "OrbitalSim.h"
#include "ephemerides.h"

#define GRAVITATIONAL_CONSTANT 6.6743E-11F
#define ASTEROIDS_MEAN_RADIUS 4E11F


static void translateBody(const EphemeridesBody * const _ephemerid_body, 
                          OrbitalBody * const _orbital_body);


/**
 * @brief Gets a uniform random value in a range
 *
 * @param min Minimum value
 * @param max Maximum value
 * @return The random value
 */
float getRandomFloat(float min, float max)
{
    return min + (max - min) * rand() / (float)RAND_MAX;
}

/**
 * @brief Configures an asteroid
 *
 * @param body An orbital body
 * @param centerMass The mass of the most massive object in the star system
 */
void configureAsteroid(OrbitalBody *body, float centerMass)
{
    // Logit distribution
    float x = getRandomFloat(0, 1);
    float l = logf(x) - logf(1 - x) + 1;

    // https://mathworld.wolfram.com/DiskPointPicking.html
    float r = ASTEROIDS_MEAN_RADIUS * sqrtf(fabsf(l));
    float phi = getRandomFloat(0, 2.0F * (float)M_PI);

    // Surprise!
    // phi = 0;

    // https://en.wikipedia.org/wiki/Circular_orbit#Velocity
    float v = sqrtf(GRAVITATIONAL_CONSTANT * centerMass / r) * getRandomFloat(0.6F, 1.2F);
    float vy = getRandomFloat(-1E2F, 1E2F);

    // Fill in with your own fields:
    // body->mass = 1E12F;  // Typical asteroid weight: 1 billion tons
    // body->radius = 2E3F; // Typical asteroid radius: 2km
    // body->color = GRAY;
    // body->position = {r * cosf(phi), 0, r * sinf(phi)};
    // body->velocity = {-v * sinf(phi), vy, v * cosf(phi)};
}

/**
 * @brief Constructs an orbital simulation
 *
 * @param timeStep: floating point value, ideally should be a multiple of the FPS,
 *                  will still work fine otherwise.
 * @return The constructed orbital simulation. Returns NULL on error.
 */
OrbitalSim *constructOrbitalSim(double timeStep)
{
    OrbitalSim * simulation = NULL;
    
    unsigned int i; //index
    
    simulation = (OrbitalSim*) malloc(sizeof(OrbitalSim));
    simulation->bodies = (OrbitalBody*)malloc(SOLARSYSTEM_BODYNUM * sizeof(OrbitalBody));
    
    if(simulation->bodies == NULL) //malloc failed, return NULL
    {
        return NULL;
    }

    for(i = 0; i < SOLARSYSTEM_BODYNUM; i++)
    {
        translateBody(&solarSystem[i],&simulation->bodies[i]);
    }
    return simulation; 
}

/**
 * @brief Destroys an orbital simulation
 */
void destroyOrbitalSim(OrbitalSim *sim)
{
    // Your code goes here...
    if ()

}

/**
 * @brief Simulates a timestep
 *
 * @param sim The orbital simulation
 */
void updateOrbitalSim(OrbitalSim *sim)
{
    // Your code goes here...


}




static void translateBody(const EphemeridesBody * const _ephemerid_body, 
                          OrbitalBody * const _orbital_body)
{
    _orbital_body->acceleration = (Vector3){.x = 0.0F, .y = 0.0F, .z = 0.0F};
    _orbital_body->action = (void(*)(void))NULL;
    _orbital_body->mass = _ephemerid_body->mass;
    _orbital_body->position = _ephemerid_body->position; //both are Vector3
    _orbital_body->radius = (double) _ephemerid_body->radius;
    _orbital_body->velocity = _ephemerid_body->velocity; //both are Vector3


    return;
}   