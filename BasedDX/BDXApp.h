#ifndef BDXAPP_H
#define BDXAPP_H

// STD
#include <memory>

// BasedCore
#include "../BasedCore/Common/BEApp.h"

// BasedDX
#include "BDXDevice.h"


class BDXApp : public BEApp
{
public:
    BDXApp();
    BDXApp(int width, int height, const char* title);
    ~BDXApp() override;

    void run() override;

private:

    std::unique_ptr<BDXDevice> appDevice = nullptr;
};


#endif // BDXAPP_H
