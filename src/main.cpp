#include <iostream>
#include <vector>

#include "modelHandling.hpp"

int main() {
    Scene scene;
    int t;
    objImporter("model_multipleobjs.obj", scene);
    scene.GetObjectList();
    std::cout<<"\n\n\n";
    scene.GetObjectData();
    std::cin>>t;
} 
