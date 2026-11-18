/*******************************************************************************
 * This file is part of SWIFT.
 * Copyright (c) 2016   Matthieu Schaller (schaller@strw.leidenuniv.nl).
 *               2018   Jacob Kegerreis (jacob.kegerreis@durham.ac.uk).
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
#ifndef SWIFT_PLANETARY_EQUATION_OF_STATE_H
#define SWIFT_PLANETARY_EQUATION_OF_STATE_H

/**
 * @file equation_of_state/planetary/equation_of_state.h
 *
 * For any/all of the planetary EOS. Each EOS type's functions are set in its
 * own header file: `equation_of_state/planetary/<eos_type>.h`.
 * See `eos_planetary_material_id` for the available choices.
 *
 * Not all functions are implemented for all EOS types, so not all can be used
 * with all hydro formulations yet.
 */

/* Some standard headers. */
#include <math.h>

/* Local headers. */
/* #include "adiabatic_index.h" */
/* #include "common_io.h" */
#include "inline.h"
/* #include "physical_constants.h" */
/* #include "restart.h" */
/* #include "units.h" */

extern struct eos_parameters eos;

/*! Material identifier flags (material_ID = type_ID * type_factor + unit_ID) */
#define eos_planetary_type_factor 100

/**
 * @brief Master type for the planetary equation of state.
 */
enum eos_planetary_type_id {

  /*! Ideal gas */
  eos_planetary_type_idg = 0,

  /*! Tillotson */
  eos_planetary_type_Til = 1,

  /*! Hubbard & MacFarlane (1980) Uranus/Neptune */
  eos_planetary_type_HM80 = 2,

  /*! SESAME */
  eos_planetary_type_SESAME = 3,

  /*! ANEOS */
  eos_planetary_type_ANEOS = 4,

  /*! Custom */
  eos_planetary_type_custom = 9,
};

/**
 * @brief Minor type for the planetary equation of state.
 */
enum eos_planetary_material_id {

  /* Ideal gas */

  /*! Default
   * (adiabatic index set at configure time by --with-adiabatic-index, default
   *  value is γ=5/3)
   */
  eos_planetary_id_idg_def = eos_planetary_type_idg * eos_planetary_type_factor,

  /* Tillotson */

  /*! Tillotson iron */
  eos_planetary_id_Til_iron =
      eos_planetary_type_Til * eos_planetary_type_factor,

  /*! Tillotson granite */
  eos_planetary_id_Til_granite =
      eos_planetary_type_Til * eos_planetary_type_factor + 1,

  /*! Tillotson water */
  eos_planetary_id_Til_water =
      eos_planetary_type_Til * eos_planetary_type_factor + 2,

  /*! Tillotson basalt */
  eos_planetary_id_Til_basalt =
      eos_planetary_type_Til * eos_planetary_type_factor + 3,

  /*! Tillotson ice */
  eos_planetary_id_Til_ice =
      eos_planetary_type_Til * eos_planetary_type_factor + 4,

  /* Hubbard & MacFarlane (1980) Uranus/Neptune */

  /*! Hydrogen-helium atmosphere */
  eos_planetary_id_HM80_HHe =
      eos_planetary_type_HM80 * eos_planetary_type_factor,

  /*! H20-CH4-NH3 ice mix */
  eos_planetary_id_HM80_ice =
      eos_planetary_type_HM80 * eos_planetary_type_factor + 1,

  /*! SiO2-MgO-FeS-FeO rock mix */
  eos_planetary_id_HM80_rock =
      eos_planetary_type_HM80 * eos_planetary_type_factor + 2,

  /* SESAME */

  /*! SESAME iron 2140 */
  eos_planetary_id_SESAME_iron =
      eos_planetary_type_SESAME * eos_planetary_type_factor,

  /*! SESAME basalt 7530 */
  eos_planetary_id_SESAME_basalt =
      eos_planetary_type_SESAME * eos_planetary_type_factor + 1,

  /*! SESAME water 7154 */
  eos_planetary_id_SESAME_water =
      eos_planetary_type_SESAME * eos_planetary_type_factor + 2,

  /*! Senft & Stewart (2008) SESAME-like water */
  eos_planetary_id_SS08_water =
      eos_planetary_type_SESAME * eos_planetary_type_factor + 3,

  /*! AQUA (Haldemann et al. 2020) SESAME-like water */
  eos_planetary_id_AQUA =
      eos_planetary_type_SESAME * eos_planetary_type_factor + 4,

  /*! CMS19 hydrogen (Chabrier et al. 2019) SESAME-like hydrogen */
  eos_planetary_id_CMS19_H =
      eos_planetary_type_SESAME * eos_planetary_type_factor + 5,

  /*! CMS19 helium (Chabrier et al. 2019) SESAME-like helium */
  eos_planetary_id_CMS19_He =
      eos_planetary_type_SESAME * eos_planetary_type_factor + 6,

  /*! CD21 hydrogen-helium (Chabrier & Debras 2021) SESAME-like H-He mixture
     (Y=0.245) */
  eos_planetary_id_CD21_HHe =
      eos_planetary_type_SESAME * eos_planetary_type_factor + 7,

  /* ANEOS */

  /*! ANEOS forsterite (Stewart et al. 2019) -- in SESAME-style tables */
  eos_planetary_id_ANEOS_forsterite =
      eos_planetary_type_ANEOS * eos_planetary_type_factor,

  /*! ANEOS iron (Stewart 2020) -- in SESAME-style tables */
  eos_planetary_id_ANEOS_iron =
      eos_planetary_type_ANEOS * eos_planetary_type_factor + 1,

  /*! ANEOS Fe85Si15 (Stewart 2020) -- in SESAME-style tables */
  eos_planetary_id_ANEOS_Fe85Si15 =
      eos_planetary_type_ANEOS * eos_planetary_type_factor + 2,
};

#endif
