#include <SDL2/SDL.h>
#include <assert.h>
#include <stdlib.h>
#include <vector>

#define HEIGHT 480
#define WIDTH 640
#define PLAYER_SIZE 50
#define BULLET_SIZE 25
#define BRICK_SIZE WIDTH / 16
#define MOVEMENT 1

bool is_inside(SDL_Rect r, int x, int y)
{
    if (x < r.x + r.w && x > r.x && y < r.y + r.h && y > r.y)
        return true;
    else
        return false;
}

int main(int argc, char* args[])
{
    /* INITIALIZATION */
    int err = SDL_Init(SDL_INIT_EVERYTHING);
    assert(err == 0);

    SDL_Window* window = SDL_CreateWindow("Jogo 1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    assert(window != NULL);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    assert(renderer != NULL);

    /* EXECUTION */
    SDL_Rect player = { WIDTH * 1 / 2, HEIGHT - PLAYER_SIZE, PLAYER_SIZE, PLAYER_SIZE };
    SDL_Event event;

    std::vector<SDL_Rect> shots;
    std::vector<SDL_Rect> bricks;

    for (size_t i = 0; i < WIDTH; i += BRICK_SIZE)
        for (size_t j = 0; j < 3 * BRICK_SIZE; j += BRICK_SIZE)
            bricks.insert(bricks.end(), { i, j, BRICK_SIZE, BRICK_SIZE });

    while (1)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
                return 0;
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_SPACE:
                {
                    SDL_Rect shot = { player.x + PLAYER_SIZE / 4, player.y, BULLET_SIZE, BULLET_SIZE };
                    shots.insert(shots.end(), shot);
                    break;
                }
                case SDLK_LEFT:
                    player.x -= 10;
                    break;
                case SDLK_RIGHT:
                    player.x += 10;
                    break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);
        SDL_RenderFillRect(renderer, NULL);

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0x00);
        SDL_RenderFillRect(renderer, &player);

        for (auto &it : shots)
        {
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0x00);
            SDL_RenderFillRect(renderer, &it);
        }

        for (auto &it : bricks)
        {
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0x00);
            SDL_RenderFillRect(renderer, &it);
        }

        SDL_RenderPresent(renderer);

        auto it = shots.begin();
        while (it != shots.end()) 
        {
            (*it).y -= MOVEMENT;

            if ((*it).y < 0)
                it = shots.erase(it);
            else
                ++it;
        }

        bool stop = false;
        auto it_shots = shots.begin();
        while (it_shots != shots.end())
        {
            auto it_bricks = bricks.begin();
            while (it_bricks != bricks.end())
            {
                if (is_inside((*it_bricks), (*it_shots).x, (*it_shots).y))
                {
                    it_bricks = bricks.erase(it_bricks);
                    it_shots = shots.erase(it_shots);
                    stop = true;
                    break;
                }
                else
                    it_bricks++;
            }
            if (stop)
                break;
            else
                it_shots++;
        }
    }

    /* FINALIZATION */
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}