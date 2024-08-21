/**
 * @brief Implements an orbital simulation view
 * @author Marc S. Ressl
 * @modifiers Matteo Ginhson, Nicanor Otamendi
 * @copyright Copyright (c) 2022-2023
 */

#include <time.h>
#include <cstdio>
#include "View.h"


#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

/**
 * @brief Converts a timestamp (number of seconds since 1/1/2022)
 *        to an ISO date ("YYYY-MM-DD")
 *
 * @param timestamp the timestamp
 * @return The ISO date (a raylib string)
 */
const char *getISODate(float timestamp)
{
    // Timestamp epoch: 1/1/2022
    struct tm unichEpochTM = {0, 0, 0, 1, 0, 122};

    // Convert timestamp to UNIX timestamp (number of seconds since 1/1/1970)
    time_t unixEpoch = mktime(&unichEpochTM);
    time_t unixTimestamp = unixEpoch + (time_t)timestamp;

    // Returns ISO date
    struct tm *localTM = localtime(&unixTimestamp);
    return TextFormat("%04d-%02d-%02d",
                      1900 + localTM->tm_year, localTM->tm_mon + 1, localTM->tm_mday);
}

/**
 * @brief Constructs an orbital simulation view
 *
 * @param fps Frames per second for the view
 * @return The view
 */
View *constructView(int fps)
{
    View *view = new View();

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "EDA Orbital Simulation");
    SetTargetFPS(fps);
    DisableCursor();

    view->camera.position = {10.0f, 10.0f, 10.0f};
    view->camera.target = {0.0f, 0.0f, 0.0f};
    view->camera.up = {0.0f, 1.0f, 0.0f};
    view->camera.fovy = 45.0f;
    view->camera.projection = CAMERA_PERSPECTIVE;

    return view;
}

/**
 * @brief Destroys an orbital simulation view
 *
 * @param view The view
 */
void destroyView(View *view)
{
    CloseWindow();

    delete view;
}

/**
 * @brief Should the view still render?
 *
 * @return Should rendering continue?
 */
bool isViewRendering(View *view)
{
    return !WindowShouldClose();
}

/**
 * Renders an orbital simulation
 *
 * @param view The view
 * @param sim The orbital sim
 */
void renderView(View *view, OrbitalSim *sim)
{
    UpdateCamera(&view->camera, CAMERA_FREE);
    int i;
    BeginDrawing();

    ClearBackground(BLACK);
    BeginMode3D(view->camera);

    

    Vector3 temp;
    double dist;

    for (i=0; i < sim->planets_range  ; i++)
    {
        DrawSphere (Vector3Scale(sim->bodies[i].position, 1E-11) ,
                    0.015 * logf(sim->bodies[i].radius), 
                    sim->bodies[i].color);
    }

    for (i=sim->planets_range; i < sim->bodies_count  ; i++)
    {
        dist = Vector3Distance(view->camera.position,Vector3Scale(sim->bodies[i].position,1E-11));
         
        if(dist < 5.0)
            DrawSphere (Vector3Scale(sim->bodies[i].position, 1E-11) ,0.015 * logf(sim->bodies[i].radius), sim->bodies[i].color);
        else
            DrawPoint3D (Vector3Scale(sim->bodies[i].position, 1E-11) , sim->bodies[i].color);

    }


    
    EndMode3D();



    //Shows FPS and Date on ISO format on the upper left corner.
    
    DrawFPS (0,0);
    DrawText (getISODate(sim->time_elapsed),0,40,22,WHITE);

    EndDrawing();

}