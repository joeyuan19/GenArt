#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <random>
#include <SDL2/SDL.h>

#define PI 3.141592653589793238

using namespace std;

struct point {
    double x;
    double y;
    double z;
};

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 500;
const int SQUARE_SIDE = 100;

bool init();
bool loadMedia();
bool close();

SDL_Window * window = NULL;
SDL_Renderer * renderer = NULL;

bool init_sdl() {
    bool success = true;

    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    } else {
        if ( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1") ) {
            printf("Warning: Linear texture filtering not enabled!");
        }

        window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if ( window == NULL ) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        } else {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0xFF);
            SDL_RenderClear(renderer);
        }
    }
    return success;
}

bool loadMedia() {
    bool success = true;
    return success;
}

bool close() {
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();

    return true;
}

const int W = SCREEN_WIDTH/SQUARE_SIDE;
const int H = SCREEN_HEIGHT/SQUARE_SIDE;
int space [W*H];

int boundary(int n, int min, int max) {
    return n%max;
}

int get(int w, int h) {
    return space[bound(w)*H+bound(h)];
}

int set(int w, int h, int val) {
    return space[w*H+h] = val;
}

void renderFillRect(SDL_Renderer * renderer, int w, int h) {
    const SDL_Rect rect = {w*SQUARE_SIDE, h*SQUARE_SIDE, SQUARE_SIDE, SQUARE_SIDE}; 
    SDL_RenderFillRect(renderer, &rect);
}

random_device rd;
default_random_engine gen(rd());
int randmax = 100000;
uniform_int_distribution<int> dist(0,randmax);

int r(int min, int max) {
    return min + dist(gen)%max;
}

void color() {
    SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0xFF);
    SDL_RenderClear(renderer);
    int w, h;
    for (w = 0; w < W; w++) {
        for (h = 0; h < H; h++) {
            if (get(w,h) == 1) {
                SDL_SetRenderDrawColor(renderer,0x14,0x40,0xAA,0xFF);
            } else {
                SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0xFF);
            }
            renderFillRect(renderer,w,h);
        }
    }
}

bool init() {
    bool success = true;
    int w, h;
    for (w = 0; w < W; w++) {
        for (h = 0; h < H; h++) {
            if (r(0,2) == 0) {
                set(w,h,1);
            } else {
                set(w,h,-1);
            }
        }
    }
    color();
    return success;
} 

bool render() {
    bool success = true;
    int w, h;
    float J = 1.0;
    float kb = 1.0, T = 1/100000.0;
    float beta = 1/(kb*T);
    float deltaE, E = 0.0, E1, E2;
    for (w = 0; w < W; w++) {
        for (h = 0; h < H; h++) {
            E = 0.0;
             
            E = get((w+1)%W,h);
            E += get((w-1)%W,h);
            E += get(w,(h+1)%H);
            E += get(w,(h-1)%H);
            
            E1 = E*get(w,h);
            E2 = E*(-get(w,h));

            deltaE = -J*(E2-E1);
            
            if (0 < deltaE || ((double) r(0,randmax)) / ((double) randmax) > exp(-beta*deltaE)) {
                set(w,h,-get(w,h));
            }
        }
    }

    color();
    
    SDL_Delay(1000);

    SDL_RenderPresent(renderer);
    return success;
}

int main(int argc, char *argv[ ] )
{
    if ( !init_sdl() || !init() ) {
        printf("Failed to initialize!\n");
    } else {
        if ( !loadMedia() ) {
            printf("Failed to load media!\n");
        } else {
            bool quit = false;
            SDL_Event e;

            while ( !quit ) {
                while ( SDL_PollEvent( &e ) != 0 ) {
                    if ( e.type == SDL_QUIT ) {
                        quit = true;
                    }
                }
                if (!render()){
                    quit = true;    
                }
            }
        }
    }
    
    close();

    return 0;
}
