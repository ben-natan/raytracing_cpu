#include "vec3.hpp"
#include "object.hpp"
#include "ray.hpp"
#include "tools.hpp"
#include "mesh.hpp"
#include "trianglemesh.hpp"
#include <iostream>
#include <vector>
#include <SDL.h>
#include <thread>
#include <memory>
#include <fstream>
#include <rapidxml.hpp>
#include <stdlib.h>
#include <chrono>

#define pano false


int main() {

    int width = 640;
    int height = 640;
    int antiAliasingSample = 1;

    #if pano
    width*=2;
    height*=2
    #endif

    // // Initialiser les objets en XML;
    std::vector<std::unique_ptr<Object>> objects;
    std::vector<std::unique_ptr<Light>> lights;
    
    int n_obj = 0, n_lig = 0;

    Tools::parseObjectsAndLights("../data/spheres.xml", objects, lights, n_obj, n_lig);

    objects.emplace_back(std::make_unique<TriangleMesh>("../data/cube.obj"));
    n_obj++;


    // TriangleMesh *mesh = TriangleMesh::generatePolySphere(vec3(0,0,-1),1, 10);
    // objects.push_back(std::unique_ptr<Object>(mesh));
    // objects[0]->moveBack(1);
    // n_obj++;

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
    int frame = 0;
    std::chrono::time_point<std::chrono::system_clock> start_rendering, end_rendering;

    while (!quit) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        start_rendering = std::chrono::system_clock::now();

        std::thread calc_thread([&] () {
            for (int y = 0; y < width; y++) {
                for (int x = 0; x < height; x++) {
                    vec3 rgb;
                    double y_offset, x_offset;
                    for (int s=0; s < antiAliasingSample; s++) {
                        // std::cout << "Pixel: " << x << ", " << y << std::endl;
                        y_offset = Tools::random_double();
                        x_offset = Tools::random_double();
                        auto primaryRay = std::make_unique<Ray>(x /* + x_offset */, y /* + y_offset */, width, height, 90);   // fov en degrés
                        primaryRay->Shoot(objects, lights, n_obj, n_lig);
                        rgb += primaryRay->color();
                    }
                    rgb = 1.0/antiAliasingSample * rgb;
                    framebuffer[y + x * width] = rgb.gammaCorrect(); 
                }
            }
        });

        std::thread render_thread([&] () {
            SDL_RenderClear(s);

            for (int y = 0; y < width; y++) {
                for (int x = 0; x < height; x++) {
                    vec3 rgb = framebuffer[y + x*width];
                    SDL_SetRenderDrawColor(s, rgb.x(), rgb.y(), rgb.z(), 255);
                    SDL_RenderDrawPoint(s, x, y);
                }
            }
            SDL_RenderPresent(s);
        });

        calc_thread.join();
        render_thread.join();
        
        end_rendering = std::chrono::system_clock::now();

        std::chrono::duration<double> rendering_time = end_rendering - start_rendering;

        frame+= 1;
        std::cout << " -- Frame " << frame << " --  #  " << rendering_time.count() << " sec." << std::endl;
        
        
    }


    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(s);
    SDL_Quit();

}