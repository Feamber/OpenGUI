
#pragma once
#include "OpenGUI/Configure.h"
// This file needs to be included first for all MSDFgen sources

#define MSDFGEN_PUBLIC OGUI_API
#ifndef MSDFGEN_PUBLIC
#include <msdfgen/msdfgen-config.h>
#endif

#include <cstddef>

namespace msdfgen {

typedef unsigned char byte;

}
