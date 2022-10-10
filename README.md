# Graphics-project
These are some OpenGl graphics programs I wrote using the tutorials from learnopengl.com and using the test system 
from "The Cherno"'s (youtube channel) opengl series to compile them into a single gui to access them from a single menu. 
1. A primitive physics simulation with a bunch of circles in a bounding box with. Some of the circles are designated as light sources which makes it so that they cast shadows behind the rest of the circles.
This program uses instancing as well as a geometry shader and stencil testing to create the shadows.
![image](https://user-images.githubusercontent.com/115482242/194908546-793368c2-7f5d-472c-9cfa-56e1c54008f4.png)
2. Another primitive physics simulation, this time in 3D. As in the previous program some spheres are light sources and phong shading is used to draw the spheres. A function is used to generate the sphere mesh. Also a framebuffer is used for HDR and gamma correction.
![image](https://user-images.githubusercontent.com/115482242/194909237-f1a44108-cd30-4d45-bcb5-a792741966b9.png)
