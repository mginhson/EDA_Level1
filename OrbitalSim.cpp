/**
 * @brief Orbital simulation
 * @author Marc S. Ressl
 * @modifiers Matteo Ginhson, Nicanor Otamendi
 * @copyright Copyright (c) 2022-2023
 */

// Enables M_PI #define in Windows

#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <stdio.h>
#include <math.h>



#include "OrbitalSim.h"
#include "ephemerides.h"

#define GRAVITATIONAL_CONSTANT 6.6743E-11F

/**
 * This value was originally 2E11F, was changed to have greater sparsity between asteroids,
 * all of them would be cluttered and close to the Sun otherwise.
*/
#define ASTEROIDS_MEAN_RADIUS 9E11F 



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
    //phi = 0;

    // https://en.wikipedia.org/wiki/Circular_orbit#Velocity
    float v = sqrtf(GRAVITATIONAL_CONSTANT * centerMass / r) * getRandomFloat(0.6F, 1.2F);
    float vy = getRandomFloat(-1E2F, 1E2F);

    // Fill in with your own fields:
    body->mass = 1E12F;  // Typical asteroid weight: 1 billion tons
    body->radius = 2E3F; // Typical asteroid radius: 2km
    body->color = GRAY;
    body->position = {r * cosf(phi), 0, r * sinf(phi)};
    body->velocity = {-v * sinf(phi), vy, v * cosf(phi)};
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
    if(simulation == NULL) //malloc failed, return NULL
        return NULL;
    
    //Loads the count of how many bodies are there on the simulation
    simulation->bodies_count = SOLARSYSTEM_BODYNUM + ASTEROIDS_COUNT;
    
    //The first SOLARSYSTEM_BODYNUM bodies are the planets, the ones after this mark are asteroids
    simulation->planets_range = SOLARSYSTEM_BODYNUM;


    simulation->bodies = (OrbitalBody*) malloc(simulation->bodies_count * sizeof(OrbitalBody));
    if(simulation->bodies == NULL) //malloc failed, return NULL
        return NULL;
    

    for(i = 0; i < SOLARSYSTEM_BODYNUM; i++)
    {
        translateBody(&solarSystem[i],&simulation->bodies[i]);
    }

    for(i = simulation->planets_range; i < simulation->bodies_count; i++)
        configureAsteroid(&simulation->bodies[i],simulation->bodies[0].mass);

    for(i = 0; i < simulation->bodies_count; ++i)
    {
        simulation->bodies[i].applied_force = (Vector3){0.0f, 0.0f, 0.0f};
    }
    simulation->time_step = timeStep;
    return simulation; 
}

OrbitalSim *constructOrbitalSim_BONUS(double timeStep)
{
    OrbitalSim * simulation = NULL;
    
    unsigned int i; //index
    
    simulation = (OrbitalSim*) malloc(sizeof(OrbitalSim));
    if(simulation == NULL) //malloc failed, return NULL
        return NULL;
    
    //Loads the count of how many bodies are there on the simulation
    simulation->bodies_count = ALPHACENTAURISYSTEM_BODYNUM + ASTEROIDS_COUNT;
    
    //The first SOLARSYSTEM_BODYNUM bodies are the planets, the ones after this mark are asteroids
    simulation->planets_range = ALPHACENTAURISYSTEM_BODYNUM;


    simulation->bodies = (OrbitalBody*) malloc(simulation->bodies_count * sizeof(OrbitalBody));
    if(simulation->bodies == NULL) //malloc failed, return NULL
        return NULL;
    

    for(i = 0; i < simulation->planets_range; i++)
    {
        translateBody(&alphaCentauriSystem[i],&simulation->bodies[i]);
    }

    for(i = simulation->planets_range; i < simulation->bodies_count; i++)
        configureAsteroid(&simulation->bodies[i],simulation->bodies[0].mass);

    for(i = 0; i < simulation->bodies_count; ++i)
    {
        simulation->bodies[i].applied_force = (Vector3){0.0f, 0.0f, 0.0f};
    }
    simulation->time_step = timeStep;
    return simulation; 
}


/**
 * @brief Destroys an orbital simulation
 */
void destroyOrbitalSim(OrbitalSim *sim)
{
    //Both were malloc'ed
    free(sim->bodies);
    free(sim);
}

/**
 * @brief updates a simulation instance
 *
 * @param sim: a pointer to the simulation instance
 * @return nothing
 */
void updateOrbitalSim(OrbitalSim *sim)
{
  
    unsigned int current,walker;
    Vector3 total_force,single_force;
    Vector3 vec_diff,vec_diff_normalized;
    Vector3 target_delta_velocity,target_delta_position;
    double vec_diff_length_sqr, coefficient;

    
   for(current = 0; current < sim->bodies_count; current++)
    {
        total_force = (Vector3){0.0, 0.0, 0.0};
        for(walker = 0; walker < sim->bodies_count; walker++)
        {
            if(walker == current)
                continue;
            vec_diff = Vector3Subtract(sim->bodies[current].position,sim->bodies[walker].position);
            vec_diff_normalized = Vector3Normalize(vec_diff);
           
            vec_diff_length_sqr = (double) Vector3LengthSqr(vec_diff);

            coefficient = (double) (-GRAVITATIONAL_CONSTANT) * 
                                    (sim->bodies[current].mass * sim->bodies[walker].mass) /
                                    vec_diff_length_sqr;
            single_force = Vector3Scale(vec_diff_normalized,coefficient);
            total_force = Vector3Add(single_force,total_force);
        }

        //Calculates body accel based on force
        sim->bodies[current].acceleration = Vector3Scale(total_force,(double)1/sim->bodies[current].mass);
        
        target_delta_velocity = Vector3Scale(sim->bodies[current].acceleration, sim->time_step);
        sim->bodies[current].velocity = Vector3Add(sim->bodies[current].velocity,target_delta_velocity);
        
        target_delta_position = Vector3Scale(sim->bodies[current].velocity,sim->time_step);
        sim->bodies[current].position = Vector3Add(sim->bodies[current].position, target_delta_position);     
    }
    sim->time_elapsed += sim->time_step;    

}




/**
 * @brief updates a simulation instance
 *
 * @param sim: a pointer to the simulation instance
 * @return nothing
 */
void updateOrbitalSimOptimized(OrbitalSim *sim)
{
  
     
    unsigned int current,walker;
    Vector3 single_force,inverted_single_force;
    Vector3 vec_diff,vec_diff_normalized;
    Vector3 target_delta_velocity,target_delta_position;
    double vec_diff_length_sqr, coefficient;

    
   for(current = 0; current < sim->bodies_count; current++)
    {
        
        for(walker = current+1; walker < sim->bodies_count; walker++)
        {
            
            vec_diff = Vector3Subtract(sim->bodies[current].position,sim->bodies[walker].position);
            vec_diff_normalized = Vector3Normalize(vec_diff);
           
            vec_diff_length_sqr = (double) Vector3LengthSqr(vec_diff);

            coefficient = (double) (-GRAVITATIONAL_CONSTANT) * 
                                    (sim->bodies[current].mass * sim->bodies[walker].mass) /
                                    vec_diff_length_sqr;
            single_force = Vector3Scale(vec_diff_normalized,coefficient);
            inverted_single_force = single_force;
            inverted_single_force.x *= -1.0f;
            inverted_single_force.y *= -1.0f;
            inverted_single_force.z *= -1.0f;
            sim->bodies[current].applied_force = Vector3Add(single_force,sim->bodies[current].applied_force);
            sim->bodies[walker].applied_force = Vector3Add(inverted_single_force,sim->bodies[walker].applied_force);
        }

        //Calculates body accel based on force
        sim->bodies[current].acceleration = Vector3Scale(sim->bodies[current].applied_force,(double)1/sim->bodies[current].mass);
        
        target_delta_velocity = Vector3Scale(sim->bodies[current].acceleration, sim->time_step);
        sim->bodies[current].velocity = Vector3Add(sim->bodies[current].velocity,target_delta_velocity);
        
        target_delta_position = Vector3Scale(sim->bodies[current].velocity,sim->time_step);
        sim->bodies[current].position = Vector3Add(sim->bodies[current].position, target_delta_position);

        //Resets the applied force since the next calculation loop expects it like that
        sim->bodies[current].applied_force = (Vector3){0.0, 0.0, 0.0};      
    }
    sim->time_elapsed += sim->time_step;    

}

/** 
 * @brief translates the members of type EphemeridesBody to type OrbitalBody
 * @param _ephemerid_body a ptr to an EphemeridBody (source)
 * @param _orbital_body a ptr to an OrbitalBody (destination)
 * @return nothing
 */
static void translateBody(const EphemeridesBody * const _ephemerid_body, 
                          OrbitalBody * const _orbital_body)
{
    _orbital_body->acceleration = (Vector3){.x = 0.0F, .y = 0.0F, .z = 0.0F};
    _orbital_body->mass = (double) _ephemerid_body->mass;
    _orbital_body->position = _ephemerid_body->position; //both are Vector3
    _orbital_body->radius = (double) _ephemerid_body->radius;
    _orbital_body->velocity = _ephemerid_body->velocity; //both are Vector3
    _orbital_body->color = _ephemerid_body->color;
    _orbital_body->one_over_mass = 1/_orbital_body->mass;
    return;
}   