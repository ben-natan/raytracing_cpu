// #include <SFML/Window.hpp>
#include "light.hpp"
#include "vec3.hpp"
#include "mat4.hpp"
#include "sphere.hpp"
#include "plane.hpp"
#include "ray.hpp"
#include "tools.hpp"
#include <iostream>
#include <vector>
#include <SDL.h>
#include <thread>
#include <memory>
#include <fstream>
#include <rapidxml.hpp>
#include <stdlib.h>




#define pano false


// using namespace rapidxml;


int main() {
    
    int width = 640;
    int height = 640;

    #if pano
    width*=2;
    height*=2
    #endif

    // // Initialiser les objets en XML;

    std::cout << "Loading world.." << std::endl;
    std::cout << std::endl;


    std::vector<std::unique_ptr<Object>> objects;
    std::vector<std::unique_ptr<Light>> lights;
    
    int n_obj = 0, n_lig = 0;

    // Tools::parseObjectsAndLights("../data/spheres.xml", objects, lights, n_obj, n_lig);

    rapidxml::xml_document<> doc;
    rapidxml::xml_node<> * root_node;

    std::ifstream theFile("../data/spheres.xml");
    std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');
            
    doc.parse<0>(&buffer[0]);

    root_node = doc.first_node("World");
    rapidxml::xml_node<> * objects_root = root_node->first_node("Objects");
    rapidxml::xml_node<> * lights_root = root_node->first_node("Lights");


    // Load objects
    rapidxml::xml_node<> * spheres_root = objects_root->first_node("Spheres");
    for (rapidxml::xml_node<> * sphere = spheres_root->first_node("Sphere"); sphere; sphere = sphere->next_sibling()) {
        std::cout << "Loading sphere" << " " << n_obj << ".." << std::endl;
        float cx, cy, cz, r;
        cx = atof(sphere->first_attribute("cx")->value());
        cy = atof(sphere->first_attribute("cy")->value());
        cz = atof(sphere->first_attribute("cz")->value());
        r = atof(sphere->first_attribute("r")->value());
        objects.emplace_back(std::make_unique<Sphere>(Sphere(vec3(cx,cy,cz), r, vec3(123,20,50))));
        n_obj++;
    }

    rapidxml::xml_node<> * planes_root = objects_root->first_node("Planes");
    for (rapidxml::xml_node<> * plane = planes_root->first_node("Plane"); plane; plane = plane->next_sibling()) {
        std::cout << "Loading plane" << " " << n_obj << ".." << std::endl;
        float px, py, pz;
        float nx, ny, nz;
        px = atof(plane->first_attribute("px")->value());
        py = atof(plane->first_attribute("py")->value());
        pz = atof(plane->first_attribute("pz")->value());
        nx = atof(plane->first_attribute("nx")->value());
        ny = atof(plane->first_attribute("ny")->value());
        nz = atof(plane->first_attribute("nz")->value());
        objects.emplace_back(std::make_unique<Plane>(Plane(vec3(px,py,pz),vec3(nx,ny,nz), vec3(50,0,50))));
        n_obj++;
    }

    // Load lights
    rapidxml::xml_node<> * pLights_root = lights_root->first_node("PointLights");
    for (rapidxml::xml_node<> * light = pLights_root->first_node("PointLight"); light; light = light->next_sibling()) {
        std::cout << "Loading pointLight" << " " << n_lig << ".." << std::endl;
        float px, py, pz;
        px = atof(light->first_attribute("px")->value());
        py = atof(light->first_attribute("py")->value());
        pz = atof(light->first_attribute("pz")->value());
        lights.emplace_back(std::make_unique<PointLight>(PointLight(mat4(), vec3(px,py,pz))));
        n_lig++;
    }
    std::cout << std::endl;
    std::cout << "Successfully loaded " << n_obj << " objects and " << n_lig << " lights." << std::endl;
    std::cout << std::endl;


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
    int frame = -1;

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
                        framebuffer[y + x * width] = rgb.gammaCorrect(); 
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
            frame+= 1;
            if (frame != 0) {
                std::cout << " -- Frame " << frame << std::endl;
            }
            

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