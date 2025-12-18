#pragma once

#include "../config.h"

#if defined(SPHENIX_AOS_PARTICLE)
#include "realisations/aos/parts.h"
#elif defined(SPHENIX_SOA_PARTICLE)
#include "realisations/soa/parts.h"
#elif defined(SPHENIX_UPSTREAM_PARTICLE)
#include "realisations/upstream/parts.h"
#else
#pragma error "Unknown memory layout realisation"
#endif
