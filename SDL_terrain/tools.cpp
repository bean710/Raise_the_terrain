//
//  tools.cpp
//  SDL_terrain
//
//  Created by Ben on 12/27/19.
//  Copyright © 2019 Ben. All rights reserved.
//

#include <stdio.h>
#include <SDL2/SDL.h>
#include "loading.hpp"

void checkButtons(SDL_Event *e, bool* quit, int *rot)
{
    while (SDL_PollEvent(e)){
        if (e->type == SDL_QUIT){
            *quit = true;
        }
        else if (e->type == SDL_KEYDOWN)
        {
            switch (e->key.keysym.sym)
            {
                case SDLK_LEFT:
                    *rot = -20;
                    break;
                    
                case SDLK_RIGHT:
                    *rot = 20;
                    break;
                    
                case SDLK_q:
                    *quit = true;
                    break;
                    
                default:
                    break;
            }
        }
        else if (e->type == SDL_KEYUP)
        {
            switch (e->key.keysym.sym)
            {
                case SDLK_LEFT:
                case SDLK_RIGHT:
                    *rot = 0;
                    break;
                
                default:
                    break;
            }
        }
    }
}

void cleanUp(int **mesh, int **connect, int total, int lines)
{
    int i;
    
    for (i = 0; i < total; ++i)
        free(mesh[i]);
    free(mesh);
    
    for (i = 0; i < lines; ++i)
        free(connect[i]);
    free(connect);
}

void render(SDL_Renderer *renderer, int lines, int **mesh, int **connect, int rot, float inclination)
{
    int i, coords[4];
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0x0, 0xFF, 0x0, 0xFF);
    
    for (i = 0; i < lines; ++i)
    {
        getCoords(coords, mesh, connect, i, rot, inclination);
        
        SDL_RenderDrawLine(renderer, coords[0], coords[1], coords[2], coords[3]);
    }
    
    SDL_RenderPresent(renderer);
}

