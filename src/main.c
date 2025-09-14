#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#define SDL_MAIN_HANDLED
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WINDOW_WIDTH 256
#define WINDOW_HEIGHT 152
#define SPEED 15

int quit = 0;
int display_width = 0;
int display_height = 0;

int win_x, win_y;
float vx = 0.4 * SPEED, vy = 0.3 * SPEED;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Surface *surface = NULL;
SDL_Texture *texture = NULL;

void move(void);
void clean(void);

int main(int argc, char* argv[]) {
    srand(time(0));

    vx = (rand() % 2 == 0) ? -vx : vx;
    vy = (rand() % 2 == 0) ? -vy : vy;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL failed to initialise: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("DVD Logo",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              WINDOW_WIDTH,
                              WINDOW_HEIGHT,
                              0);


    SDL_SetWindowAlwaysOnTop(window, SDL_TRUE);

    if (window == NULL) {
        fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    surface = IMG_Load("dvd_logo.png");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_SetTextureColorMod(texture, rand() % 255, rand() % 255, rand() % 255);

    SDL_Rect display_bounds;
    if (SDL_GetDisplayBounds(0, &display_bounds) == 0) {
        display_width = display_bounds.w;
        display_height = display_bounds.h;
    }

    while (!quit) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }

        move();

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        SDL_Delay(20);
    }

    clean();
    return 0;
}

void move(void) {
    SDL_GetWindowPosition(window, &win_x, &win_y);

    win_x += vx;
    win_y += vy;

    if (win_y <= 0 || win_y >= display_height - WINDOW_HEIGHT) {
        vy = -vy;
        SDL_SetTextureColorMod(texture, rand() % 255, rand() % 255, rand() % 255);
    }

    if (win_x <= 0 || win_x >= display_width - WINDOW_WIDTH) {
        vx = -vx;
        SDL_SetTextureColorMod(texture, rand() % 255, rand() % 255, rand() % 255);
    }

    SDL_SetWindowPosition(window, win_x, win_y);
}

void clean(void) {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();
}
