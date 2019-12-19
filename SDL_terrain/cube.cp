/*This source code copyrighted by Lazy Foo' Productions (2004-2019)
 and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdio.h>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main( int argc, char* args[] )
{
    //The window we'll be rendering to
    SDL_Window* window = NULL;
    SDL_Renderer *renderer = NULL;
    
    //The surface contained by the window
    SDL_Surface* screenSurface = NULL;
    
    int i = 0;
    float inclination = 0.7;
    
    printf("Now running...\n");
    
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }
    else
    {
        //Create window
        window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( window == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        }
        else
        {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            
            int square[4][2] = {{100, 100}, {200, 100},
                                {100, 200}, {200, 200}};
            
            int lines = 5;
            int connect[5][2] = {{0, 1}, {1, 3}, {2, 3}, {2, 0}};
            
            int cube[8][3] =
            {
                {300, 0, 0}, {400, 0, 0}, {300, 100, 0}, {400, 100, 0},
                {300, 0, 100}, {400, 0, 100}, {300, 100, 100}, {400, 100, 100}
            };
            
            int clines = 12;
            int cconnect[12][2] =
            {
                {0, 1}, {1, 3}, {2, 3}, {2, 0},
                {4, 5}, {5, 7}, {6, 7}, {6, 4},
                {0, 4}, {1, 5}, {2, 6}, {3, 7}
            };
            
            SDL_Event e;
            bool quit = false;
            while (!quit){
                while (SDL_PollEvent(&e)){
                    if (e.type == SDL_QUIT){
                        quit = true;
                    }
                    if (e.type == SDL_MOUSEBUTTONDOWN){
                        quit = true;
                    }
                }
                
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                SDL_RenderClear(renderer);
                SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0, 0xFF);
                for (int j = 0; j < lines; ++j)
                {
                    //printf("x: %d\n", square[connect[3][0] - 1][0]);
                    SDL_RenderDrawLine(renderer, square[connect[j][0]][0], square[connect[j][0]][1],
                                       square[connect[j][1]][0], square[connect[j][1]][1]);
                }
                
                for (int l = 0; l < clines; ++l)
                {
                    int x1 = inclination * cube[cconnect[l][0]][0] - inclination * cube[cconnect[l][0]][1];
                    int y1 = (1 - inclination) * cube[cconnect[l][0]][0] + (1 - inclination) * cube[cconnect[l][0]][1] - cube[cconnect[l][0]][2];
                    int x2 = inclination * cube[cconnect[l][1]][0] - inclination * cube[cconnect[l][1]][1];
                    int y2 = (1 - inclination) * cube[cconnect[l][1]][0] + (1 - inclination) * cube[cconnect[l][1]][1] - cube[cconnect[l][1]][2];
                    
                    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
                }
                // SDL_RenderDrawLine(renderer, 10, 10, 100 + i, 100 );
                SDL_RenderPresent(renderer);
                
                ++i;
            }
        }
    }
    
    //Destroy window
    SDL_DestroyWindow( window );
    
    //Quit SDL subsystems
    SDL_Quit();
    
    return 0;
}
