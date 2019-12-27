#include <SDL2/SDL.h>
#include <stdio.h>
#include "loading.hpp"

void checkButtons(SDL_Event *e, bool* quit, int *rot)
{
    while (SDL_PollEvent(e)){
        if (e->type == SDL_QUIT){
            *quit = true;
        }
        else if(e->type == SDL_KEYDOWN)
        {
            switch(e->key.keysym.sym)
            {
                case SDLK_LEFT:
                    *rot -= 20;
                    break;
                    
                case SDLK_RIGHT:
                    *rot += 20;
                    break;
                    
                case SDLK_q:
                    *quit = true;
                    break;
                    
                default:
                    break;
            }
        }
    }
}

void cleanUp(int **mesh, int **connect)
{
    int i;
    
    for (i = 0; i < 64; ++i)
        free(mesh[i]);
    free(mesh);
    
    for (i = 0; i < 112; ++i)
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

int main( int argc, char** args )
{
    using namespace std;
    SDL_Window* window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Event e;
    int **mesh, **connect;
    int nheights[64];
    int lines = 112, rot = 0;
    float inclination = 0.7;
    bool quit = false;
    
    if (argc != 2)
    {
        printf("Incorrect usage. Usage: terrain file\n");
        exit(1);
    }
    
    loadHeights(nheights, args[1]);
    
    mesh = (int **)malloc(sizeof(int *) * 64);
    loadMesh(mesh, nheights);
    
    connect = (int **)malloc(sizeof(int *) * lines);
    loadConnections(connect);
    
    printf("Now running SDL...\n");
    
    if(SDL_Init( SDL_INIT_VIDEO ) < 0)
    {
        printf("SDL could not initialize! %s\n", SDL_GetError());
        exit(1);
    }
    //Create window
    window = SDL_CreateWindow("Terrain", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(window == NULL)
    {
        printf("Window could not be created! %s\n", SDL_GetError());
        exit(1);
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    while (!quit)
    {
        checkButtons(&e, &quit, &rot);
        render(renderer, lines, mesh, connect, rot, inclination);
    }
    
    cleanUp(mesh, connect);
    
    SDL_DestroyWindow( window );
    
    SDL_Quit();
    
    printf("Exited normally\n");
    
    return 0;
}
