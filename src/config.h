/**
 * Selects the correct config.h file from the subdir.
 */

#pragma once

#if defined CONFIG_LOCAL_HP_GCC
#include "config/config_local_HP.h"
#elif defined CONFIG_COSMA_GN001_GCC
#include "config/config_cosma_gn001.h"
#endif
