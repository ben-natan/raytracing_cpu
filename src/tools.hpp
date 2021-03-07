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
#include "trianglemesh.hpp"
#include "texture.hpp"
#include <chrono>
#include <cstring>

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

            std::ifstream theFile(xmlfilename);
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
                        std::cout << "Loading object" << " " << n_obj + 1 << " : Sphere.." << std::endl;
                        float cx, cy, cz, r;
                        int red, green, blue;
                        char* texture;
                        cx = atof(sphere->first_attribute("cx")->value());
                        cy = atof(sphere->first_attribute("cy")->value());
                        cz = atof(sphere->first_attribute("cz")->value());
                        r = atof(sphere->first_attribute("r")->value());

                        if (sphere->first_attribute("red")) {
                            red = atof(sphere->first_attribute("red")->value());
                            green = atof(sphere->first_attribute("green")->value());
                            blue = atof(sphere->first_attribute("blue")->value());
                        }
                        if (sphere->first_attribute("ior")) {
                            float ior = atof(sphere->first_attribute("ior")->value());
                            if (sphere->first_attribute("texture")) {
                                texture = sphere->first_attribute("texture")->value();
                                if (strcmp(texture, "checkers") == 0) {
                                    Texture* texture = new CheckersTexture();
                                    objects.emplace_back(std::make_unique<Sphere>(Sphere(vec3(cx,cy,cz), r, texture, true, ior)));
                                    n_obj++;
                                }
                            } else  {
                                objects.emplace_back(std::make_unique<Sphere>(Sphere(vec3(cx,cy,cz), r, vec3(red,blue,green), true, ior)));
                                n_obj++;
                            }
                        } else {
                            if (sphere->first_attribute("texture")) {
                                texture = sphere->first_attribute("texture")->value();
                                if (strcmp(texture, "checkers") == 0) {
                                    Texture* texture = new CheckersTexture();
                                    objects.emplace_back(std::make_unique<Sphere>(Sphere(vec3(cx,cy,cz), r, texture)));
                                    n_obj++;
                                }
                            } else {
                                objects.emplace_back(std::make_unique<Sphere>(Sphere(vec3(cx,cy,cz), r, vec3(red,green,blue))));
                                n_obj++;
                            }
                        }

                        
                    }
                }
                

                rapidxml::xml_node<> * planes_root = objects_root->first_node("Planes");
                if (planes_root) {
                    for (rapidxml::xml_node<> * plane = planes_root->first_node("Plane"); plane; plane = plane->next_sibling()) {
                        std::cout << "Loading object" << " " << n_obj + 1 << " : Plane.." << std::endl;
                        float px, py, pz;
                        float nx, ny, nz;
                        int red, green, blue;
                        char* texture;
                        px = atof(plane->first_attribute("px")->value());
                        py = atof(plane->first_attribute("py")->value());
                        pz = atof(plane->first_attribute("pz")->value());
                        nx = atof(plane->first_attribute("nx")->value());
                        ny = atof(plane->first_attribute("ny")->value());
                        nz = atof(plane->first_attribute("nz")->value());

                        if (plane->first_attribute("red")) {
                            red = atof(plane->first_attribute("red")->value());
                            green = atof(plane->first_attribute("green")->value());
                            blue = atof(plane->first_attribute("blue")->value());
                        }
                        if (plane->first_attribute("ior")) {
                            float ior = atof(plane->first_attribute("ior")->value());
                            if (plane->first_attribute("texture")) {
                                texture = plane->first_attribute("texture")->value();
                                if (strcmp(texture, "checkers") == 0) {
                                    Texture* texture = new CheckersTexture();
                                    objects.emplace_back(std::make_unique<Plane>(Plane(vec3(px,py,pz), vec3(nx,ny,nz), texture, true, ior)));
                                    n_obj++;
                                }
                            } else {
                                objects.emplace_back(std::make_unique<Plane>(Plane(vec3(px,py,pz), vec3(nx,ny,nz), vec3(red,green,blue), true, ior)));
                                n_obj++;
                            }
                        } else {
                            if (plane->first_attribute("texture")) {
                                texture = plane->first_attribute("texture")->value();
                                if (strcmp(texture, "checkers") == 0) {
                                    Texture* texture = new CheckersTexture();
                                    objects.emplace_back(std::make_unique<Plane>(Plane(vec3(px,py,pz), vec3(nx,ny,nz), texture)));
                                    n_obj++;
                                }
                            } else {
                                objects.emplace_back(std::make_unique<Plane>(Plane(vec3(px,py,pz), vec3(nx,ny,nz), vec3(red,green,blue))));
                                n_obj++;
                            }
                        }
                    } 
                }

                rapidxml::xml_node<> * objFiles_root = objects_root->first_node("obj-files");
                if (objFiles_root) {
                    for (rapidxml::xml_node<> * objFile = objFiles_root->first_node("obj-file"); objFile; objFile = objFile->next_sibling()) {
                        std::cout << "Loading object" << " " << n_obj + 1 << " from obj file.." << std::endl;
                        objects.emplace_back(std::make_unique<TriangleMesh>(objFile->value()));
                        n_obj++;
                    }
                }

                rapidxml::xml_node<> * proceduralMeshes_root = objects_root->first_node("Procedural-meshes");
                if (proceduralMeshes_root) {
                    rapidxml::xml_node<> * sphereMeshes_root = proceduralMeshes_root->first_node("Sphere-meshes");
                    if (sphereMeshes_root) {
                        for (rapidxml::xml_node<> * sphereMesh = sphereMeshes_root->first_node("Mesh"); sphereMesh; sphereMesh = sphereMesh->next_sibling()) {
                            std::cout << "Loading object" << " " << n_obj + 1 << " : Procedural Sphere.." << std::endl;
                            float cx, cy, cz, r;
                            int divs;
                            cx = atof(sphereMesh->first_attribute("cx")->value());
                            cy = atof(sphereMesh->first_attribute("cy")->value());
                            cz = atof(sphereMesh->first_attribute("cz")->value());
                            r = atof(sphereMesh->first_attribute("r")->value());
                            divs = atoi(sphereMesh->first_attribute("divs")->value());
                            TriangleMesh *mesh = TriangleMesh::generatePolySphere(vec3(cx,cy,cz),r,divs);
                            objects.push_back(std::unique_ptr<Object>(mesh));
                            n_obj++;
                        }
                    }
                }
            }
            
            // Load lights
            if (lights_root) {
                rapidxml::xml_node<> * pLights_root = lights_root->first_node("PointLights");
                if (pLights_root) {
                    for (rapidxml::xml_node<> * light = pLights_root->first_node("PointLight"); light; light = light->next_sibling()) {
                        std::cout << "Loading light" << " " << n_lig + 1 << " : Point Light.." << std::endl;
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
                        std::cout << "Loading light " << n_lig + 1 << " : Directional Light.." << std::endl;
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


        static void getConfig(std::string configfilename, int& depth, float& epsilon, int& sWidth, int& sHeight, int& antiAliasingSample, float& ambientLevel, int& numThreads) {
            rapidxml::xml_document<> doc;
            rapidxml::xml_node<> * root_node;

            std::ifstream theFile(configfilename);
            std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
            buffer.push_back('\0');
                    
            doc.parse<0>(&buffer[0]);

            root_node = doc.first_node("config");
            rapidxml::xml_node<> * depth_node = root_node->first_node("depth");
            rapidxml::xml_node<> * epsilon_node = root_node->first_node("epsilon");
            rapidxml::xml_node<> * sWidth_node = root_node->first_node("screen-width");
            rapidxml::xml_node<> * sHeight_node = root_node->first_node("screen-height");
            rapidxml::xml_node<> * antiAliSample_node = root_node->first_node("anti-aliasing-sample");
            rapidxml::xml_node<> * ambientLevel_node = root_node->first_node("ambient-level");
            rapidxml::xml_node<> * numThreads_node = root_node->first_node("num-threads");
            

            if (depth_node) {
                depth = atoi(depth_node->value());
            } else {
                depth = 3;
            }

            if (epsilon_node) {
                epsilon = atof(epsilon_node->value());
            } else {
                epsilon = 0.001;
            }

            if (sWidth_node) {
                sWidth = atoi(sWidth_node->value());
            } else {
                sWidth = 640;
            }

            if (sHeight_node) {
                sHeight = atoi(sHeight_node->value());
            } else {
                sHeight = 640;
            }

            if (antiAliSample_node) {
                antiAliasingSample = atoi(antiAliSample_node->value());
            } else {
                antiAliasingSample = 3;
            }

            if (ambientLevel_node) {
                ambientLevel = atof(ambientLevel_node->value());
            } else {
                ambientLevel = 0.005;
            }

            if (numThreads_node) {
                numThreads = atoi(numThreads_node->value());
            } else {
                numThreads = 4;
            }


        }
};

#endif