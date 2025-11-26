/*******************************************************************************
 * This file is part of SWIFT.
 * Copyright (c) 2016 Matthieu Schaller (schaller@strw.leidenuniv.nl)
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
#ifndef SWIFT_GRAVITY_CACHE_H
#define SWIFT_GRAVITY_CACHE_H

/* Config parameters. */
#include "../../config.h"

/* Local headers */
#include "align.h"

/**
 * @brief A SoA object for the #gpart of a cell.
 *
 * This is used to help vectorize the leaf-leaf gravity interactions.
 */
struct gravity_cache {

  /*! #gpart x position. */
  float *restrict x SWIFT_CACHE_ALIGN;

  /*! #gpart y position. */
  float *restrict y SWIFT_CACHE_ALIGN;

  /*! #gpart z position. */
  float *restrict z SWIFT_CACHE_ALIGN;

  /*! #gpart softening length. */
  float *restrict epsilon SWIFT_CACHE_ALIGN;

  /*! #gpart mass. */
  float *restrict m SWIFT_CACHE_ALIGN;

  /*! #gpart x acceleration. */
  float *restrict a_x SWIFT_CACHE_ALIGN;

  /*! #gpart y acceleration. */
  float *restrict a_y SWIFT_CACHE_ALIGN;

  /*! #gpart z acceleration. */
  float *restrict a_z SWIFT_CACHE_ALIGN;

  /*! #gpart potential. */
  float *restrict pot SWIFT_CACHE_ALIGN;

  /*! Is this #gpart active ? */
  int *restrict active SWIFT_CACHE_ALIGN;

  /*! Can this #gpart use a M2P interaction ? */
  int *restrict use_mpole SWIFT_CACHE_ALIGN;

  /*! Cache size */
  int count;
};

#endif
