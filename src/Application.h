#ifndef APPLICATION_H
#define APPLICATION_H

#include "./Graphics.h"
#include "./Physics/Particle.h"
#include <vector>

struct MouseContext{
  Vec2 position;
  bool leftPressed = false;
  bool rightPressed = false;
};

class Application {
    private:
        bool running = false;
        std::vector<Particle*> particles;
        Vec2 pushForce = Vec2(0, 0);
        SDL_Rect liquidRect;
        SDL_Event event;
        MouseContext mouseCtx;

    public:
        Application() = default;
        ~Application() = default;
        bool IsRunning();
        void Setup();
        void Input();
        void Update();
        void Render();
        void Destroy();
};

#endif
