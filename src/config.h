/**
 * Selects the correct config.h file from the subdir.
 */

#pragma once

#if defined CONFIG_LOCAL_HP_ICX
#include "config/config_local_HP_icx.h"
#elif defined CONFIG_LOCAL_HP_GCC
#include "config/config_local_HP_gcc.h"
#elif defined CONFIG_COSMA_GN001_GCC
#include "config/config_cosma_gn001.h"
#else
#error "Unknown compiler/machine config selected"
#endif

#ifndef HAVE_CUDA
#error "Something went wrong, says here I don't have cuda?"
#endif
