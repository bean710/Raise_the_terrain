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

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void loadHeights(int *nheights, char *name);
void loadMesh(int **mesh, int *nheights);
void loadConnections(int **connect);
void getCoords(int *coords, int **mesh, int **connect, int l, int rot, float inclination);

#endif /* loading_hpp */
