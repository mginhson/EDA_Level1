/**
 * @brief Orbital simulation main module
 * @author Marc S. Ressl
 * @modifiers Matteo Ginhson, Nicanor Otamendi
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
    double timeMultiplier = 100 * SECONDS_PER_DAY; // Simulation speed: 100 days per simulation second
    double timeStep = timeMultiplier / fps;

    OrbitalSim *sim = constructOrbitalSim(timeStep);
    View *view = constructView(fps);

    while (isViewRendering(view))
    {
        for (int i = 0; i < 1; i++)
            updateOrbitalSimOptimized(sim);

        renderView(view, sim);
    }

    destroyView(view);
    destroyOrbitalSim(sim);

    return 0;
}
