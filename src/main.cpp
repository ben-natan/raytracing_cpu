// #include <SFML/Window.hpp>
#include "light.hpp"
#include "vec3.hpp"
#include "mat4.hpp"
#include "sphere.hpp"
#include "plane.hpp"
#include "ray.hpp"
#include <iostream>
#include <vector>
#include <SDL.h>
#include <thread>
#include <memory>

#define pano false

int main() {
    
    int width = 640;
    int height = 640;

    #if pano
    width*=2;
    height*=2
    #endif

    // // Initialiser les objets en XML;
    std::vector<std::unique_ptr<Object>> objects;
    std::vector<std::unique_ptr<Light>> lights;
    
    int n_obj = 5, n_lig = 4;

    // Load lights

    lights.emplace_back(std::make_unique<PointLight>(PointLight(mat4(), vec3(-5,3.5,-6))));
    lights.emplace_back(std::make_unique<PointLight>(PointLight(mat4(), vec3(-5,3.5,-6))));
    lights.emplace_back(std::make_unique<PointLight>(PointLight(mat4(), vec3(0,-3.5,6))));
    lights.emplace_back(std::make_unique<PointLight>(PointLight(mat4(), vec3(0,-3.5,6))));
    // lights.emplace_back(std::make_unique<PointLight>(PointLight(mat4(), vec3(-1,-5,-1))));
    // lights.emplace_back(std::make_unique<PointLight>(PointLight(mat4(), vec3(0,0,0))));

    //Load objects
    
    // objects.emplace_back(std::make_unique<Sphere>(Sphere(vec3(0,-1.5,-3), 1.5, vec3(255,100,0))));

    objects.emplace_back(std::make_unique<Sphere>(Sphere(vec3(4.5,4,-12), 5.0f, vec3(123,20,50))));
    objects.emplace_back(std::make_unique<Sphere>(Sphere(vec3(-3.5,2,-10), 0.5f, vec3(0,0,0), 0.0))); 
    objects.emplace_back(std::make_unique<Sphere>(Sphere(vec3(-4.5,1,-14), 2.0f, vec3(255,0,0))));
    objects.emplace_back(std::make_unique<Sphere>(Sphere(vec3(-0.5,0.5,-3), 1.0f, vec3(0,0,255))));
    objects.emplace_back(std::make_unique<Plane>(Plane(vec3(0,-3.0f,-17),vec3(0,1,0), vec3(50,0,50))));


    
    // objects.emplace_back(std::make_unique<Sphere>(Sphere(vec3(-2,-1,-4.5), 2.0, vec3(255,100,0), 0.2)));
    // objects.emplace_back(std::make_unique<Sphere>(Sphere(vec3(5,0,-6), 2.0, vec3(255, 255, 0), 0.2)));
    // objects.emplace_back(std::make_unique<Sphere>(Sphere(vec3(-3,3,-11), 2.0, vec3(0, 0, 255), 0.2)));
    // objects.emplace_back(std::make_unique<Sphere>(Sphere(vec3(3,3,-10), 2.0, vec3(0, 100, 0), 0.2)));
    
    // objects.emplace_back(std::make_unique<Plane>(Plane(vec3(-1,-8,0), vec3(0,1,0))));


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

    std::vector<vec3> framebuffer = std::vector<vec3>(width*height); 

    while (!quit) {
        //drawing particles
        //setting up objects
        //repeated over and over again

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        

        std::thread calc_thread([&] () {
            for (int y = 0; y < width; y++) {
                for (int x = 0; x < height; x++) {
                        // Ray primaryRay(x,y,width, height, 2.0); // std::unique_ptr & std::shared_ptr
                        // if (x ==  height/2 && y ==  width /2 ) {
                        auto primaryRay = std::make_unique<Ray>(x,y,width,height,90);   // fov en degrés
                        int min_obj_ind;
                        float distance;
                        float min_distance = INFINITY;
                        vec3 pHit, normal, color;
                        vec3 min_pHit, min_normal, min_color;
                        
                        primaryRay->Shoot(objects, lights, n_obj, n_lig);
                        
                        vec3 rgb = primaryRay->color();
                        framebuffer[y + x * width] = rgb; 
                }
            }
        });

        std::thread render_thread([&] () {
            // We clear what we draw before
            SDL_RenderClear(s);

            // Now we can draw our point
            for (int y = 0; y < width; y++) {
                for (int x = 0; x < height; x++) {
                    vec3 rgb = framebuffer[y + x*width];
                    SDL_SetRenderDrawColor(s, rgb.x(), rgb.y(), rgb.z(), 255);
                    SDL_RenderDrawPoint(s, x, y);
                }
            }

            std::cout << "Frame rendered" << std::endl;

            // And now we present everything we draw after the clear.
            SDL_RenderPresent(s);
        });

        calc_thread.join();
        render_thread.join();
        
    }


    SDL_DestroyWindow(window);
    // We have to destroy the renderer, same as with the window.
    SDL_DestroyRenderer(s);
    SDL_Quit();

}