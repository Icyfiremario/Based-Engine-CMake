#ifndef BDXPIPELINE_H
#define BDXPIPELINE_H

// BasedDX
#include "stdafx.h"

using Microsoft::WRL::ComPtr;

class BDXPipeline
{
public:

    BDXPipeline();
    ~BDXPipeline();

private:

    ComPtr<ID3D12Device> device;

    void loadPipeline();

};


#endif // BDXPIPELINE_H
