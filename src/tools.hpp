#ifndef TOOLS_H
#define TOOLS_H

#include <math.h>
#include <algorithm>
// #include "light.hpp"
// #include "sphere.hpp"
// #include "plane.hpp"



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

        // static void parseObjectsAndLights(std::string filename,
        //         std::vector<std::unique_ptr<Object>>& objects,
        //         std::vector<std::unique_ptr<Light>>& lights,
        //         int& n_obj, int& n_lig) 
        // {
        //     rapidxml::xml_document<> doc;
        //     rapidxml::xml_node<> * root_node;

        //     std::ifstream theFile(filename);
        //     std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
        //     buffer.push_back('\0');

        //     std::cout << &buffer[0] <<std::endl;
            
        //     doc.parse<0>(&buffer[0]);


        //     root_node = doc.first_node("World");
        //     rapidxml::xml_node<> * objects_root = root_node->first_node("Objects");
        //     rapidxml::xml_node<> * lights_root = root_node->first_node("Lights");


        //     // Load objects
        //     rapidxml::xml_node<> * spheres_root = objects_root->first_node("Spheres");
        //     for (rapidxml::xml_node<> * sphere = spheres_root->first_node("Sphere"); sphere; sphere = sphere->next_sibling()) {
        //         std::cout << "Sphere" << " " << n_obj << std::endl;
        //         float cx, cy, cz, r;
        //         cx = atof(sphere->first_attribute("cx")->value());
        //         cy = atof(sphere->first_attribute("cy")->value());
        //         cz = atof(sphere->first_attribute("cz")->value());
        //         r = atof(sphere->first_attribute("r")->value());
        //         objects.emplace_back(std::make_unique<Sphere>(Sphere(vec3(cx,cy,cz), r, vec3(123,20,50))));
        //         n_obj++;
        //     }

        //     rapidxml::xml_node<> * planes_root = objects_root->first_node("Planes");
        //     for (rapidxml::xml_node<> * plane = planes_root->first_node("Plane"); plane; plane = plane->next_sibling()) {
        //         std::cout << "Plane" << " " << n_obj << std::endl;
        //         float px, py, pz;
        //         float nx, ny, nz;
        //         px = atof(plane->first_attribute("px")->value());
        //         py = atof(plane->first_attribute("py")->value());
        //         pz = atof(plane->first_attribute("pz")->value());
        //         nx = atof(plane->first_attribute("nx")->value());
        //         ny = atof(plane->first_attribute("ny")->value());
        //         nz = atof(plane->first_attribute("nz")->value());
        //         objects.emplace_back(std::make_unique<Plane>(Plane(vec3(px,py,pz),vec3(nx,ny,nz), vec3(50,0,50))));
        //         n_obj++;
        //     }

        //     // Load lights
        //     rapidxml::xml_node<> * pLights_root = lights_root->first_node("PointLights");
        //     for (rapidxml::xml_node<> * light = pLights_root->first_node("PointLight"); light; light = light->next_sibling()) {
        //         std::cout << "PointLight" << " " << n_lig << std::endl;
        //         float px, py, pz;
        //         px = atof(light->first_attribute("px")->value());
        //         py = atof(light->first_attribute("py")->value());
        //         pz = atof(light->first_attribute("pz")->value());
        //         lights.emplace_back(std::make_unique<PointLight>(PointLight(mat4(), vec3(px,py,pz))));
        //         n_lig++;
        //     }
        // }
};

#endif