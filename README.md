## problèmes :
- primary ray qui déforment les objets quand ils sont pas au centre



## TODO:
- Bouger la logique de load dans sa propre classe          -- OK 
- Régler le load des textures et des normals               -- OK (si normals donnés sur les verts et pas les faces)
- Utiliser N et st dans l'algo                             -- OK
- Clairement définir la transparence etc                   -- OK ( faire le cas seulement refrac )
- Destructeurs                                             -- OK
- Optimiser (changer en pointeurs? etc)                    -- OK
- Revoir les couleurs des lights                           -- OK
- Directional light?                                       -- OK
- Parser: gérer les cas sans objet                         -- OK
- Mettre les globals dans une classe                       -- OK
- Essayer de faire marcher un .obj (Zeus?)                 
- Timer                                                    -- OK
- Bouger l'objet?
- Faire une classe vec2?                                   - X
- Retirer les commentaires de test
- Problème de diffraction
- Revoir tout le main pour bien mettre les variables 
- Texture sur objets de base                               
- Config.xml


## Transparence.
albedo
Ks
spec_n

kr (fresnel)  ---- kr*mirrorRay + (1-kr)*refractedRay
https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel
Kr est calculé en fonction de la normale et représente le taux de lumiere qui est refracté vs relecté

Ks = coeff de specular 
spec_n = shinyness
albdedo = coeff de diffuse 
Ka = coeff ambient (ON A AMBIENTLEVEL QUI EST INDEPENDANT DES OBJETS)
k_mirror = fraction réflectée par un objet reflecteur (~= 0.8)