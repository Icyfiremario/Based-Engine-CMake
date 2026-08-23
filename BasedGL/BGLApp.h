#ifndef BGLAPP_H
#define BGLAPP_H

// STD
#include <memory>

// BasedCore
#include "../BasedCore/Common/BEApp.h"

// BasedGL
#include "BGLWindow.h"

class BGLApp : public BEApp
{
public:

    BGLApp();
    BGLApp(int w, int h, const char* title);
    ~BGLApp() override;

    void run() override;

private:

    std::unique_ptr<BGLWindow> appWindow;
};


#endif //BGLAPP_H
