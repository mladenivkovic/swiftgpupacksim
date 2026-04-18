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



#if defined(SWIFT_PARTICLE_ACCESS_PART_STRUCT)

#if defined(SWIFT_LOOP_SPLIT_NONE) ||       \
    (defined(SWIFT_LOOP_SPLIT_BY_STRUCT) && \
     (defined(SPHENIX_AOS_PARTICLE) || defined(SPHENIX_UPSTREAM_PARTICLE)))

  /* If all particle data is in a single struct, than splitting loop by struct
   * is equivalent to not splitting the loop at all */

#include "gpu_part_pack_functions/gpu_part_pack_functions_part_struct_access_loop_split_none.h"

#elif defined(SWIFT_LOOP_SPLIT_BY_TYPE) ||       \
    (defined(SWIFT_LOOP_SPLIT_BY_STRUCT_AND_TYPE) && \
     (defined(SPHENIX_AOS_PARTICLE) || defined(SPHENIX_UPSTREAM_PARTICLE)))

  /* If all particle data is in a single struct, than splitting loop by struct
   * is equivalent to not splitting the loop at all */

#include "gpu_part_pack_functions/gpu_part_pack_functions_part_struct_access_loop_split_by_type.h"


#elif defined(SWIFT_LOOP_SPLIT_BY_STRUCT) &&    \
    (defined(SPHENIX_PACK_GRADIENT_PARTICLE) || \
     defined(SPHENIX_PACK_FORCE_PARTICLE) ||    \
     defined(SPHENIX_PACK_SHARED_PARTICLE))

  /* Only use this for particle data memory layouts where particle is split
   * into several sub-structs, but not fully SoA */

#include "gpu_part_pack_functions/gpu_part_pack_functions_part_struct_access_loop_split_by_struct.h"

#elif defined(SWIFT_LOOP_SPLIT_BY_STRUCT_AND_TYPE) &&    \
    (defined(SPHENIX_PACK_GRADIENT_PARTICLE) || \
     defined(SPHENIX_PACK_FORCE_PARTICLE) ||    \
     defined(SPHENIX_PACK_SHARED_PARTICLE))

  /* Only use this for particle data memory layouts where particle is split
   * into several sub-structs, but not fully SoA */

#include "gpu_part_pack_functions/gpu_part_pack_functions_part_struct_access_loop_split_by_struct_and_type.h"


#elif defined(SWIFT_LOOP_SPLIT_BY_ELEMENT) || \
    (defined(SWIFT_LOOP_SPLIT_BY_STRUCT) && defined(SPHENIX_SOA_PARTICLE))

  /* Splitting SoA "by struct" is equivalent to SoA access, i.e. loops split by
   * element. */

#include "gpu_part_pack_functions/gpu_part_pack_functions_part_struct_access_loop_split_by_element.h"

#else
#pragma error "UNKNOWN CONFIGURATION OF PARTICLE + LOOP SPLIT"
#endif /* Loop splitting variants */


#elif defined(SWIFT_PARTICLE_ACCESS_GLOBAL_VAR)

#if defined(SWIFT_LOOP_SPLIT_NONE) ||       \
    (defined(SWIFT_LOOP_SPLIT_BY_STRUCT) && \
     (defined(SPHENIX_AOS_PARTICLE) || defined(SPHENIX_UPSTREAM_PARTICLE)))

  /* If all particle data is in a single struct, than splitting loop by struct
   * is equivalent to not splitting the loop at all */

#include "gpu_part_pack_functions/gpu_part_pack_functions_global_var_access_loop_split_none.h"
#if defined(SWIFT_LOOP_SPLIT_BY_TYPE) ||       \
    (defined(SWIFT_LOOP_SPLIT_BY_STRUCT_AND_TYPE) && \
     (defined(SPHENIX_AOS_PARTICLE) || defined(SPHENIX_UPSTREAM_PARTICLE)))

  /* If all particle data is in a single struct, than splitting loop by struct
   * is equivalent to not splitting the loop at all */

#include "gpu_part_pack_functions/gpu_part_pack_functions_global_var_access_loop_split_by_type.h"


#elif defined(SWIFT_LOOP_SPLIT_BY_STRUCT) &&    \
    (defined(SPHENIX_PACK_GRADIENT_PARTICLE) || \
     defined(SPHENIX_PACK_FORCE_PARTICLE) ||    \
     defined(SPHENIX_PACK_SHARED_PARTICLE))

  /* Only use this for particle data memory layouts where particle is split
   * into several sub-structs, but not fully SoA */

#include "gpu_part_pack_functions/gpu_part_pack_functions_global_var_access_loop_split_by_struct.h"

#elif defined(SWIFT_LOOP_SPLIT_BY_STRUCT_AND_TYPE) &&    \
    (defined(SPHENIX_PACK_GRADIENT_PARTICLE) || \
     defined(SPHENIX_PACK_FORCE_PARTICLE) ||    \
     defined(SPHENIX_PACK_SHARED_PARTICLE))

  /* Only use this for particle data memory layouts where particle is split
   * into several sub-structs, but not fully SoA */

#include "gpu_part_pack_functions/gpu_part_pack_functions_global_var_access_loop_split_by_struct_and_type.h"

#elif defined(SWIFT_LOOP_SPLIT_BY_ELEMENT) || \
    (defined(SWIFT_LOOP_SPLIT_BY_STRUCT) && defined(SPHENIX_SOA_PARTICLE))

  /* Splitting SoA "by struct" is equivalent to SoA access, i.e. loops split by
   * element. */

#include "gpu_part_pack_functions/gpu_part_pack_functions_global_var_access_loop_split_by_element.h"

#else
#pragma error "UNKNOWN CONFIGURATION OF PARTICLE + LOOP SPLIT"
#endif /* Loop splitting variants */



#elif defined(SWIFT_PARTICLE_ACCESS_EXPLICIT_VAR)

#if defined(SWIFT_LOOP_SPLIT_NONE) ||       \
    (defined(SWIFT_LOOP_SPLIT_BY_STRUCT) && \
     (defined(SPHENIX_AOS_PARTICLE) || defined(SPHENIX_UPSTREAM_PARTICLE)))

  /* If all particle data is in a single struct, than splitting loop by struct
   * is equivalent to not splitting the loop at all */

#include "gpu_part_pack_functions/gpu_part_pack_functions_explicit_var_access_loop_split_none.h"

#elif defined(SWIFT_LOOP_SPLIT_BY_TYPE) ||       \
    (defined(SWIFT_LOOP_SPLIT_BY_STRUCT_AND_TYPE) && \
     (defined(SPHENIX_AOS_PARTICLE) || defined(SPHENIX_UPSTREAM_PARTICLE)))

  /* If all particle data is in a single struct, than splitting loop by struct
   * is equivalent to not splitting the loop at all */

#include "gpu_part_pack_functions/gpu_part_pack_functions_explicit_var_access_loop_split_by_type.h"


#elif defined(SWIFT_LOOP_SPLIT_BY_STRUCT) &&    \
    (defined(SPHENIX_PACK_GRADIENT_PARTICLE) || \
     defined(SPHENIX_PACK_FORCE_PARTICLE) ||    \
     defined(SPHENIX_PACK_SHARED_PARTICLE))

  /* Only use this for particle data memory layouts where particle is split
   * into several sub-structs, but not fully SoA */

#include "gpu_part_pack_functions/gpu_part_pack_functions_explicit_var_access_loop_split_by_struct.h"

#elif defined(SWIFT_LOOP_SPLIT_BY_STRUCT_AND_TYPE) &&    \
    (defined(SPHENIX_PACK_GRADIENT_PARTICLE) || \
     defined(SPHENIX_PACK_FORCE_PARTICLE) ||    \
     defined(SPHENIX_PACK_SHARED_PARTICLE))

  /* Only use this for particle data memory layouts where particle is split
   * into several sub-structs, but not fully SoA */

#include "gpu_part_pack_functions/gpu_part_pack_functions_explicit_var_access_loop_split_by_struct_and_type.h"


#elif defined(SWIFT_LOOP_SPLIT_BY_ELEMENT) || \
    (defined(SWIFT_LOOP_SPLIT_BY_STRUCT) && defined(SPHENIX_SOA_PARTICLE))

  /* Splitting SoA "by struct" is equivalent to SoA access, i.e. loops split by
   * element. */

#include "gpu_part_pack_functions/gpu_part_pack_functions_explicit_var_access_loop_split_by_element.h"

#else
#pragma error "UNKNOWN CONFIGURATION OF PARTICLE + LOOP SPLIT"
#endif /* Loop splitting variants */


#else
#pragma error "Unknown particle access layout realisation"
#endif
