#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define WIDTH 320
#define HEIGHT 200 
// #define WIDTH 80
// #define HEIGHT 25
#define SCALE 3

uint32_t framebuffer[WIDTH * HEIGHT];

void color_pixel(uint32_t x, u_int32_t y, uint32_t color){
    if(x < 0 || y < 0 ) return;
    framebuffer[(WIDTH * y) + x] = color;
}

void draw_pixel_center(){
    color_pixel(WIDTH/2, HEIGHT/2, 0xFF00FF00);
}

void draw_diagonal(){
    //Display RGB diagonal.
    //Fix bug in this function; 
    uint8_t colorCode = 0;
    for (int i=0; i<HEIGHT; i++){
        for (int j=0; j<HEIGHT; j++){
            if(i == j){
                uint32_t ind = (WIDTH * i) + j; 
                if(colorCode == 0){
                    // framebuffer[ind] = 0xFFFF0000; //red
                    color_pixel(i, j, 0xFFFF0000);
                } 
                else if(colorCode == 1) {
                    // framebuffer[ind] = 0xFF00FF00; //green
                    color_pixel(i, j, 0xFF00FF00);
                }
                else if(colorCode == 2){
                    // framebuffer[ind] = 0xFF0000FF; //blue
                    color_pixel(i, j, 0xFF0000FF);
                } 
                colorCode = (colorCode + 1) % 3;
            }
        }
    }  
}

void draw_random_figures(){
    u_int32_t colorCode = 0;

    for(int i=0; i<HEIGHT; i++){
        colorCode = 0;
        for(int j=0; j<WIDTH; j++){
            if(i%5 == 0){
                if(colorCode == 0){
                    color_pixel(i, j, 0xFF00FF00);
                }
                else if(colorCode == 1){
                    color_pixel(i, j, 0xFFFF0000);
                }
                else if(colorCode == 2) {
                    color_pixel(i, j, 0xFF0000FF);
                }
                colorCode = (colorCode + 1) % 3;
            } 
        }
    }
}

int main(void)
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;

    // Initialize SDL video subsystem
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        printf("SDL Initialization Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Create window
    window = SDL_CreateWindow(
        "SDL Framebuffer",
        WIDTH * SCALE,
        HEIGHT * SCALE,
        0
    );

    if (window == NULL)
    {
        printf("Window Creation Error: %s\n", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, NULL);

    if (renderer == NULL)
    {
        printf("Renderer Creation Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // Create texture
    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_XRGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH,
        HEIGHT
    );

    if (texture == NULL)
    {
        printf("Texture Creation Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // Initialize framebuffer with black pixels
    for (int i = 0; i < WIDTH * HEIGHT; i++)
    {
        framebuffer[i] = 0xFF000000;
    }

    // Draw a green pixel at coordinate (0, 0)
    // framebuffer[0] = 0xFF00FF00;

    //Draw green diagonal
    // uint16_t minVal = HEIGHT;
    // bool isGreen = true;
    // for (int i=0; i<HEIGHT; i++){
    //     for (int j=0; j<HEIGHT; j++){
    //         if(i == j){
    //             uint32_t ind = (WIDTH * i) + j; 
    //             if(isGreen) framebuffer[ind] = 0xFF00FF00;
    //             else framebuffer[ind] = 0xFF0000FF; // blue color
    //             isGreen = !isGreen;
    //         }
    //     }
    // }

    // draw_diagonal();
    draw_pixel_center();
    // draw_random_figures();

    bool running = true;
    SDL_Event event;

    // Main application loop
    while (running)
    {
        // Handle window events
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }

        // Update texture with framebuffer data
        SDL_UpdateTexture(
            texture,
            NULL,
            framebuffer,
            WIDTH * sizeof(uint32_t)
        );

        // Clear renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render texture
        SDL_RenderTexture(
            renderer,
            texture,
            NULL,
            NULL
        );

        // Display rendered output
        SDL_RenderPresent(renderer);

        // Limit rendering speed
        SDL_Delay(16);
    }

    // Release resources
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return EXIT_SUCCESS;
}

