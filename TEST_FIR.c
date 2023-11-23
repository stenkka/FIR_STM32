#include "TEST_FIR.h"
#include "FIR.h"
#include "linked_list.h"

#include <SDL.h>
#include <SDL_ttf.h>

#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory.h>
#include <math.h>

Uint32 t_end, t_start, startTick;

Node_t *nodeListTmp, *nodeListHead = NULL;

uint8_t dataPointColorR = 255;
uint8_t dataPointColorG = 0;
uint8_t dataPointColorB = 0;

int colorStep = 2;

void initWindow()
{
    	if(SDL_Init(SDL_INIT_VIDEO) < 0)
        {
		printf("SDL could not be initialized: %s\n", SDL_GetError());
        }
        else
        {
            printf("SDL video system is ready to go\n");
        }
        app.running = 1;
		app.window = SDL_CreateWindow( "APP", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		app.renderer =  SDL_CreateRenderer(app.window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED );
		app.screenSurface = SDL_GetWindowSurface(app.window);
}

TTF_Font* initFont()
{
	if(TTF_Init() == -1)
    {
			printf("Could not initailize SDL2_ttf, error: %s\n", TTF_GetError());
	}
	else
    {
		printf("SDL2_ttf system ready to go!\n");
	}

	// Load our font file and set the font size
	TTF_Font* myFont = TTF_OpenFont("./fonts/Orbitron-Medium.ttf", 128);
	// Confirm that it was loaded
	if (myFont == NULL)
    {
		printf("Could not load font\n");
		return NULL;
	}
	return myFont;
}

void renderCircles()
{
    SDL_SetRenderDrawColor(app.renderer,0,0,255,0);
    nodeListTmp = nodeListHead;
    while (nodeListTmp != NULL)
    {
        SDL_SetRenderDrawColor(app.renderer, nodeListTmp->circle.r, nodeListTmp->circle.g, nodeListTmp->circle.b, 100);

        const int32_t diameter = nodeListTmp->circle.radius * 2;

        int32_t x = (nodeListTmp->circle.radius - 1);
        int32_t y = 0;
        int32_t tx = 1;
        int32_t ty = 1;
        int32_t error = (tx - diameter);

        int a;
        while (x >= y)
        {
            //  Each of the following renders an octant of the circle
            SDL_RenderDrawPoint(app.renderer, nodeListTmp->circle.x + x, nodeListTmp->circle.y - y);
            SDL_RenderDrawPoint(app.renderer, nodeListTmp->circle.x + x, nodeListTmp->circle.y + y);
            SDL_RenderDrawPoint(app.renderer, nodeListTmp->circle.x - x, nodeListTmp->circle.y - y);
            SDL_RenderDrawPoint(app.renderer, nodeListTmp->circle.x - x, nodeListTmp->circle.y + y);
            SDL_RenderDrawPoint(app.renderer, nodeListTmp->circle.x + y, nodeListTmp->circle.y - x);
            SDL_RenderDrawPoint(app.renderer, nodeListTmp->circle.x + y, nodeListTmp->circle.y + x);
            SDL_RenderDrawPoint(app.renderer, nodeListTmp->circle.x - y, nodeListTmp->circle.y - x);
            SDL_RenderDrawPoint(app.renderer, nodeListTmp->circle.x - y, nodeListTmp->circle.y + x);

            if (error <= 0)
            {
                ++y;
                error += ty;
                ty += 2;
            }

            if (error > 0)
            {
                --x;
                tx += 2;
                error += (tx - diameter);
            }
        }
        nodeListTmp = nodeListTmp->next;
    }
}

void render()
{
	SDL_SetRenderDrawColor(app.renderer,180,180,180,0);
	SDL_RenderClear(app.renderer);
	
	SDL_SetRenderDrawColor(app.renderer, 255, 0, 0, 100);
    //SDL_RenderDrawRect(app.renderer, &level.floor);

    renderCircles();

    

    TTF_RenderText_Solid(app.myFont, "Test", app.textColor);

    SDL_RenderCopy(app.renderer,app.textureText,NULL,&app.messageRect);
	
    //SDL_RenderTexture(app.renderer, app.textureText, NULL, &app.messageRect);

	SDL_RenderPresent(app.renderer);
}

void spawnDataPoint(int x, int y, int radius, int* color, char* label)
{
    prependNode(&nodeListHead, createNode(nodeListHead));
    nodeListHead->circle.x = x;
    nodeListHead->circle.y = y;
    nodeListHead->circle.radius = radius;
    nodeListHead->circle.r = color[0];
    nodeListHead->circle.g = color[1];
    nodeListHead->circle.b = color[2];
    sprintf(nodeListHead->label, "%s", label);
}

const char* getField(char* line, int num)
{
    const char* tok;
    for (tok = strtok(line, ",");
            tok && *tok;
            tok = strtok(NULL, ",\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}

int main(int argc, char* argv[])
{
    printf("argc = %d\n", argc);

    FILE* stream = fopen(argv[1], "r");

	app.initWindow = initWindow;
    app.initFont = initFont;
    app.render = render;

    level.floor.x = 0;
    level.floor.y = 800;
    level.floor.w = 800;
    level.floor.h = 1;

    app.initWindow();

	app.myFont = app.initFont();
    
    app.textColor.r = 0;
    app.textColor.g = 255;
    app.textColor.b = 0;

    app.messageRect.x = 500;
    app.messageRect.y = 10;
    app.messageRect.w = 300;
    app.messageRect.h = 25;

	SDL_Color owncolor = {255,255,255};

	const Uint8 *kbmState = SDL_GetKeyboardState(NULL);
	
    uint8_t dataPointsRendered = 0;

    char line[1024];

    FIR_Filter_t* lowPassFIR = (FIR_Filter_t*)malloc(sizeof(FIR_Filter_t));
    initFIR_Filter(lowPassFIR, 16);
	
    while (app.running)
    {
		while(SDL_PollEvent(&app.event))
        {
	    	switch(app.event.type)
            {
			case SDL_QUIT:
				// shut down
				app.running = 0;
				break;
			}       
		}

        startTick = SDL_GetTicks();

        if (kbmState[SDL_SCANCODE_R])
        {
            freeNodes(&nodeListHead);
        }
		if (kbmState[SDL_SCANCODE_ESCAPE])
        {
			app.running = 0;
		}

        if (!dataPointsRendered)
        {
            int x = 0;
            int red[3] = {255, 0, 0};
            int blue[3] = {0, 0, 255};
            while (fgets(line, 1024, stream))
            {
                char* tmp = strdup(line);
                const char* inputStr = getField(tmp, atoi(argv[2]));
                if (!inputStr) { break; }
                double input = 10 * strtod(inputStr, NULL);
                // plot FIR input
                spawnDataPoint(x, 200 + input, 1, red, "Input");
                // Do filtering and plot FIR output
                updateFIR_Filter(lowPassFIR, input);
                spawnDataPoint(x, 600 + lowPassFIR->output, 1, blue, "Output");
                free(tmp);
                x++;
                SDL_Delay(1);
                // render here for drawing animation
                app.render();
            }            
            
            dataPointsRendered = 1;
        }
        app.render();
	}
    free(lowPassFIR->buffer);
    free(lowPassFIR);
	SDL_DestroyTexture(app.textureText);
	SDL_DestroyWindow(app.window);
	TTF_CloseFont(app.myFont);
	SDL_Quit();

	return 0;
}
