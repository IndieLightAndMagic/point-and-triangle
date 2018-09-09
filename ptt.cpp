#include <iostream>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>

#include <SDL2/SDL.h>



SDL_Window* pWindow;
SDL_Renderer* pRenderer;
constexpr int WindowWidth = 1200;
constexpr int WindowHeight = 800;
glm::vec2* ppx{nullptr};
void GetMouseStateNormalCoordinates(){

    int x,y;
    SDL_GetMouseState(&x, &y);

    ppx->x = x;
    ppx->y = y;
    ppx->x /= WindowWidth;
    ppx->y /= WindowHeight;

}
/**
 * @brief      Calculates the inv denominator of a ABC triangle.
 *
 * @param[in]  AB2    Square Length of side AB.
 * @param[in]  AC2    Square Length of side AC.
 * @param[in]  Apert  Aperture and projection of AB and AC (Dot (AB, AC) )
 *
 * @return     A float representing the inverse of ABC Triangle Denominator.
 */
float CalcInvDenominator(
    float AB2, 
    float AC2, 
    float Apert){

    return 1.0f / (AB2 * AC2 - Apert * Apert);

}

/**
 * @brief      Calculates if a Point described by vector pi is inside Triangle A,B,C.  
 *
 * @param      pi        Vector to Point to test (SC: screen coordinates)
 * @param      A         Vector to Point A of the triangle (SC)         
 * @param      AB        Vector going from point A to point B in the triangle, <B-A>
 * @param      AC        Vector going from point A to point C in the triangle, <C-A>
 * @param      AB2       Square Length of side AB.
 * @param      Apert     Aperture and projection of AB and AC (Dot (AB, AC) )
 * @param      AC2       Square Length of side AC.
 * @param[in]  invDenom  The inv denominator
 * @param      w1        The w 1
 * @param      w2        The w 2
 *
 * @return     If pi is in triangle return true, otherwise return false.
 */
bool IsItPiInABCTrianle(

    const glm::vec2& pi, 
    const glm::vec2& A, 
    const glm::vec2& AB, 
    const glm::vec2& AC, 
    const float& AB2, 
    const float& Apert, 
    const float& AC2, 
    const float invDenom, 
    float& w1, 
    float& w2){

    auto APi    = pi - A;
    auto dot02  = glm::dot(AB, APi);
    auto dot12  = glm::dot(AC, APi);
    w1          = (AC2 * dot02 - Apert * dot12) * invDenom;
    w2          = (AB2 * dot12 - Apert * dot02) * invDenom;

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
    glm::vec2 AB{0.0f, 0.0f};
    glm::vec2 AC{0.0f, 0.0f};
    glm::vec2 APi{0.0f, 0.0f};

    auto AB2        = glm::dot(AB, AB);
    auto Apert      = glm::dot(AB, AC);
    auto dot02      = glm::dot(AB, APi);
    auto AC2        = glm::dot(AC, AC);
    auto dot12      = glm::dot(AC, APi);
    auto invDenom   = 1.0f / (AB2 * AC2 - Apert * Apert);
    auto w1         = (AC2 * dot02 - Apert * dot12) * invDenom;
    auto w2         = (AB2 * dot12 - Apert * dot02) * invDenom;
    auto inTriangle = false;
    ppx             = &p1;

    std::cout << "FIRST_POINT: ";
                
    while (!quit){

        while (SDL_PollEvent(&e) != 0){

            if (e.type == SDL_QUIT){

                quit = true;
                continue;

            } else if (e.type == SDL_MOUSEBUTTONUP){

                GetMouseStateNormalCoordinates();
                
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

                    AB  = p2 - p1;
                    AB2 = glm::dot(AB, AB);


                } else if ( state == State::THIRD_POINT){

                    std::cout << "(" << ppx->x << " , " << ppx->y << ")" << std::endl;
                    std::cout << "CALC_POINT: ";
                    state = State::CALC_POINT;
                    ppx = &pi;

                    AC          = p3 - p1;
                    Apert       = glm::dot(AB, AC);
                    AC2         = glm::dot(AC, AC);
                    invDenom    = CalcInvDenominator(AB2, AC2, Apert);

                } else if ( state == State::CALC_POINT){

                    inTriangle = IsItPiInABCTrianle(pi, p1, AB, AC, AB2, Apert, AC2, invDenom, w1, w2);
                    if( inTriangle ){
                        std::cout << "In " << w1 << " , " << w2 << "\n";
                    } else {
                        std::cout << "Out " << w1 << " , " << w2 << "\n";
                    }
                }

            } else if (e.type == SDL_MOUSEMOTION && state == State::CALC_POINT){

                GetMouseStateNormalCoordinates();
                inTriangle = IsItPiInABCTrianle(pi, p1, AB, AC, AB2, Apert, AC2, invDenom, w1, w2);

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





































