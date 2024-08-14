/**
 * @brief Orbital simulation main module
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

#include <stdio.h>
#include <stdlib.h>
#include "OrbitalSim.h"
#include "View.h"

#define SECONDS_PER_DAY 86400

int main(int argc, char **argv)
{
    int fps = 60;                                 // Frames per second
    float timeMultiplier = 100 * SECONDS_PER_DAY; // Simulation speed: 100 days per simulation second
    float timeStep = timeMultiplier / fps;

    OrbitalSim *sim = constructOrbitalSim(timeStep);
    View *view = constructView(fps);

    while (isViewRendering(view))
    {
        //updateOrbitalSim(sim);

        renderView(view, NULL);
    }

    destroyView(view);
    destroyOrbitalSim(sim);

    return 0;
}
