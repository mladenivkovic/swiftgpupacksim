/*******************************************************************************
 * This file is part of SWIFT.
 * Copyright (c) 2026 Mladen Ivkovic (mladen.ivkovic@durham.ac.uk)
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
#ifndef SWIFT_HYDRO_PART_ARRAYS_FLUSH_H
#define SWIFT_HYDRO_PART_ARRAYS_FLUSH_H

/**
 * @file src/hydro_part_arrays_flush.h
 * @brief Top level file for hydro particle struct data flushing. Imports the
 * correct hydro_part_arrays_flush definition.
 */

/* Config parameters. */
#include "../config.h"

#if defined(SPHENIX_AOS_PARTICLE)
#include "realisations/aos/hydro_part_arrays_flush.h"
#elif defined(SPHENIX_SOA_PARTICLE)
#include "realisations/soa/hydro_part_arrays_flush.h"
#elif defined(SPHENIX_UPSTREAM_PARTICLE)
#include "realisations/upstream/hydro_part_arrays_flush.h"
#elif defined(SPHENIX_RANDOM_ORDER_PARTICLE)
#include "realisations/random_order/hydro_part_arrays_flush.h"
#elif defined(SPHENIX_PACK_GRADIENT_PARTICLE)
#include "realisations/pack_gradient/hydro_part_arrays_flush.h"
#elif defined(SPHENIX_PACK_GRADIENT_TYPE_PARTICLE)
#include "realisations/pack_gradient_type/hydro_part_arrays_flush.h"
#elif defined(SPHENIX_PACK_FORCE_PARTICLE)
#include "realisations/pack_force/hydro_part_arrays_flush.h"
#elif defined(SPHENIX_PACK_FORCE_TYPE_PARTICLE)
#include "realisations/pack_force_type/hydro_part_arrays_flush.h"
#elif defined(SPHENIX_PACK_SHARED_PARTICLE)
#include "realisations/pack_shared/hydro_part_arrays_flush.h"
#elif defined(SPHENIX_PACK_SHARED_TYPE_PARTICLE)
#include "realisations/pack_shared_type/hydro_part_arrays_flush.h"
#else
#error "Unknown memory layout realisation"
#endif


#endif /* ifdef SWIFT_HYDRO_PART_ARRAYS_FLUSH_H */
