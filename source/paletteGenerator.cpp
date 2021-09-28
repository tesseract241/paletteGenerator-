#include <cassert>
#include <cmath>
#include <random>
#include <vector>
#include <paletteGenerator.hpp>

void coordinatesFromIndex(const int index, const int edge, int coordinates[]){
    assert(coordinates && "coordinatesFromIndex called with uninitialized coordinates\n");
    coordinates[0] = index/(edge*edge);
    int dummy = index - coordinates[0]*edge*edge;
    coordinates[1] = dummy/edge;
    coordinates[2] = dummy - coordinates[1]*edge;
}

void rotateColourspace(const int x, const int y, const int z, int outColours[]){
    outColours[0] = (x + z)/2;
    outColours[1] = (y + z)/2;
    outColours[2] = (x + y)/2;
}

int findEdge(int numberOfColours){
    float delta = std::cbrt(std::sqrt(27*27*numberOfColours*numberOfColours + 27*4*numberOfColours) + (27*numberOfColours + 2)/2.);
    return int(std::ceil((delta + 1/delta + 1.)/3.));
}

void generatePalette(int numberOfColours, uint8_t *colours){
    assert(numberOfColours>0 && "generatePalette called with non-positive numberOfColours");
    assert(colours && "generatePalette called with uninitialized colors\n");
    int nodesPerSide = findEdge(numberOfColours);
    int skipThreshold = int(std::ceil(nodesPerSide/6.));
    int nodeSize = 255/(nodesPerSide - 1);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<int> randomColoursVector(nodesPerSide*nodesPerSide*nodesPerSide);
    for(int i=0;i<nodesPerSide*nodesPerSide*nodesPerSide;++i){
        randomColoursVector[i] = i;
    }
    int randomColours[nodesPerSide*nodesPerSide*nodesPerSide];
    for(int i=0;i<nodesPerSide*nodesPerSide*nodesPerSide;++i){
        std::uniform_int_distribution<> distrib(0, randomColoursVector.size() - 1);
        int index = distrib(gen);
        randomColours[i] = randomColoursVector[index];
        randomColoursVector.erase(randomColoursVector.begin() + index);
    }
    int j=0;
    for(int i=0;i<numberOfColours;++i){
        int indices[3];
        for( ; ;++j){
            coordinatesFromIndex(randomColours[j], nodesPerSide, indices);
            if(
                    std::abs(indices[0]-indices[1]>= skipThreshold) ||
                    std::abs(indices[1]-indices[2]>= skipThreshold) ||
                    std::abs(indices[0]-indices[2]>= skipThreshold)
            ){
                ++j;
                break;
            }
        }
        rotateColourspace(indices[0]*nodeSize, indices[1]*nodeSize, indices[2]*nodeSize, colours + 3*i);
    }
}
