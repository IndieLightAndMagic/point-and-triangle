#include <iostream>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>

#include <SDL2/SDL.h>



SDL_Window* pWindow;
SDL_Renderer* pRenderer;
constexpr int WindowWidth = 1200;
constexpr int WindowHeight = 800;

bool CalcInTriangle(glm::vec2& pi, glm::vec2& p1, glm::vec2& AB, glm::vec2& AC, float& dot00, float& dot01, float& dot11, float invDenom, float& w1, float& w2){

    auto v2     = pi - p1;
    auto dot02  = glm::dot(AB, v2);
    auto dot12  = glm::dot(AC, v2);
    w1          = (dot11 * dot02 - dot01 * dot12) * invDenom;
    w2          = (dot00 * dot12 - dot01 * dot02) * invDenom;

    return (w1 >= 0) && (w2 >= 0) && (w1 + w2 < 1.0f);
}
void RenderLine(glm::vec2& p0, glm::vec2& p1){

    glm::ivec2 ip0{p0.x * WindowWidth, p0.y * WindowHeight};
    glm::ivec2 ip1{p1.x * WindowWidth, p1.y * WindowHeight};
    SDL_RenderDrawLine(pRenderer, ip0.x, ip0.y, ip1.x, ip1.y);

}

int SDLInitialization(){

    Uint32 m_initFlags = SDL_INIT_EVERYTHING;
    if (SDL_Init(m_initFlags) != 0){
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        SDL_assert(false);
        return 0;
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
        return 0;
    }
    pRenderer = nullptr;
    pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_TARGETTEXTURE);
    if (!pRenderer){
        std::cerr << "SDL_CreatRenderer failed. \n";
        SDL_assert(false);
        return 0;
    }
    return 1;

}

void MainLoop(){

    auto quit = false;
    SDL_Event e{};

    enum class State {
        FIRST_POINT,
        SECOND_POINT,
        THIRD_POINT,
        CALC_POINT
    };

    State state{State::FIRST_POINT};
    
    
    glm::vec2 p1{0.0f, 0.0f};
    glm::vec2 p2{0.0f, 0.0f};
    glm::vec2 p3{0.0f, 0.0f};
    glm::vec2 pi{0.0f, 0.0f};
    glm::vec2 v0{0.0f, 0.0f};
    glm::vec2 v1{0.0f, 0.0f};
    glm::vec2 v2{0.0f, 0.0f};

    auto dot00      = glm::dot(v0, v0);
    auto dot01      = glm::dot(v0, v1);
    auto dot02      = glm::dot(v0, v2);
    auto dot11      = glm::dot(v1, v1);
    auto dot12      = glm::dot(v1, v2);
    auto invDenom   = 1.0f / (dot00 * dot11 - dot01 * dot01);
    auto w1         = (dot11 * dot02 - dot01 * dot12) * invDenom;
    auto w2         = (dot00 * dot12 - dot01 * dot02) * invDenom;
    auto inTriangle = false;
    glm::vec2* ppx = &p1;

    std::cout << "FIRST_POINT: ";
                
    while (!quit){

        while (SDL_PollEvent(&e) != 0){

            if (e.type == SDL_QUIT){

                quit = true;
                continue;

            } else if (e.type == SDL_MOUSEBUTTONUP){

                int x,y;
                SDL_GetMouseState(&x, &y);

                ppx->x = x;
                ppx->y = y;
                ppx->x /= WindowWidth;
                ppx->y /= WindowHeight;

                if ( state == State::FIRST_POINT ){

                    std::cout << "(" << ppx->x << " , " << ppx->y << ")" << std::endl;
                    ppx = &p2;
                    state = State::SECOND_POINT;
                    std::cout << "SECOND_POINT: ";


                } else if ( state == State::SECOND_POINT ){

                    std::cout << "(" << ppx->x << " , " << ppx->y << ")" << std::endl;
                    std::cout << "THIRD_POINT: ";
                    state = State::THIRD_POINT;
                    ppx = &p3;

                    v0 = p2 - p1;
                    dot00 = glm::dot(v0, v0);


                } else if ( state == State::THIRD_POINT){

                    std::cout << "(" << ppx->x << " , " << ppx->y << ")" << std::endl;
                    std::cout << "CALC_POINT: ";
                    state = State::CALC_POINT;
                    ppx = &pi;

                    v1          = p3 - p1;
                    dot01       = glm::dot(v0, v1);
                    dot11       = glm::dot(v1, v1);
                    invDenom    = 1.0f / (dot00 * dot11 - dot01 * dot01);


                } else if ( state == State::CALC_POINT){


                    inTriangle = CalcInTriangle(pi, p1, v0, v1, dot00, dot01, dot11, invDenom, w1, w2);
                    if( inTriangle ){
                        std::cout << "In " << w1 << " , " << w2 << "\n";
                    } else {
                        std::cout << "Out " << w1 << " , " << w2 << "\n";
                    }
                }

            } else if (e.type == SDL_KEYDOWN) {

                if (e.key.keysym.sym == SDLK_ESCAPE) {

                    quit = true;
                    continue;

                }
            }
        }

        SDL_SetRenderDrawColor( pRenderer, 0x00, 0x00, 0x00, 0xFF );
        SDL_RenderClear( pRenderer );
        SDL_SetRenderDrawColor( pRenderer, 0xff, 0x00, 0x00, 0xFF );
        if (state == State::THIRD_POINT){
            RenderLine(p1, p2);
        } else if (state == State::CALC_POINT) {
            RenderLine(p1, p2);
            RenderLine(p2, p3);
            RenderLine(p1, p3);

            auto u = p2 - p1;
            glm::normalize(u);

            auto v = p3 - p1;
            glm::normalize(v);

            if (inTriangle){
                u *= w1;
                v *= w2;
                auto p1u = p1 + u;
                auto p1v = p1 + v + u;
                SDL_SetRenderDrawColor( pRenderer, 0x00, 0xff, 0x00, 0xFF );
                RenderLine(p1, p1u);
                SDL_SetRenderDrawColor( pRenderer, 0x00, 0x00, 0xff, 0xFF );
                RenderLine(p1u, p1v);
            }

        }

        //Update screen
        SDL_RenderPresent( pRenderer );
    }
    
}

void SDLClosing(){

    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();

}
int main (){

    if (SDLInitialization() == 0) return 0;
    MainLoop();
    SDLClosing();
    return 0;
}





































