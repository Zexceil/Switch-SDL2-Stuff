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

typedef struct {
    int exit;
} Status;

//Setup a texture
SDL_Texture* PlayerTex;
SDL_Texture* BackgroundTex;

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

//Put the stuff to init
void Initializer(){
    
    romfsInit(); //Init the romfs for loading our image
	SDL_Init(SDL_INIT_VIDEO); //Init SDL video
    IMG_Init(IMG_INIT_PNG); //Init SDL IMG 
}


//Where we put all the stuff we want to render
void Render(SDL_Renderer *renderer, Player *player){
	
	int w = 1920, h = 1080;
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
		

    // fill window bounds
    SDL_SetRenderDrawColor(renderer, 111, 111, 111, 255);
    SDL_Rect f = {0, 0, w, h};
	SDL_RenderCopy(renderer, BackgroundTex, NULL, &f);
	
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_Rect r = {player->x, player->y, 64, 64};
	SDL_RenderFillRect(renderer, &r);
	SDL_RenderCopy(renderer, PlayerTex, NULL, &r); 





    SDL_RenderPresent(renderer);
	
}	

//Texture loading stuff
void LoadTextures(SDL_Renderer *renderer){
    
    
    
    //Specify which image file to load into our pre made surface then we map it to a texture and free the surface
    SDL_Surface* PlayerSurface = IMG_Load("romfs:assets/image/player.png"); 
	SDL_Surface* BackgroundSurface = IMG_Load("romfs:assets/image/Background.png"); 
	
	PlayerTex = SDL_CreateTextureFromSurface(renderer, PlayerSurface);
	BackgroundTex = SDL_CreateTextureFromSurface(renderer, BackgroundSurface);
    
	SDL_FreeSurface(PlayerSurface); 
	SDL_FreeSurface(BackgroundSurface);
}

//All the controls stuff
void Controls(Player *player, Status *status){
    
	//States
	u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
		
	u64 kHeld = hidKeysHeld(CONTROLLER_P1_AUTO);
		
	u64 kUp = hidKeysUp(CONTROLLER_P1_AUTO);
		
	//Scan for inputs
	hidScanInput();
	
	if (kDown & KEY_PLUS) status->exit = 1;
		
	if (kHeld & KEY_DLEFT) player->x--;
		
	if (kHeld & KEY_DRIGHT) player->x++;
	
	if (kHeld & KEY_DUP) player->y--;
	
	if (kHeld & KEY_DDOWN) player->y++;
	
	
}

//Init window and renderer; main loop
int main(int argc, char *argv[])
{
	
    SDL_Window *window;
    SDL_Renderer *renderer;
	
    //Init everything
	Initializer();
    
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
    
    Status status;
    status.exit = 0;
	

	//Call function to load in textures
	LoadTextures(renderer);
	
    //Make sure done is 0 so we can keep the loop running
	int done = 0;
    
	//Code that executes per frame
    while (!done) {
		
	//Process our events and render what we want to render
	done = processEvents(window, &player);
	Controls(&player, &status);
	Render(renderer, &player);

	//Used to break out of loop when plus pressed
	if (status.exit == 1){
		done = 1;
		break;
		}
    }
	
	IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
