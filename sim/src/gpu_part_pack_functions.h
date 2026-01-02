/*******************************************************************************
 * This file is part of SWIFT.
 * Copyright (c) 2025 Abouzied M. A. Nasar (abouzied.nasar@manchester.ac.uk)
 *                    Mladen Ivkovic (mladen.ivkovic@durham.ac.uk)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/
#pragma once

#include "config.h"

/**
 * @file gpu_part_pack_functions.h
 * @brief Top level file to include appropriate part pack functions
 */

#ifdef VECTORIZE

#if defined(SPHENIX_AOS_PARTICLE)
#include "realisations/aos/gpu_part_pack_functions.h"
#elif defined(SPHENIX_SOA_PARTICLE)
#include "realisations/soa/gpu_part_pack_functions.h"
#elif defined(SPHENIX_UPSTREAM_PARTICLE)
#include "realisations/upstream/gpu_part_pack_functions.h"
#elif defined(SPHENIX_PACK_GRADIENT_PARTICLE)
#include "realisations/pack_gradient/gpu_part_pack_functions.h"
#elif defined(SPHENIX_PACK_FORCE_PARTICLE)
#include "realisations/pack_force/gpu_part_pack_functions.h"
#elif defined(SPHENIX_PACK_SHARED_PARTICLE)
#include "realisations/pack_shared/gpu_part_pack_functions.h"
#elif defined(SPHENIX_SOA_MANUAL_PARTICLE)
#include "realisations/soa_manual/gpu_part_pack_functions.h"
#else
#pragma error "Unknown memory layout realisation"
#endif


#else

#if defined(SPHENIX_SOA_MANUAL_PARTICLE)
#include "realisations/soa_manual/gpu_part_pack_functions.h"
#else
#include "swift_placeholders/cuda/gpu_part_pack_functions.h"
#endif

#endif /* defined VECTORIZE */


