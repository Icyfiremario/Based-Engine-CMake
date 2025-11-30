#ifndef BEDEVICEMANAGER_H
#define BEDEVICEMANAGER_H

// STD
#include <mutex>
#include <map>
#include <memory>

// BasedCore
#include "../Common/BEDevice.h"
#include "../BEUtils.h"


struct DeviceEntry_t
{
    std::unique_ptr<BasedEngine::Common::BEDevice> devicePtr;
    unsigned api = BasedEngine::API::VULKAN;
};

/// @brief Stores pointers to each available GPU
class BEDeviceManager
{
public:

    BEDeviceManager(const BEDeviceManager&) = delete;

    static BEDeviceManager* getInstance();

    [[nodiscard]] const std::map<unsigned, DeviceEntry_t>& getDeviceList() const { return devices; }

private:

    static BEDeviceManager *m_instance;
    static std::mutex mtx;

    std::map<unsigned, DeviceEntry_t> devices;

    std::unique_ptr<BasedEngine::Common::BEDevice> selectedDevice = nullptr;

    BEDeviceManager();

    void discoverDevices();
};

#endif // BEDEVICEMANAGER_H