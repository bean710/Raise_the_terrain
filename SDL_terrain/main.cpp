#include <SDL2/SDL.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main( int argc, char** args )
{
    using namespace std;
    SDL_Window* window = NULL;
    SDL_Renderer *renderer = NULL;
    
    int i = 0;
    float inclination = 0.7;
    
    if (argc != 2)
    {
        printf("Incorrect usage. Usage: terrain file\n");
        exit(1);
    }
    
    ifstream infile;
    infile.open(args[1], ios::in);
    
    int nheights[64];
    
    if (!infile)
    {
        printf("That's not a valid input file!\n");
        exit(1);
    }
    
    int hcount = 0;
    while (infile && hcount < 64)
    {
        string strIn;
        infile >> strIn;
        nheights[hcount] = atoi(strIn.c_str());
        ++hcount;
        cout << strIn << endl;
    }
    
    printf("Now running...\n");
    
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! %s\n", SDL_GetError() );
    }
    else
    {
        //Create window
        window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( window == NULL )
        {
            printf( "Window could not be created! %s\n", SDL_GetError() );
        }
        else
        {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            
            int **mesh = (int **)malloc(sizeof(int *) * 64);
            for (int i = 0; i < 64; ++i)
                mesh[i] = (int *)malloc(sizeof(int) * 3);
            
            if (mesh == NULL)
            {
                puts("Malloc failed");
                exit(1);
            }
            
            for (int a = 0; a < 8; ++a)
            {
                for (int b = 0; b < 8; ++b)
                {
                    mesh[a * 8 + b][0] = 50 * a - 175;
                    mesh[a * 8 + b][1] = 50 * b - 175;
                    printf("Height: %d\n", nheights[b * 8 + a] / 2 - 150);
                    mesh[a * 8 + b][2] = nheights[b * 8 + a] / 2 - 150;
                }
            }
            
            int lines = 112;
            int **connect = (int **)malloc(sizeof(int *) * lines);
            for (int i = 0; i < 112; ++i)
                connect[i] = (int *)malloc(sizeof(int) * 2);
            
            for (int a = 0, it = 0; a < 8; ++a)
            {
                for (int b = 0; b < 8; ++b)
                {
                    if (b - 1 >= 0)
                    {
                        connect[it][0] = (a * 8) + b - 1;
                        connect[it][1] = (a * 8) + b;
                        ++it;
                    }
                    if (a - 1 >= 0)
                    {
                        connect[it][0] = ((a - 1) * 8) + b;
                        connect[it][1] = (a * 8) + b;
                        ++it;
                    }
                }
            }
            
            SDL_Event e;
            bool quit = false;
            float x1, y1, z1, x2, y2, z2, rx1, ry1, rx2, ry2, ox1, oy1, ox2, oy2;
            int rot = 0;
            while (!quit){
                while (SDL_PollEvent(&e)){
                    if (e.type == SDL_QUIT){
                        quit = true;
                    }
                    else if( e.type == SDL_KEYDOWN )
                    {
                        switch( e.key.keysym.sym )
                        {
                            case SDLK_LEFT:
                                rot -= 10;
                                break;
                                
                            case SDLK_RIGHT:
                                rot += 10;
                                break;
                                
                            case SDLK_q:
                                quit = true;
                                break;
                                
                            default:
                                break;
                        }
                    }
                }
                
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                SDL_RenderClear(renderer);
                SDL_SetRenderDrawColor(renderer, i % 255, (i + 85) % 255, (i + 170) % 255, 0xFF);
                
                for (int l = 0; l < lines; ++l)
                {
                    x1 = mesh[connect[l][0]][0];
                    y1 = mesh[connect[l][0]][1];
                    z1 = mesh[connect[l][0]][2];
                    x2 = mesh[connect[l][1]][0];
                    y2 = mesh[connect[l][1]][1];
                    z2 = mesh[connect[l][1]][2];
                    
                    float a = rot/10.0 * M_PI / 180;
                    float sa = sin(a);
                    float ca = cos(a);
                    rx1 = x1 * ca - y1 * sa;
                    ry1 = x1 * sa + y1 * ca;
                    rx2 = x2 * ca - y2 * sa;
                    ry2 = x2 * sa + y2 * ca;
                    
                    ox1 = inclination * rx1 - inclination * ry1 + SCREEN_WIDTH / 2;
                    oy1 = (1 - inclination) * rx1 + (1 - inclination) * ry1 - z1 + SCREEN_HEIGHT / 5;
                    ox2 = inclination * rx2 - inclination * ry2 + SCREEN_WIDTH / 2;
                    oy2 = (1 - inclination) * rx2 + (1 - inclination) * ry2 - z2 + SCREEN_HEIGHT / 5;
                   
                    SDL_RenderDrawLine(renderer, ox1, oy1, ox2, oy2);
                }
                // SDL_RenderDrawLine(renderer, 10, 10, 100 + i, 100 );
                SDL_RenderPresent(renderer);
                
                ++i;
            }
            
            for (int i = 0; i < 64; ++i)
                free(mesh[i]);
            free(mesh);
            
            for (int i = 0; i < 112; ++i)
                free(connect[i]);
            free(connect);
        }
    }
    
    SDL_DestroyWindow( window );
    
    SDL_Quit();
    
    return 0;
}
