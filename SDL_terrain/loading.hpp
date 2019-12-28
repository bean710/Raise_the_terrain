//
//  loading.hpp
//  SDL_terrain
//
//  Created by Ben on 12/18/19.
//  Copyright © 2019 Ben. All rights reserved.
//

#ifndef loading_hpp
#define loading_hpp

#include <stdio.h>
#include <SDL2/SDL.h>

struct height_t {
    int val;
    height_t *next;
};

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void loadHeights(int *nheights, char *name);
int *newLoad(char *name, int *fwidth, int *fheight);
void loadMesh(int **mesh, int *nheights, int width, int height);
void loadConnections(int **connect, int width, int height);
void getCoords(int *coords, int **mesh, int **connect, int l, int rot, float inclination);

void checkButtons(SDL_Event *e, bool* quit, int *rot);
void cleanUp(int **mesh, int **connect, int total, int lines);
void render(SDL_Renderer *renderer, int lines, int **mesh, int **connect, int rot, float inclination);


#endif /* loading_hpp */
