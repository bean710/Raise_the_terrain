#include <SDL2/SDL.h>
#include <stdio.h>
#include "loading.hpp"

int main(int argc, char** args)
{
    using namespace std;
    SDL_Window* window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Event e;
    int **mesh, **connect;
    int *nheights;
    int width, height;
    int lines, rot = 0;
    float inclination = 0.7;
    bool quit = false;
    
    if (argc != 2)
    {
        printf("Incorrect number of arguments. Usage: terrain file\n");
        exit(1);
    }
    
    nheights = newLoad(args[1], &width, &height);
    lines = width * height * 2 - width - height;
    
    mesh = (int **)malloc(sizeof(int *) * width * height);
    loadMesh(mesh, nheights, width, height);
    
    connect = (int **)malloc(sizeof(int *) * lines);
    loadConnections(connect, width, height);
    
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
    
    cleanUp(mesh, connect, width * height, lines);
    
    SDL_DestroyWindow(window);
    
    SDL_Quit();
    
    printf("Exited normally\n");
    
    return 0;
}
