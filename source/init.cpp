#include "init.h"

void Initializer::Init(){
		romfsInit(); //Init the romfs for loading our image
	    SDL_Init(SDL_INIT_VIDEO); //Init SDL video
        IMG_Init(IMG_INIT_PNG); //Init SDL IMG 
	};

