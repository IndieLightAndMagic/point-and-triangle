#include <iostream>

#include <SDL2/SDL.h>



SDL_Window* pWindow;
SDL_Renderer* pRenderer;

void SDLInitialization(){


	constexpr int WindowWidth = 1200;
    constexpr int WindowHeight = 800;

    Uint32 m_initFlags = SDL_INIT_EVERYTHING;
    if (SDL_Init(m_initFlags) != 0){
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        SDL_assert(false);
    }
    pWindow = nullptr;
    pWindow = SDL_CreateWindow(
            "Kill the Martian!!!",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            WindowWidth,
            WindowHeight,
            SDL_WINDOW_OPENGL
    );
    if (!pWindow){
        std::cerr << "SDL CreateWindow failed. \n";
        SDL_assert(false);
    }
    pRenderer = nullptr;
    pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_TARGETTEXTURE);
    if (!pRenderer){
        std::cerr << "SDL_CreatRenderer failed. \n";
        SDL_assert(false);
    }

}


int main (){
	return 0;
}