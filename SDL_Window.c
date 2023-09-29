#include <stdio.h>
#include <sys/time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>

#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)

#define SPEED (300)

int main(){

    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0){
        printf("Error initalizing SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* win = SDL_CreateWindow("Hello!", 
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    if(!win){
        printf("Error creating window! %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
    if(!rend){
        printf("Error creating renderer%s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    SDL_Surface* surface = IMG_Load("cat.jpeg");
    if(!surface){
        printf("Error creating surface %s\n", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface);
    if(!tex){
        printf("Error creating texture %s\n", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    SDL_Rect dest;

    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    // dest.w /= 2;
    // dest.h /= 2;

    float x_pos = (WINDOW_WIDTH - dest.w) / 2;
    float y_pos = (WINDOW_HEIGHT - dest.h) / 2;
    float x_vel = SPEED;
    float y_vel = -SPEED;

    int up = 0;
    int down = 0;
    int left = 0;
    int right = 0;

    struct timeval t1, t2;
    double elapsedTime = 0;
    double overlap = 0;

    int close_requested = 0;
    while (!close_requested){

        elapsedTime += overlap;
        overlap = 0;
        gettimeofday(&t1, NULL);

        SDL_Event event;
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT){
                close_requested = 1;
            }
        }

        SDL_RenderClear(rend);

        dest.x = (int) x_pos;
        dest.y = (int) y_pos;

        SDL_RenderCopy(rend, tex, NULL, &dest);
        SDL_RenderPresent(rend);

        x_pos += (float) x_vel / 60;
        y_pos += (float) y_vel / 60;

        if (x_pos <= 0 || (x_pos + dest.w) >= WINDOW_WIDTH){
            x_vel = -x_vel;
        }

        if (y_pos <= 0 || (y_pos + dest.h) >= WINDOW_HEIGHT){
            y_vel = -y_vel;
        }

        gettimeofday(&t2, NULL);
        elapsedTime = (t1.tv_sec - t1.tv_sec) * 1000;
        if (elapsedTime > 1000/60){
            overlap = elapsedTime - 1000/60;
            elapsedTime = 1000/60;
        }

        SDL_Delay((1000/60) - elapsedTime);
    }

    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}