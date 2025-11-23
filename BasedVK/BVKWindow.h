#ifndef BVKWINDOW_H
#define BVKWINDOW_H

// BasedCore
#include "../BasedCore/BEWindow.h"


class BVKWindow final : public BEWindow
{
public:

    BVKWindow(int w, int h, std::string title);

private:

    void frameBufferResizedFunc(int width, int height) override;
};


#endif // BVKWINDOW_H