#include <SDL.h>
#include <SDL_ttf.h>

#define FPS                 144

#define SCREEN_WIDTH        800
#define SCREEN_HEIGHT       800

#define GRAVITY             400
#define NORMAL_FORCE        400

#define PROJECTILE_VELOCITY 15
#define PROJECTILE_RADIUS   8
#define PROJECTILE_RATE     

#define PARTICLE_VELOCITY   6
#define PARTICLE_RADIUS     2

#ifndef _TEST_FIR_H_
#define _TEST_FIR_H_

struct App {
    SDL_Window* window;
	SDL_Surface* screenSurface;
	SDL_Surface* surfaceText;
	SDL_Texture* textureText;
    SDL_Color textColor;
    char textStr[64];
	TTF_Font* myFont;
	SDL_Renderer *renderer;
	SDL_Event event;
    SDL_Rect messageRect;
    int running;

    TTF_Font* (*initFont)();
    void (*initWindow)();
    void (*render)();
} app;

struct Circle {
    int x, y, radius;
    uint8_t r, g, b;
};

struct Level {
    SDL_Rect floor;    
} level;

#endif
