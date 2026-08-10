#ifndef BDXPIPELINE_H
#define BDXPIPELINE_H

// BasedDX
#include "stdafx.h"
#include "BDXHelper.h"

using Microsoft::WRL::ComPtr;

class BDXPipeline
{
public:

    BDXPipeline();
    ~BDXPipeline();

private:

    ComPtr<ID3D12Device> device;

    bool useWarpDevice = false;

    void loadPipeline();

    _Use_decl_annotations_
    void getHardwareAdapter(IDXGIFactory1* pFactory, IDXGIAdapter1** ppAdapter, bool requestHighPerformanceAdapter = false);

};


#endif // BDXPIPELINE_H
