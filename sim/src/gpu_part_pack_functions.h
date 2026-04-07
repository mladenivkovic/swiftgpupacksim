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
#include "gpu_part_pack_functions/gpu_part_pack_functions_part_struct_access.h"
#elif defined(SWIFT_PARTICLE_ACCESS_GLOBAL_VAR)
#include "gpu_part_pack_functions/gpu_part_pack_functions_global_var_access.h"
#elif defined(SWIFT_PARTICLE_ACCESS_EXPLICIT_VAR)
#include "gpu_part_pack_functions/gpu_part_pack_functions_explicit_var_access.h"
#else
#pragma error "Unknown particle access layout realisation"
#endif
