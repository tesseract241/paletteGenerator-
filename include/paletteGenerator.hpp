#pragma once
#include <cstdint>

//This random palette generator is based on a perceptual model of color vision, which says that, while 
//the cones in our eyes are sensitive to specific frequency bands, our visual cortex codifies colours
//based on where they lie among three different axes, at the ends of which are certain color components.
//Specifically this program uses a Green-Magenta, Red-Cyan and Blue-Yellow choice of coordinates.
//Thus we generate colours that are as distant as possible in this choice of coordinates, specifically generating a 
//cubical crystal, the nodes of which are our colours. We skip the nodes on the diagonal, 
//as they are less visibile according to our model, and eventually convert back to an RGB color model for ease of use.

void generatePalette(int numberOfColours, float *colours);
