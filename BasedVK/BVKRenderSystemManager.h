#ifndef BVKRENDERSYSTEMMANAGER_H
#define BVKRENDERSYSTEMMANAGER_H

// STD
#include <vector>
#include <string>

class BVKRenderSystemManager
{
public:
    BVKRenderSystemManager();
    ~BVKRenderSystemManager();

private:

    std::vector<int> renderSystems;
};


#endif //BVKRENDERSYSTEMMANAGER_H
