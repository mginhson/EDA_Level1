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
    
    simulation->bodies_count = SOLARSYSTEM_BODYNUM + ASTEROIDS_COUNT;
    
    simulation->bodies = (OrbitalBody*) malloc(simulation->bodies_count * sizeof(OrbitalBody));
    if(simulation->bodies == NULL)
        return NULL;
    

    for(i = 0; i < SOLARSYSTEM_BODYNUM; i++)
    {
        translateBody(&solarSystem[i],&simulation->bodies[i]);
        //printf("%lf %lf\n",solarSystem[i].radius,simulation->bodies[i].radius);
    }
    for(i = SOLARSYSTEM_BODYNUM; i < simulation->bodies_count; i++)
        configureAsteroid(&simulation->bodies[i],simulation->bodies[0].mass);
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
 * @brief Simulates a timestep
 *
 * @param sim The orbital simulation
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


void updateOrbitalSimOptimized(OrbitalSim *sim)
{
  
    unsigned int current,walker;
    Vector3 total_force,single_force;
    Vector3 vec_diff,vec_diff_normalized;
    Vector3 target_delta_velocity,target_delta_position;
    double vec_diff_length_sqr, coefficient;

    //This loop expects ALL of sim->bodies[].applied_force to be zero
    for(current = 0; current < sim->bodies_count; current++)
    {
        
        
        for(walker = current+1; walker < sim->bodies_count; walker++)
        {
            
            vec_diff = Vector3Subtract(sim->bodies[current].position,sim->bodies[walker].position);
            vec_diff_normalized = Vector3Normalize(vec_diff);
           
            vec_diff_length_sqr = (double) Vector3LengthSqr(vec_diff);

            coefficient = (double) (-GRAVITATIONAL_CONSTANT * 
                                    sim->bodies[walker].mass) /
                                    vec_diff_length_sqr;
            single_force = Vector3Scale(vec_diff_normalized,coefficient);
            sim->bodies[current].applied_force = Vector3Add(single_force,sim->bodies[current].applied_force);

            single_force.x *= -1.0F;
            single_force.z *= -1.0F;
            single_force.y *= -1.0F;
            sim->bodies[walker].applied_force = Vector3Add(Vector3Scale(single_force,1/sim->bodies[current].mass), sim->bodies[walker].applied_force);
            
        }
        /**
             * Since the force body A applies to body B is the opposite
             * of the force B applies to A, we apply these results to both the
             * walker and current vector
            */
        //Calculates body accel based on force
        sim->bodies[current].acceleration = sim->bodies[current].applied_force;
        
        target_delta_velocity = Vector3Scale(sim->bodies[current].acceleration, sim->time_step);
        sim->bodies[current].velocity = Vector3Add(sim->bodies[current].velocity,target_delta_velocity);
        
        target_delta_position = Vector3Scale(sim->bodies[current].velocity,sim->time_step);
        sim->bodies[current].position = Vector3Add(sim->bodies[current].position, target_delta_position);

        //Clears the .applied_force field for the next updateSimulation
        sim->bodies[current].applied_force = (Vector3){ .x = 0.0F, .y = 0.0F, .z = 0.0F};    
             
    }

    
    sim->time_elapsed += sim->time_step;    

}

/**
 * @brief
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
    return;
}   