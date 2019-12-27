//
//  loading.cpp
//  SDL_terrain
//
//  Created by Ben on 12/18/19.
//  Copyright © 2019 Ben. All rights reserved.
//

#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include "loading.hpp"

void freeHL(height_t *head)
{
    for (; head; head = head->next)
        free(head);
}

int *newLoad(char *name, int *fwidth, int *fheight)
{
    using namespace std;
    ifstream infile;
    int numHeights = 0, width = 0, height = 0, prevWidth = 0;
    string line;
    char *cline, *token;
    height_t *head = (height_t *)malloc(sizeof(height_t)), *tmp = head;
    int *ret, it;
    
    if (!head)
    {
        printf("Could not malloc for heights!");
        exit(1);
    }
    
    infile.open(name, ios::in);
    
    if (!infile)
    {
        printf("%s is not a valid input file!\n", name);
        freeHL(head);
        exit(1);
    }
    
    printf("Loading heights from file '%s'... ", name);
    
    while (getline(infile, line))
    {
        ++height;
        width = 0;
        cline = new char[line.length() + 1];
        strcpy(cline, line.c_str());
        token = strtok(cline, " ");
        while (token)
        {
            ++width;
            if (tmp != head)
            {
                tmp->next = (height_t *)malloc(sizeof(height_t));
                if (!tmp->next)
                {
                    printf("Could not malloc space for heights!");
                    freeHL(head);
                    exit(1);
                }
                tmp = tmp->next;
            }
            tmp->val = atoi(token);
            ++numHeights;
            token = strtok(cline, " ");
        }
        delete [] cline;
        
        if (height == 1)
            prevWidth = width;
        else if (width != prevWidth)
        {
            printf("Mesh is not a rectangle!");
            freeHL(head);
            exit(1);
        }
    }
    tmp->next = NULL;
    if (height == 0)
    {
        printf("No data in file!");
        freeHL(head);
        exit(1);
    }
    
    ret = (int *)malloc(sizeof(int) * width * height);
    if (!ret)
    {
        printf("Could not malloc for heights!");
        freeHL(head);
        exit(1);
    }
    
    for (tmp = head, it = 0; tmp; tmp = tmp->next, ++it)
    {
        ret[it] = tmp->val;
    }
    
    freeHL(head);
    
    *fwidth = width;
    *fheight = height;
    return (ret);
}

void loadHeights(int *nheights, char *name)
{
    using namespace std;
    ifstream infile;
    
    infile.open(name, ios::in);
    
    if (!infile)
    {
        printf("%s is not a valid input file!\n", name);
        exit(1);
    }
    
    printf("Loading heights from file '%s'... ", name);
    int hcount = 0;
    while (infile && hcount < 64)
    {
        string strIn;
        infile >> strIn;
        nheights[hcount] = atoi(strIn.c_str());
        ++hcount;
    }
    printf("Done!\n");
}

void loadMesh(int **mesh, int *nheights)
{
    printf("Creating mesh points... ");

    if (mesh == NULL)
    {
        puts("Malloc failed");
        exit(1);
    }
    
    for (int i = 0; i < 64; ++i)
    {
        mesh[i] = (int *)malloc(sizeof(int) * 3);
        
        if (mesh[i] == NULL)
        {
            puts("Malloc failed");
            exit(1);
        }
    }
    
    for (int a = 0; a < 8; ++a)
    {
        for (int b = 0; b < 8; ++b)
        {
            mesh[a * 8 + b][0] = 50 * a - 175;
            mesh[a * 8 + b][1] = 50 * b - 175;
            mesh[a * 8 + b][2] = nheights[b * 8 + a] / 2 - 150;
        }
    }
    printf("Done!\n");
}

void loadConnections(int **connect)
{
    printf("Generating line coordinates... ");
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
    printf("Done!\n");
}

void getCoords(int *coords, int **mesh, int **connect, int l, int rot, float inclination)
{
    float x1, y1, z1, x2, y2, z2, rx1, ry1, rx2, ry2;
    
    x1 = mesh[connect[l][0]][0];
    y1 = mesh[connect[l][0]][1];
    z1 = mesh[connect[l][0]][2];
    x2 = mesh[connect[l][1]][0];
    y2 = mesh[connect[l][1]][1];
    z2 = mesh[connect[l][1]][2];
    
    float a = rot/10.0 * M_PI / 180;
    float sa = sin(a), ca = cos(a);
    rx1 = x1 * ca - y1 * sa;
    ry1 = x1 * sa + y1 * ca;
    rx2 = x2 * ca - y2 * sa;
    ry2 = x2 * sa + y2 * ca;
    
    coords[0] = inclination * rx1 - inclination * ry1 + SCREEN_WIDTH / 2;
    coords[1] = (1 - inclination) * rx1 + (1 - inclination) * ry1 - z1 + SCREEN_HEIGHT / 5;
    coords[2] = inclination * rx2 - inclination * ry2 + SCREEN_WIDTH / 2;
    coords[3] = (1 - inclination) * rx2 + (1 - inclination) * ry2 - z2 + SCREEN_HEIGHT / 5;
}
