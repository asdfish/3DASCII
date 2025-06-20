#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "managers.hpp"

Coordinator coordinator;

EventBus eventBus;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    coordinator.Init();

    //Registration of components

    //Registration of systems

}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
}