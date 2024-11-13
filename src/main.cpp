#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "logger.h"


int main(int argc, char *argv[])
{
    logger.debug("PROGRAMM START");

    bool QUIT = false;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::string ErrorMessage{"Couldn't initialize sdl video subsystem: "};
        logger.error(100, ErrorMessage.append(SDL_GetError()));
        QUIT = true;
    }

    if (TTF_Init() < 0)
    {
        std::string ErrorMessage{"Couldn't initialize sdl ttf subsystem: "};
        logger.error(100, ErrorMessage.append(SDL_GetError()));
        QUIT = true;
    }

    SDL_Window *window = SDL_CreateWindow("Template", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    while (!QUIT)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {

        }

        SDL_RenderClear(renderer);

        SDL_RenderPresent(renderer);
    }


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();

    logger.debug("PROGRAMM END");

    return 0;
}