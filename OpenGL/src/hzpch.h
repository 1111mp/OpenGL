#pragma once

// How to add precompiled headers in CMake for visual studio generator
// https://stackoverflow.com/questions/62541168/how-to-add-precompiled-headers-in-cmake-for-visual-studio-generator

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Log.h"

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif