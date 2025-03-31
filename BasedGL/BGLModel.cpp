#include "BGLModel.h"

void BGLModel::bind(uint32_t bufferType, uint32_t& buffer, const void* data, uint32_t flags, size_t size)
{
    glBindBuffer(bufferType, buffer);
    glBufferData(bufferType, size, data, flags);
}