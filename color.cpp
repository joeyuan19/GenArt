#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <random>
#include <SDL2/SDL.h>

#define PI 3.141592653589793238

using namespace std;

const int FRAME_RATE = 100;
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;
const int SQUARE_SIDE = 5;


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
const int C = 3;
int space [W*H*C];

int bound(int n, int min, int max) {
    if (n < 0) {
        return max+n;
    } else {
        return n%max; 
    }
}

int abs(int n) {
    if (n < 0) {
        return -n;
    } else {
        return n;
    }
}

int bc(int n, int lower_n, int upper_n) {
    if (n < lower_n) {
        n = upper_n+n%upper_n;
    } else if (n >= upper_n) {
        n = n%upper_n;
    }
    return n;
}

int get(int w, int h, int c) {
    w = bc(w,0,W);
    h = bc(h,0,H);
    c = bc(c,0,C);
    return space[w*H*C+h*C+c];
}

int set(int w, int h, int c, int val) {
    return space[w*H*C+h*C+c] = val;
}

void renderFillRect(SDL_Renderer * renderer, int w, int h) {
    const SDL_Rect rect = {w*SQUARE_SIDE, h*SQUARE_SIDE, SQUARE_SIDE, SQUARE_SIDE}; 
    SDL_RenderFillRect(renderer, &rect);
}

random_device rd;
default_random_engine gen(rd());
int randmax = 10000;
uniform_int_distribution<int> dist(0,randmax);

int R(int min, int max) {
    return min + dist(gen)%max;
}

int divide(int n, int d) {
    float num = n;
    float den = d;
    int r = num/den;
    return r; 
}

void color() {
    SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0xFF);
    SDL_RenderClear(renderer);
    int w, h, c;
    for (w = 0; w < W; w++) {
        for (h = 0; h < H; h++) {
            SDL_SetRenderDrawColor(renderer,get(w,h,0)%256,get(w,h,1)%256,get(w,h,2)%256,0xFF);
            renderFillRect(renderer,w,h);
        }
    }
}

int gauss(double x, double y) {
    return 255*exp(-(x*x +y*y) );
}

int f(double x, double y) {
    return 150120*abs(sin(x));
}

int f1(double x, double y) {
    return 1000000*abs(cos(x));
}

int f2(double x, double y) {
    return 10000*exp(-(x*x) );
}

bool init() {
    bool success = true;
    int w, h, c, r;
    double x, y;
    for (w = 0; w < W; w++) {
        for (h = 0; h < H; h++) {
            x = (3.15*w)/W;
            y = (3.15*h)/H;
            set(w,h,0,f(x,y));
            set(w,h,1,f1(x,y));
            set(w,h,2,f2(x,y));
        }
    }
    /*
    for (w = 0; w < W; w++) {
        for (h = 0; h < H; h++) {
            x = 2.*w/W - 1.;
            y = 2.*w/W - 1.;
            // Random initial
            for (c = 0; c < C; c++) {
                set(w,h,c,R(0,255*100000));
            }
        }
    }
    */
    color();
    return success;
} 

bool render() {
    bool success = true;
    int w, h, dw, dh;
    int r, g, b;
    for (w = 0; w < W; w++) {
        for (h = 0; h < H; h++) {
            r = 0;
            g = 0;
            b = 0;
            for (dw = -1; dw <= 1; dw++) {
                for (dh = -1; dh <= 1; dh++) {
                    if (w+dw >= 0 && w+dw < W) {
                    if (h+dh >= 0 && h+dh < H) {
                        r += get(w+dw,h+dh,0);
                        g += get(w+dw,h+dh,1);
                        b += get(w+dw,h+dh,2);
                    }}
                }
            }
            set(w,h,0,divide(r,9));
            set(w,h,1,divide(g,9));
            set(w,h,2,divide(b,9));
        }
    }

    color();
    
    SDL_Delay(FRAME_RATE);

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
