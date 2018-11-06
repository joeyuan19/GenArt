#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <random>
#include <SDL2/SDL.h>

#define PI 3.141592653589793238

using namespace std;

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 400;

bool init_sdl();
bool loadMedia();
bool close();
bool render();

SDL_Window * window = NULL;
SDL_Renderer * renderer = NULL;

class Point {
    public:
        Point ();
        Point (int,int,int);
        long double x;
        long double y;
        long double z;
};

Point::Point() {
    x = 0.0;
    y = 0.0;
    z = 0.0;
} 

Point::Point(int _x, int _y, int _z) {
    x = _x;
    y = _y;
    z = _z;
} 

int X(double x) {
    return SCREEN_WIDTH/2 + ((int)x);
}

int Y(double y) {
    return SCREEN_HEIGHT/2 - ((int)y);
}

void rotatePoint(Point * p, double angle) {
    long double c = cos(angle), s = sin(angle);
    long double x = p->x;
    long double y = p->y;
    p->x = c*x - s*y; 
    p->y = s*x + c*y; 
}

void translatePoint(Point * p, double dx, double dy) {
    p->x = max(min(p->x + dx,(long double)(SCREEN_WIDTH/2)),(long double)(-SCREEN_WIDTH/2));
    p->y = max(min(p->y + dy,(long double)(SCREEN_HEIGHT/2)),(long double)(-SCREEN_HEIGHT/2));
}
void translatePoint(Point * p, double dx, double dy, double long min_x, double long min_y, double long max_x, double long max_y) {
    p->x = max(min(p->x + dx,max_x),min_x);
    p->y = max(min(p->y + dy,max_y),min_y);
}

void drawLine(Point p1, Point p2) {
    SDL_RenderDrawLine(renderer,X(p1.x),Y(p1.y),X(p2.x),Y(p2.y));
}

class Square {
    public:
        Square ();
        Square (int,int,int);
        Point p1, p2, p3, p4;
        Uint8 r = 0x00, g = 0x00, b = 0x00, a = 0xFF;
        void rotate(double angle) {
            rotatePoint(&p1,angle);
            rotatePoint(&p2,angle);
            rotatePoint(&p3,angle);
            rotatePoint(&p4,angle);
        };
        void translate(double dx, double dy) {
            translatePoint(&p1,dx,dy);
            translatePoint(&p2,dx,dy);
            translatePoint(&p3,dx,dy);
            translatePoint(&p4,dx,dy);
        };
        void translate(double dx, double dy, int min_x, int min_y, int max_x, int max_y) {
            translatePoint(&p1,dx,dy,min_x,min_y,max_x,max_y);
            translatePoint(&p2,dx,dy,min_x,min_y,max_x,max_y);
            translatePoint(&p3,dx,dy,min_x,min_y,max_x,max_y);
            translatePoint(&p4,dx,dy,min_x,min_y,max_x,max_y);
        };
        bool is_inside(Point p) {
            return p.x >= p3.x && p.x <= p1.x && p.y >= p3.y && p.y <= p1.y;
        };
        void set_color(Uint8 _r, Uint8 _g, Uint8 _b, Uint8 _a) {
            r = _r;
            g = _g;
            b = _b;
            a = _a;
        };
        void draw() {
            SDL_SetRenderDrawColor(renderer,r,g,b,a);
            drawLine(p1,p2);
            drawLine(p2,p3);
            drawLine(p3,p4);
            drawLine(p4,p1);
        };
};

Square::Square() {
    p1.x =  100; 
    p1.y =  100;
    
    p2.x =  100;
    p2.y = -100;
    
    p3.x = -100;
    p3.y = -100;
    
    p4.x = -100;
    p4.y =  100;
}

Square::Square(int center_x, int center_y, int side) {
    p1.x = center_x + ((double)side)/2;
    p1.y = center_y + ((double)side)/2;
    
    p2.x = center_x + ((double)side)/2;
    p2.y = center_y - ((double)side)/2;
    
    p3.x = center_x - ((double)side)/2;
    p3.y = center_y - ((double)side)/2;
    
    p4.x = center_x - ((double)side)/2;
    p4.y = center_y + ((double)side)/2;
}

void ps(Square s) {
    cout << "(" << s.p4.x << "," << s.p4.y << ") (" << s.p1.x << "," << s.p1.y << ")" << endl;
    cout << "(" << s.p3.x << "," << s.p3.y << ") (" << s.p4.x << "," << s.p4.y << ")" << endl;
}

void pp(Point p) {
    cout << "(" << p.x  << "," << p.y << ")" << endl;
}

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

Square square;
Square square2(0,0,100);
double dx  =  1, dy  =  2;
double dx2 = -2, dy2 = -3;


bool init() {
    bool success = true;
    
    square2.set_color(0x00,0xCC,0xFF,0xFF);
    
    square.draw();
    square2.draw();
    return success;
} 

bool render() {
    bool success = true;
    
    SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0xFF);
    SDL_RenderClear(renderer);
    
    square.translate(dx,dy); 
    if ((int)(square.p1.x) == SCREEN_WIDTH/2 || (int)(square.p3.x) == -SCREEN_WIDTH/2) {
        dx = -dx;
    }
    if ((int)(square.p1.y) == SCREEN_HEIGHT/2 || (int)(square.p3.y) == -SCREEN_HEIGHT/2) {
        dy = -dy;
    }
    square.draw();
    
    square2.translate(dx2,dy2,square.p3.x,square.p3.y,square.p1.x,square.p1.x); 
    if ((int)(square2.p1.x) >= (int)(square.p1.x) || (int)(square2.p3.x) <= (int)(square.p3.x)) {
        cout << "Square.1.p1.x = " << square.p1.x << endl;
        cout << "Square.2.p1.x = " << square2.p1.x << endl;
        cout << "Square.1.p3.x = " << square.p3.x << endl;
        cout << "Square.2.p3.x = " << square2.p3.x << endl;
        cout << endl;
        dx2 = -dx2;
    }
    if ((int)(square2.p1.y) >= (int)(square.p1.y) || (int)(square2.p3.y) <= (int)(square.p3.y)) {
        cout << "Square.1.p1.y = " << square.p1.y << endl;
        cout << "Square.2.p1.y = " << square2.p1.y << endl;
        cout << "Square.1.p3.y = " << square.p3.y << endl;
        cout << "Square.2.p3.y = " << square2.p3.y << endl;
        cout << endl;
        dy2 = -dy2;
    }
    square2.draw();
    
    SDL_Delay(1000);
    SDL_RenderPresent(renderer);
    return success;
}

int main(int argc, char *argv[ ] ) {
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
