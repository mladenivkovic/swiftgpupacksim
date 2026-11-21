/* Minimal config.h for compilation tests to work. */

/* No black hole model */
#define BLACK_HOLES_NONE 1

/* No chemistry function */
#define CHEMISTRY_NONE 1

/* Use doubles in grackle */
#define CONFIG_BFLOAT_8 1

/* No cooling function */
#define COOLING_NONE 1

/* Cubic spline kernel */
#define CUBIC_SPLINE_KERNEL 1

/* No entropy floor */
#define ENTROPY_FLOOR_NONE 1

/* Ideal gas equation of state */
#define EOS_IDEAL_GAS 1

/* No external potential */
#define EXTERNAL_POTENTIAL_NONE 1

/* No extra_io function */
#define EXTRA_IO_NONE 1

/* No feedback */
#define FEEDBACK_NONE 1

/* Number of rays to use for the AGN feedback */
#define FEEDBACK_NR_RAYS_AGN 50

/* Number of rays to use for the SNII feedback */
#define FEEDBACK_NR_RAYS_SNII 1

/* Number of rays to use for the SNIa feedback */
#define FEEDBACK_NR_RAYS_SNIa 1

/* No adaptive softening */
#define FIXED_SOFTENING 1

/* No external forcing terms */
#define FORCING_NONE 1

/* Defined if you have HDF5 support */
#define HAVE_HDF5 1

/* Number of hydro cooling tasks per cell */
#define HYDRO_COOLING_NTASK 1

/* 3D solver */
#define HYDRO_DIMENSION_3D 3

/* Adiabatic index is 5/3 */
#define HYDRO_GAMMA_5_3 5./3.

/* Number of hydro density ghost tasks per cell */
#define HYDRO_GHOST_NTASK 1

/* Do symmetric MPI interactions */
#define MPI_SYMMETRIC_FORCE_INTERACTION_RT 1

/* Gravity scheme with per-particle type softening value and background
   particles */
#define MULTI_SOFTENING_GRAVITY 1

/* No mhd */
#define NONE_MHD 1

/* No pressure floor */
#define PRESSURE_FLOOR_NONE 1

/* No Riemann solver */
#define RIEMANN_SOLVER_NONE 1

/* No radiative transfer scheme */
#define RT_NONE 1

/* Multipole order */
#define SELF_GRAVITY_MULTIPOLE_ORDER 4

/* No sink particle model */
#define SINK_NONE 1

/* SPHENIX SPH */
/* #undef SPHENIX_SPH 1 */

/* Basic stellar model */
#define STARS_BASIC 1

/* Number of stars density ghost tasks per cell */
#define STARS_GHOST_NTASK 1

/* Number of stars density self tasks per cell */
#define STARS_SELF_NTASK 1

/* No star formation */
#define STAR_FORMATION_NONE 1

/* No tracers function */
#define TRACERS_NONE 1

/* Enable extensions on AIX, Interix, z/OS.  */
#ifndef _ALL_SOURCE
# define _ALL_SOURCE 1
#endif
/* Enable extensions on Cosmopolitan Libc. */
#ifndef _COSMO_SOURCE
# define _COSMO_SOURCE 1
#endif
/* Enable general extensions on macOS.  */
#ifndef _DARWIN_C_SOURCE
# define _DARWIN_C_SOURCE 1
#endif
/* Enable general extensions on Solaris.  */
#ifndef __EXTENSIONS__
# define __EXTENSIONS__ 1
#endif
/* Enable GNU extensions on systems that have them.  */
#ifndef _GNU_SOURCE
# define _GNU_SOURCE 1
#endif
/* Enable general extensions on NetBSD.
   Enable NetBSD compatibility extensions on Minix.  */
#ifndef _NETBSD_SOURCE
# define _NETBSD_SOURCE 1
#endif
/* Enable OpenBSD compatibility extensions on NetBSD.
   Oddly enough, this does nothing on OpenBSD.  */
#ifndef _OPENBSD_SOURCE
# define _OPENBSD_SOURCE 1
#endif
/* Define to 1 if needed for POSIX-compatible behavior.  */
#ifndef _POSIX_SOURCE
/* # undef _POSIX_SOURCE */
#endif
/* Define to 2 if needed for POSIX-compatible behavior.  */
#ifndef _POSIX_1_SOURCE
/* # undef _POSIX_1_SOURCE */
#endif
/* Enable POSIX-compatible threading on Solaris.  */
#ifndef _POSIX_PTHREAD_SEMANTICS
# define _POSIX_PTHREAD_SEMANTICS 1
#endif
/* Enable extensions specified by ISO/IEC TS 18661-5:2014.  */
#ifndef __STDC_WANT_IEC_60559_ATTRIBS_EXT__
# define __STDC_WANT_IEC_60559_ATTRIBS_EXT__ 1
#endif
/* Enable extensions specified by ISO/IEC TS 18661-1:2014.  */
#ifndef __STDC_WANT_IEC_60559_BFP_EXT__
# define __STDC_WANT_IEC_60559_BFP_EXT__ 1
#endif
/* Enable extensions specified by ISO/IEC TS 18661-2:2015.  */
#ifndef __STDC_WANT_IEC_60559_DFP_EXT__
# define __STDC_WANT_IEC_60559_DFP_EXT__ 1
#endif
/* Enable extensions specified by C23 Annex F.  */
#ifndef __STDC_WANT_IEC_60559_EXT__
# define __STDC_WANT_IEC_60559_EXT__ 1
#endif
/* Enable extensions specified by ISO/IEC TS 18661-4:2015.  */
#ifndef __STDC_WANT_IEC_60559_FUNCS_EXT__
# define __STDC_WANT_IEC_60559_FUNCS_EXT__ 1
#endif
/* Enable extensions specified by C23 Annex H and ISO/IEC TS 18661-3:2015.  */
#ifndef __STDC_WANT_IEC_60559_TYPES_EXT__
# define __STDC_WANT_IEC_60559_TYPES_EXT__ 1
#endif
/* Enable extensions specified by ISO/IEC TR 24731-2:2010.  */
#ifndef __STDC_WANT_LIB_EXT2__
# define __STDC_WANT_LIB_EXT2__ 1
#endif
/* Enable extensions specified by ISO/IEC 24747:2009.  */
#ifndef __STDC_WANT_MATH_SPEC_FUNCS__
# define __STDC_WANT_MATH_SPEC_FUNCS__ 1
#endif
/* Enable extensions on HP NonStop.  */
#ifndef _TANDEM_SOURCE
# define _TANDEM_SOURCE 1
#endif
/* Enable X/Open extensions.  Define to 500 only if necessary
   to make mbstate_t available.  */
#ifndef _XOPEN_SOURCE
/* # undef _XOPEN_SOURCE */
#endif

/* Hack for min() and max() using g++ 6+ */
#define _GLIBCXX_INCLUDE_NEXT_C_HEADERS 1

/* Needed to get PRIxxx macros from stdint.h when not using C99 */
#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS 1
#endif

/* Define to '__inline__' or '__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
/* #undef inline */
#endif

/* Define to the equivalent of the C99 'restrict' keyword, or to
   nothing if this is not supported.  Do not define if restrict is
   supported only directly.  */
#define restrict __restrict__
/* Work around a bug in older versions of Sun C++, which did not
   #define __restrict__ or support _Restrict or __restrict__
   even though the corresponding Sun C compiler ended up with
   "#define restrict _Restrict" or "#define restrict __restrict__"
   in the previous line.  This workaround can be removed once
   we assume Oracle Developer Studio 12.5 (2016) or later.  */
#if defined __SUNPRO_CC && !defined __RESTRICT && !defined __restrict__
# define _Restrict
# define __restrict__
#endif

#define MAX_NUM_OF_NEIGHBOURS 10
