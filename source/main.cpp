#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <switch.h>

//Our player struct
typedef struct {
	int x;
	int y;
} Player;

//Setup a texture
SDL_Texture* PlayerTex;

//Event processor
int processEvents(SDL_Window *window, Player *player){
	
	SDL_Event event;
	int done = 0;
	
	while(SDL_PollEvent(&event)){
		switch(event.type){
			case SDL_WINDOWEVENT_CLOSE:{
				if(window){
					SDL_DestroyWindow(window);
					window = NULL;
					done = 1;
				}
			}
			break;
			case SDL_QUIT:
			done = 1;
			break;
		}
	}
	return done;
}

//Where we put all the stuff we want to render
void Render(SDL_Renderer *renderer, Player *player){
	
	int w = 1920, h = 1080;
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
		

    // fill window bounds
    SDL_SetRenderDrawColor(renderer, 111, 111, 111, 255);
    SDL_Rect f = {0, 0, w, h};
    SDL_RenderFillRect(renderer, &f);
	
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_Rect r = {player->x, player->y, 64, 64};
	SDL_RenderFillRect(renderer, &r);
	SDL_RenderCopy(renderer, PlayerTex, NULL, &r); 





    SDL_RenderPresent(renderer);
	
}	

//Init window and renderer; main loop
int main(int argc, char *argv[])
{
	
	char keysNames[32][32] = {
        "KEY_A", "KEY_B", "KEY_X", "KEY_Y",
        "KEY_LSTICK", "KEY_RSTICK", "KEY_L", "KEY_R",
        "KEY_ZL", "KEY_ZR", "KEY_PLUS", "KEY_MINUS",
        "KEY_DLEFT", "KEY_DUP", "KEY_DRIGHT", "KEY_DDOWN",
        "KEY_LSTICK_LEFT", "KEY_LSTICK_UP", "KEY_LSTICK_RIGHT", "KEY_LSTICK_DOWN",
        "KEY_RSTICK_LEFT", "KEY_RSTICK_UP", "KEY_RSTICK_RIGHT", "KEY_RSTICK_DOWN",
        "KEY_SL", "KEY_SR", "KEY_TOUCH", "",
        "", "", "", ""
	};


    SDL_Window *window;
    SDL_Renderer *renderer;
    int w = 1920, h = 1080;
	
	romfsInit(); //Init the romfs for loading our image
	SDL_Init(SDL_INIT_VIDEO); //Init SDL video
	IMG_Init(IMG_INIT_PNG); //Init SDL IMG 
	
	//Create the window we're going to render our SDL2 stuff in
	window = SDL_CreateWindow("Window1",
	                          SDL_WINDOW_FULLSCREEN,
							  SDL_WINDOW_FULLSCREEN,
							  1920,
							  1080,
							  0
							  );
	//Setup the renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // mandatory at least on switch, else gfx is not properly closed (Not my code, taken from the SDL2 example)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        SDL_Log("SDL_Init: %s\n", SDL_GetError());
        return -1;
    }
	
	//Setup a player instance with our Player struct (case sensitive)
	Player player;
	player.x = 0;
	player.y = 0;
	
    
	//Specify which image file to load into our pre made surface then we map it to a texture and free the surface
	SDL_Surface* PlayerSurface = IMG_Load("romfs:assets/image/player.png"); 
	PlayerTex = SDL_CreateTextureFromSurface(renderer, PlayerSurface);
	SDL_FreeSurface(PlayerSurface); 
	
	int done = 0;
	
	//Code that executes per frame
    while (!done) {
		
		
		//States
		u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
		
		u64 kHeld = hidKeysHeld(CONTROLLER_P1_AUTO);
		
		u64 kUp = hidKeysUp(CONTROLLER_P1_AUTO);
		
		//Scan for inputs
		hidScanInput();
		
		
		//Controls (Not working, I'll fix this later)
		if (kDown & KEY_PLUS) break;
		
		if (kHeld & KEY_DLEFT) NULL;
		
		if (kHeld & KEY_DRIGHT) NULL;
				
		
		//Process our events and render what we want to render
		done = processEvents(window, &player);
		Render(renderer, &player);

    }
	
	IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}