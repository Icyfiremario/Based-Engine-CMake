#ifndef BEAPP_H
#define BEAPP_H

// STD
#include <string>

class BEApp
{
public:

    BEApp() = default;
    virtual ~BEApp() = default;

    virtual void run() = 0;

private:

    int m_width = 0, m_height = 0;
    std::string m_title;
};


#endif //BEAPP_H