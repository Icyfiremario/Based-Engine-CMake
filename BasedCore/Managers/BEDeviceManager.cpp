#include "BEDeviceManager.h"

BEDeviceManager* BEDeviceManager::m_instance = nullptr;
std::mutex BEDeviceManager::mtx;

BEDeviceManager* BEDeviceManager::getInstance()
{
    if (m_instance == nullptr)
    {
        std::lock_guard lock(mtx);
        if (m_instance == nullptr)
        {
            m_instance = new BEDeviceManager();
        }
    }

    return m_instance;
}

BEDeviceManager::BEDeviceManager()
{
    discoverDevices();
}

void BEDeviceManager::discoverDevices()
{

}
