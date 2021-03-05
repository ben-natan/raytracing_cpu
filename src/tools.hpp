#ifndef TOOLS_H
#define TOOLS_H

#include <math.h>
#include <algorithm>
#include <random>
#include "light.hpp"
#include <rapidxml.hpp>
#include <fstream>
#include "object.hpp"
#include "sphere.hpp"
#include "plane.hpp"
#include "triangle.hpp"


class Tools {
    public:
        static bool solveQuadratic(float a, float b, float c, float &x0, float &x1) {
            float discr = b*b - 4*a*c;
            if (discr < 0) {
                return false;
            } else if ( discr == 0 ) {
                x0 = x1 = -0.5 * b / a;
                return true;
            } else {
                float q = sqrt(discr);
                x0 = (-b + q)/(2*a);
                x1 = (-b - q)/(2*a);
                if (x0 > x1) {
                    std::swap(x0,x1); // x1 plus grand que x0
                }
                return true;
            };
        }
        
        static double random_double() {
            static std::uniform_real_distribution<double> distribution(0.0, 1.0);
            static std::mt19937 generator;
            return distribution(generator);
        }

        static void parseObjectsAndLights(std::string xmlfilename, std::vector<std::unique_ptr<Object>>& objects,
                                            std::vector<std::unique_ptr<Light>>& lights, int& n_obj, int& n_lig) {
            
            std::chrono::time_point<std::chrono::system_clock> start, end;
            start = std::chrono::system_clock::now();

            std::cout << "Loading world.." << std::endl;
            std::cout << std::endl;

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
            if (objects_root) {
                rapidxml::xml_node<> * spheres_root = objects_root->first_node("Spheres");
                if (spheres_root) {
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
                }
                

                rapidxml::xml_node<> * planes_root = objects_root->first_node("Planes");
                if (planes_root) {
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
                }
            }
            
            // Load lights
            if (lights_root) {
                rapidxml::xml_node<> * pLights_root = lights_root->first_node("PointLights");
                if (pLights_root) {
                    for (rapidxml::xml_node<> * light = pLights_root->first_node("PointLight"); light; light = light->next_sibling()) {
                        std::cout << "Loading pointLight" << " " << n_lig << ".." << std::endl;
                        float px, py, pz;
                        px = atof(light->first_attribute("px")->value());
                        py = atof(light->first_attribute("py")->value());
                        pz = atof(light->first_attribute("pz")->value());
                        lights.emplace_back(std::make_unique<PointLight>(PointLight(vec3(px,py,pz))));
                        n_lig++;
                    }
                }

                rapidxml::xml_node<> * dirLights_root = lights_root->first_node("DirectionalLights");
                if (dirLights_root) {
                    for (rapidxml::xml_node<> * light = dirLights_root->first_node("DirectionalLight"); light; light = light->next_sibling()) {
                        std::cout << "Loading directionalLight " << n_lig << ".." << std::endl;
                        float dx, dy, dz;
                        dx = atof(light->first_attribute("dx")->value());
                        dy = atof(light->first_attribute("dx")->value());
                        dz = atof(light->first_attribute("dx")->value());
                        lights.emplace_back(std::make_unique<DirectionalLight>(DirectionalLight(vec3(dx,dy,dz))));
                        n_lig++;
                    }
                }
            }

            end = std::chrono::system_clock::now();
            std::chrono::duration<double> loading_time = end - start;

            std::cout << std::endl;
            std::cout << "Successfully loaded " << n_obj << " objects and " << n_lig << " lights in " << loading_time.count() << " seconds. " << std::endl;
            std::cout << std::endl;
        }
};

#endif