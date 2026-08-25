#ifndef BEAPP_H
#define BEAPP_H

// STD
#include <memory>
#include <string>

class BEApp
{
public:

    BEApp() = default;
    virtual ~BEApp() = default;

    virtual void run() = 0;

private:
};


#endif //BEAPP_H