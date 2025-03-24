#pragma once

// STD
#include <vector>
#include <cassert>
#include <cstring>
#include <memory>
#include <unordered_map>

// GLM
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

// BasedVK
#include "BVKDevice.h"
#include "BVKUtils.h"
#include "BVKBuffer.h"

class BVKModel
{};