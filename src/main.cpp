// #include <SFML/Window.hpp>
#include "vec3.hpp"
#include "sphere.hpp"
#include "ray.hpp"
#include <iostream>
#include <typeinfo>
#include <vector>
#include <SDL.h>
#include <memory>


int main() {
    
    int width = 640;
    int height = 480;

    // // Initialiser les objets en XML;
    // std::vector<std::unique_ptr<Object>> objects[3];

    Sphere objects[1];
    for (int i = 0; i<1; i++) {
        objects[i].setCenter(vec3(3,3,-5)); // Problème dans primary ray qui déforme l'image quand on bouge du centre
        objects[i].setRadius(0.5);
    }


    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return 1;
        std::cout << "Initialization failed" << std::endl;
    }

    SDL_Window *window = SDL_CreateWindow("Raytracing",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width,
            height, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        SDL_Quit();
        return 2;
    }

    // We create a renderer with hardware acceleration, we also present according with the vertical sync refresh.
    SDL_Renderer *s = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) ;

    bool quit = false;
    SDL_Event event;

    while (!quit) {
        //drawing particles
        //setting up objects
        //repeated over and over again

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        // We clear what we draw before
        SDL_RenderClear(s);

        
        // Now we can draw our point
        for (int y = 0; y < width; y++) {
            for (int x = 0; x < height; x++) {
                // Ray primaryRay(x,y,width, height, 2.0); // std::unique_ptr & std::shared_ptr
                auto primaryRay = std::make_unique<Ray>(x,y,width,height,90);   // fov en degrés
                // std::unique_ptr<Object> min_obj;
                float distance;
                float min_distance = INFINITY;
                vec3 pHit, normal, color;
                vec3 min_pHit, min_normal, min_color;
                for (int i = 0; i < 1; i++) {
                    if (objects[i].intersect(primaryRay, &distance, pHit, normal, color)) {
                        if (distance < min_distance) {
                            min_distance = distance;
                            min_pHit = pHit;
                            min_normal = normal;
                            min_color = color;
                            // min_obj = std::make_unique<Object>(objects[i]);
                        }
                    }
                }
                // for every light:
                    // cast shadow ray
                    // color += ambient term
                if (min_distance != INFINITY ) {
                    // if (min_obj->isMirror()) {
                    //     // Shoot un ray
                    //     auto mirrorRay = std::make_unique<Ray>();
                    //     primaryRay->addColor(min_obj->k_mirror() * mirrorRay->color());
                    // }
                    // if (min_obj->isTransparent()) {
                    //     // Shoot un ray
                    //     auto transparentRay = std::make_unique<Ray>();
                    //     primaryRay->addColor(min_obj->k_transparent() * transparentRay->color());
                    // }
                
                    

                    SDL_SetRenderDrawColor(s, 120, 0, 0, 255);
                    SDL_RenderDrawPoint(s, x, y);
                    
                    primaryRay->Shoot(); // avec phit normal et color
                } else {
                    SDL_SetRenderDrawColor(s, 0,0,0,0);
                    SDL_RenderDrawPoint(s, x, y);
                }
            }
        }

        // And now we present everything we draw after the clear.
        SDL_RenderPresent(s);
    }


    SDL_DestroyWindow(window);
    // We have to destroy the renderer, same as with the window.
    SDL_DestroyRenderer(s);
    SDL_Quit();

}