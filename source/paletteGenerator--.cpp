#include <cassert>
#include <cmath>
#include <random>
#include <vector>
#include <paletteGenerator--.hpp>

void coordinatesFromIndex(const int index, const int edge, int coordinates[]){
    assert(coordinates && "coordinatesFromIndex called with uninitialized coordinates\n");
    coordinates[0] = index/(edge*edge);
    int dummy = index - coordinates[0]*edge*edge;
    coordinates[1] = dummy/edge;
    coordinates[2] = dummy - coordinates[1]*edge;
}

void rotateColorSpace(const float inColors[], float outColors[]){
    outColors[0] = (inColors[0] + inColors[2])/2.;
    outColors[1] = (inColors[1] + inColors[2])/2.;
    outColors[2] = (inColors[0] + inColors[1])/2.;
}

int findEdge(int numberOfColors){
    float delta = std::cbrt(std::sqrt(27*27*numberOfColors*numberOfColors + 27*4*numberOfColors) + (27*numberOfColors + 2)/2.);
    return int(std::ceil((delta + 1/delta + 1.)/3.));
}

void generatePalette(int numberOfColors, float colors[][3]){
    assert(numberOfColors>0 && "generatePalette called with non-positive numberOfColors");
    assert(colors && "generatePalette called with uninitialized colors\n");
    int nodesPerSide = findEdge(numberOfColors);
    int skipThreshold = int(std::ceil(nodesPerSide/6.));
    float nodeSize = 1./float(nodesPerSide - 1);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<int> randomColorsVector(nodesPerSide*nodesPerSide*nodesPerSide);
    for(int i=0;i<nodesPerSide*nodesPerSide*nodesPerSide;++i){
        randomColorsVector[i] = i;
    }
    int randomColors[nodesPerSide*nodesPerSide*nodesPerSide];
    for(int i=0;i<nodesPerSide*nodesPerSide*nodesPerSide;++i){
        std::uniform_int_distribution<> distrib(0, randomColorsVector.size() - 1);
        int index = distrib(gen);
        randomColors[i] = randomColorsVector[index];
        randomColorsVector.erase(randomColorsVector.begin() + index);
    }
    int j=0;
    for(int i=0;i<numberOfColors;++i){
        int indexes[3];
        for( ; ;++j){
            coordinatesFromIndex(randomColors[j], nodesPerSide, indexes);
            if(
                    std::abs(indexes[0]-indexes[1]>= skipThreshold) ||
                    std::abs(indexes[1]-indexes[2]>= skipThreshold) ||
                    std::abs(indexes[0]-indexes[2]>= skipThreshold)
            ){
                ++j;
                break;
            }
        }
        float fColors[3];
        for(int k=0;k<3;++k){
            fColors[k] = nodeSize * float(indexes[k]);
        }
        rotateColorSpace(fColors, colors[i]);
    }
}
