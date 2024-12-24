#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    running = Graphics::OpenWindow();
    liquidRect.x = 0;
    liquidRect.y = Graphics::windowHeight - Graphics::windowHeight / 3; //1/3 of height starting from bottom
    liquidRect.w = Graphics::Width();
    liquidRect.h = Graphics::Height();

    mouseCtx.position = Vec2(0, 0);
}

///////////////////////////////////////////////////////////////////////////////
// Input processing
///////////////////////////////////////////////////////////////////////////////
void Application::Input() {
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                  mouseCtx.leftPressed = true;

                  // generate random mass between 1 and 8
                  int mass = 1 + std::rand() / (RAND_MAX / (8 - 1));
                  Particle* p = Particle::Create(event.button.x, event.button.y, mass);
                  particles.push_back(p);
                }
                if (event.button.button == SDL_BUTTON_RIGHT){
                  mouseCtx.rightPressed = true;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if(event.button.button == SDL_BUTTON_LEFT){
                  mouseCtx.leftPressed = false;
                }
                if (event.button.button == SDL_BUTTON_RIGHT){
                  mouseCtx.rightPressed = true;
                }
                break;

            case SDL_MOUSEMOTION:
              mouseCtx.position = Vec2(event.motion.x, event.motion.y);
              break;

            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_ESCAPE)
                  running = false;
                if(event.key.keysym.sym == SDLK_w){
                  pushForce.y = -50 * PIXELS_PER_METER;
                }
                if(event.key.keysym.sym == SDLK_s){
                  pushForce.y = 50 * PIXELS_PER_METER;
                }
                if(event.key.keysym.sym == SDLK_a){
                  pushForce.x = -50 * PIXELS_PER_METER;
                }
                if(event.key.keysym.sym == SDLK_d){
                  pushForce.x = 50 * PIXELS_PER_METER;
                }
                break;

            case SDL_KEYUP:
               if(event.key.keysym.sym == SDLK_w){
                  pushForce.y = 0;
                }
                if(event.key.keysym.sym == SDLK_s){
                  pushForce.y = 0;
                }
                if(event.key.keysym.sym == SDLK_a){
                  pushForce.x = 0;
                }
                if(event.key.keysym.sym == SDLK_d){
                  pushForce.x = 0;
                }
               break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update() {
    // Wait some time until the reach the target frame time in milliseconds
    static int timePreviousFrame;
    const int millisecondsPerFrame = 1000 / Graphics::displayMode.refresh_rate;
    int timeToWait = millisecondsPerFrame - (SDL_GetTicks() - timePreviousFrame);
    if (timeToWait > 0)
        SDL_Delay(timeToWait);

    // Calculate the deltatime in seconds
    float deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;
    if (deltaTime > 0.016)
        deltaTime = 0.016;

    // Set the time of the current frame to be used in the next one
    timePreviousFrame = SDL_GetTicks();


    for (auto particle: particles) {
        if(particle->position.y >= liquidRect.y){
            Vec2 dragF = Force::GenerateDragForce(*particle, 0.1);
            particle->AddForce(dragF);
        }

        Vec2 weight = Vec2(0.0, particle->mass * 9.8 * PIXELS_PER_METER);
        particle->AddForce(weight);
        // Integrate the acceleration and velocity to estimate the new position
        particle->Integrate(deltaTime);
    }

    for (auto particle: particles) {
        // Nasty hardcoded flip in velocity if it touches the limits of the screen window
        if (particle->position.x - particle->radius <= 0) {
            particle->position.x = particle->radius;
            particle->velocity.x *= -0.9;
        } else if (particle->position.x + particle->radius >= Graphics::Width()) {
            particle->position.x = Graphics::Width() - particle->radius;
            particle->velocity.x *= -0.9;
        }
        if (particle->position.y - particle->radius <= 0) {
            particle->position.y = particle->radius;
            particle->velocity.y *= -0.9;
        } else if (particle->position.y + particle->radius >= Graphics::Height()) {
            particle->position.y = Graphics::Height() - particle->radius;
            particle->velocity.y *= -0.9;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
    Graphics::ClearScreen(0xFF056263);

    //Draw the liquid area
    Graphics::DrawFillRect(liquidRect.x + Graphics::windowWidth / 2, liquidRect.y + Graphics::windowHeight / 2, liquidRect.w, liquidRect.h, 0xFF6E3713); //blue
    for (auto particle: particles) {
        Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->radius, 0xFFFFFFFF);
    }

    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    for (auto particle: particles) {
        delete particle;
    }
    Graphics::CloseWindow();
}
