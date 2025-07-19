/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Adaptive softening */
/* #undef ADAPTIVE_SOFTENING */

/* ANARCHY (PU) SPH */
/* #undef ANARCHY_PU_SPH */

/* EAGLE black hole model */
/* #undef BLACK_HOLES_EAGLE */

/* No black hole model */
#define BLACK_HOLES_NONE 1

/* Spin and jet black hole model */
/* #undef BLACK_HOLES_SPIN_JET */

/* Chemistry taken from the AGORA model */
/* #undef CHEMISTRY_AGORA */

/* Chemistry taken from the EAGLE model */
/* #undef CHEMISTRY_EAGLE */

/* Chemistry taken from the GEAR model */
/* #undef CHEMISTRY_GEAR */

/* No chemistry function */
#define CHEMISTRY_NONE 1

/* Chemistry taken from the Quick-Lyman-alpha model */
/* #undef CHEMISTRY_QLA */

/* Use doubles in grackle */
/* #undef CONFIG_BFLOAT_8 */

/* Const du/dt cooling function */
/* #undef COOLING_CONST_DU */

/* Const Lambda cooling function */
/* #undef COOLING_CONST_LAMBDA */

/* Cooling following the EAGLE model (Wiersma+09 tables) */
/* #undef COOLING_EAGLE */

/* Cooling via the grackle library */
/* #undef COOLING_GRACKLE */

/* Grackle chemistry network */
/* #undef COOLING_GRACKLE_MODE */

/* No cooling function */
#define COOLING_NONE 1

/* Cooling following the PS2020 model (Ploeckinger+20 tables) */
/* #undef COOLING_PS2020 */

/* Cooling following the Quick-Lyman-alpha model */
/* #undef COOLING_QLA */

/* Cooling following the Quick-Lyman-alpha model */
/* #undef COOLING_QLA_EAGLE */

/* Cubic spline kernel */
#define CUBIC_SPLINE_KERNEL 1

/* Define to 1 if your C++ compiler doesn't accept -c and -o together. */
/* #undef CXX_NO_MINUS_C_MINUS_O */

/* Enable interaction debugging for sinks */
/* #undef DEBUG_INTERACTIONS_SINKS */

/* Enable interaction debugging */
/* #undef DEBUG_INTERACTIONS_SPH */

/* Enable interaction debugging for stars */
/* #undef DEBUG_INTERACTIONS_STARS */

/* Basic gravity scheme */
/* #undef DEFAULT_GRAVITY */

/* EAGLE entropy floor */
/* #undef ENTROPY_FLOOR_EAGLE */

/* No entropy floor */
#define ENTROPY_FLOOR_NONE 1

/* Quick Lyman-alpha entropy floor */
/* #undef ENTROPY_FLOOR_QLA */

/* Barotropic gas equation of state */
/* #undef EOS_BAROTROPIC_GAS */

/* Ideal gas equation of state */
#define EOS_IDEAL_GAS 1

/* Isothermal gas equation of state */
/* #undef EOS_ISOTHERMAL_GAS */

/* All planetary equations of state */
/* #undef EOS_PLANETARY */

/* Constant gravitational acceleration. */
/* #undef EXTERNAL_POTENTIAL_CONSTANT */

/* Disc-patch external potential */
/* #undef EXTERNAL_POTENTIAL_DISC_PATCH */

/* Hernquist external potential */
/* #undef EXTERNAL_POTENTIAL_HERNQUIST */

/* Hernquist external potential following Springel, Di Matteo & Hernquist 2005
 */
/* #undef EXTERNAL_POTENTIAL_HERNQUIST_SDMH05 */

/* Isothermal external potential */
/* #undef EXTERNAL_POTENTIAL_ISOTHERMAL */

/* Milky-Way like potential composed of a Navarro-Frenk-White + Miyamoto-Nagai
   disk + Power spherical cuttoff external potential. */
/* #undef EXTERNAL_POTENTIAL_MWPotential2014 */

/* Navarro-Frenk-White external potential */
/* #undef EXTERNAL_POTENTIAL_NFW */

/* Navarro-Frenk-White + Miyamoto-Nagai disk external potential */
/* #undef EXTERNAL_POTENTIAL_NFW_MN */

/* No external potential */
#define EXTERNAL_POTENTIAL_NONE 1

/* Point-mass external potential */
/* #undef EXTERNAL_POTENTIAL_POINTMASS */

/* Softened point-mass potential with form 1/(r^2 + softening^2). */
/* #undef EXTERNAL_POTENTIAL_POINTMASS_SOFT */

/* Sine wave external potential in 1D */
/* #undef EXTERNAL_POTENTIAL_SINE_WAVE */

/* Extra i/o fields taken from the EAGLE model */
/* #undef EXTRA_IO_EAGLE */

/* No extra_io function */
#define EXTRA_IO_NONE 1

/* AGORA stellar feedback and evolution model */
/* #undef FEEDBACK_AGORA */

/* EAGLE kinetic stellar feedback and evolution model */
/* #undef FEEDBACK_EAGLE_KINETIC */

/* EAGLE thermal stellar feedback and evolution model */
/* #undef FEEDBACK_EAGLE_THERMAL */

/* GEAR stellar feedback and evolution model */
/* #undef FEEDBACK_GEAR */

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

/* ABC flow external forcing terms */
/* #undef FORCING_ABC_FLOW */

/* No external forcing terms */
#define FORCING_NONE 1

/* Roberts' flow external forcing terms */
/* #undef FORCING_ROBERTS_FLOW */

/* Roberts' flow external forcing terms entering the equations as an
   acceleration term */
/* #undef FORCING_ROBERTS_FLOW_ACCELERATION */

/* Gadget-2 SPH */
/* #undef GADGET2_SPH */

/* Gasoline SPH */
/* #undef GASOLINE_SPH */

/* Number of element to follow */
/* #undef GEAR_CHEMISTRY_ELEMENT_COUNT */

/* GIZMO MFM SPH */
/* #undef GIZMO_MFM_SPH */

/* GIZMO MFV SPH */
/* #undef GIZMO_MFV_SPH */

/* Define to 1 to support Advanced Bit Manipulation */
#define HAVE_ABM 1

/* Define to 1 to support Multi-Precision Add-Carry Instruction Extensions */
#define HAVE_ADX 1

/* Define to 1 to support Advanced Encryption Standard New Instruction Set
   (AES-NI) */
#define HAVE_AES 1

/* Support Altivec instructions */
/* #undef HAVE_ALTIVEC */

/* Define to 1 if you have the <altivec.h> header file. */
/* #undef HAVE_ALTIVEC_H */

/* Define if you have enabled the CNTVCT cycle counter on ARMv7a */
/* #undef HAVE_ARMV7A_CNTVCT */

/* Define if you have enabled the PMCCNTR cycle counter on ARMv7a */
/* #undef HAVE_ARMV7A_PMCCNTR */

/* Define to 1 if the ARM v8.1a instruction CNTVCT_EL0 exists. */
/* #undef HAVE_ARMV8_CNTVCT_EL0 */

/* Define to 1 if the ARM v8.1a instruction PMCCNTR_EL0 exists. */
/* #undef HAVE_ARMV8_PMCCNTR_EL0 */

/* Define to 1 to support Advanced Vector Extensions */
#define HAVE_AVX 1

/* Define to 1 to support Advanced Vector Extensions 2 */
#define HAVE_AVX2 1

/* Define to 1 to support AVX-512 Byte and Word Instructions */
#define HAVE_AVX512_BW 1

/* Define to 1 to support AVX-512 Conflict Detection Instructions */
#define HAVE_AVX512_CD 1

/* Define to 1 to support AVX-512 Doubleword and Quadword Instructions */
#define HAVE_AVX512_DQ 1

/* Define to 1 to support AVX-512 Exponential & Reciprocal Instructions */
/* #undef HAVE_AVX512_ER */

/* Define to 1 to support AVX-512 Foundation Extensions */
#define HAVE_AVX512_F 1

/* Define to 1 to support AVX-512 Integer Fused Multiply Add Instructions */
/* #undef HAVE_AVX512_IFMA */

/* Define to 1 to support AVX-512 Conflict Prefetch Instructions */
/* #undef HAVE_AVX512_PF */

/* Define to 1 to support AVX-512 Vector Byte Manipulation Instructions */
/* #undef HAVE_AVX512_VBMI */

/* Define to 1 to support AVX-512 Vector Length Extensions */
#define HAVE_AVX512_VL 1

/* Define to 1 if you have the `backtrace' function. */
#define HAVE_BACKTRACE 1

/* Define to 1 if you have the `backtrace_symbols' function. */
#define HAVE_BACKTRACE_SYMBOLS 1

/* Define to 1 to support Bit Manipulation Instruction Set 1 */
#define HAVE_BMI1 1

/* Define to 1 to support Bit Manipulation Instruction Set 2 */
#define HAVE_BMI2 1

/* Defined if we have CFITSIO */
/* #undef HAVE_CFITSIO */

/* Defined if we have CHEALPIX */
/* #undef HAVE_CHEALPIX */

/* Define to 1 if you have the `clock_gettime' function. */
#define HAVE_CLOCK_GETTIME 1

/* The CUDA compiler is installed. */
#define HAVE_CUDA 1

/* Define to 1 if you have the <c_asm.h> header file. */
/* #undef HAVE_C_ASM_H */

/* Define to 1 if you have the declaration of `__sincos', and to 0 if you
   don't. */
#define HAVE_DECL___SINCOS 1

/* Define to 1 if you have the declaration of `__sincosf', and to 0 if you
   don't. */
#define HAVE_DECL___SINCOSF 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* The dummy VELOCIraptor library is present. */
/* #undef HAVE_DUMMY_VELOCIRAPTOR */

/* The exp10 function is present. */
#define HAVE_EXP10 1

/* The exp10f function is present. */
#define HAVE_EXP10F 1

/* Defined if floating-point exceptions can be trapped. */
/* #undef HAVE_FE_ENABLE_EXCEPT */

/* The FFTW library appears to be present. */
#define HAVE_FFTW 1

/* Define to 1 to support Fused Multiply-Add Extensions 3 */
#define HAVE_FMA3 1

/* Define to 1 to support Fused Multiply-Add Extensions 4 */
/* #undef HAVE_FMA4 */

/* Define to 1 if you have the `gethrtime' function. */
/* #undef HAVE_GETHRTIME */

/* The GRACKLE library appears to be present. */
/* #undef HAVE_GRACKLE */

/* Defined if you have HDF5 support */
#define HAVE_HDF5 1

/* The HIP compiler is installed. */
/* #undef HAVE_HIP */

/* Define to 1 if hrtime_t is defined in <sys/time.h> */
/* #undef HAVE_HRTIME_T */

/* Define to 1 if you have the <immintrin.h> header file. */
#define HAVE_IMMINTRIN_H 1

/* Define to 1 if you have the <intrinsics.h> header file. */
/* #undef HAVE_INTRINSICS_H */

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* The jemalloc library appears to be present. */
/* #undef HAVE_JEMALLOC */

/* Define to 1 if you have the `amdlibm' library (-lamdlibm). */
/* #undef HAVE_LIBAMDLIBM */

/* The GMP library appears to be present. */
#define HAVE_LIBGMP 1

/* The GSL library appears to be present. */
#define HAVE_LIBGSL 1

/* The GSL CBLAS library appears to be present. */
#define HAVE_LIBGSLCBLAS 1

/* Define to 1 if you have the `m' library (-lm). */
#define HAVE_LIBM 1

/* The NUMA library appears to be present. */
#define HAVE_LIBNUMA 1

/* Have __lsan_ignore_object() call */
/* #undef HAVE_LSAN_IGNORE_OBJECT */

/* The lustre API library appears to be present. */
/* #undef HAVE_LUSTREAPI */

/* Define to 1 if you have the `mach_absolute_time' function. */
/* #undef HAVE_MACH_ABSOLUTE_TIME */

/* Define to 1 if you have the <mach/mach_time.h> header file. */
/* #undef HAVE_MACH_MACH_TIME_H */

/* Define to 1 if you have the <math.h> header file. */
#define HAVE_MATH_H 1

/* The METIS library is present. */
/* #undef HAVE_METIS */

/* Define to 1 if you have the <minix/config.h> header file. */
/* #undef HAVE_MINIX_CONFIG_H */

/* Define to 1 to support Multimedia Extensions */
#define HAVE_MMX 1

/* Define if you have the MPI library. */
#define HAVE_MPI 1

/* The MPI FFTW library appears to be present. */
/* #undef HAVE_MPI_FFTW */

/* The MPI VELOCIraptor library appears to be present. */
/* #undef HAVE_MPI_VELOCIRAPTOR */

/* Define to 1 to support Memory Protection Extensions */
#define HAVE_MPX 1

/* Define if OpenMP is enabled */
#define HAVE_OPENMP 1

/* The OpenMP FFTW library appears to be present. */
/* #undef HAVE_OPENMP_FFTW */

/* HDF5 library supports parallel access */
/* #undef HAVE_PARALLEL_HDF5 */

/* The ParMETIS library is present. */
/* #undef HAVE_PARMETIS */

/* The posix library implements file allocation functions. */
#define HAVE_POSIX_FALLOCATE 1

/* Define to 1 if `posix_memalign' works. */
#define HAVE_POSIX_MEMALIGN 1

/* Define to 1 to support Prefetch Vector Data Into Caches WT1 */
/* #undef HAVE_PREFETCHWT1 */

/* The posix library implements barriers */
#define HAVE_PTHREAD_BARRIERS 1

/* Have PTHREAD_PRIO_INHERIT. */
#define HAVE_PTHREAD_PRIO_INHERIT 1

/* Define to 1 to support Digital Random Number Generator */
#define HAVE_RDRND 1

/* Define to 1 if you have the `read_real_time' function. */
/* #undef HAVE_READ_REAL_TIME */

/* Defined if pthread_setaffinity_np exists. */
#define HAVE_SETAFFINITY 1

/* Define to 1 to support Secure Hash Algorithm Extension */
/* #undef HAVE_SHA */

/* The sincos function is present. */
#define HAVE_SINCOS 1

/* The sincosf function is present. */
#define HAVE_SINCOSF 1

/* Define to 1 to support Streaming SIMD Extensions */
#define HAVE_SSE 1

/* Define to 1 to support Streaming SIMD Extensions */
#define HAVE_SSE2 1

/* Define to 1 to support Streaming SIMD Extensions 3 */
#define HAVE_SSE3 1

/* Define to 1 to support Streaming SIMD Extensions 4.1 */
#define HAVE_SSE4_1 1

/* Define to 1 to support Streaming SIMD Extensions 4.2 */
#define HAVE_SSE4_2 1

/* Define to 1 to support AMD Streaming SIMD Extensions 4a */
/* #undef HAVE_SSE4a */

/* Define to 1 to support Supplemental Streaming SIMD Extensions 3 */
#define HAVE_SSSE3 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdio.h> header file. */
#define HAVE_STDIO_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* The SUNDIALS library is present. */
/* #undef HAVE_SUNDIALS */

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* The TBBmalloc library appears to be present. */
/* #undef HAVE_TBBMALLOC */

/* The tcmalloc library appears to be present. */
/* #undef HAVE_TCMALLOC */

/* The threaded FFTW library appears to be present. */
#define HAVE_THREADED_FFTW 1

/* Define to 1 if you have the `time_base_to_time' function. */
/* #undef HAVE_TIME_BASE_TO_TIME */

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* The VELOCIraptor library appears to be present. */
/* #undef HAVE_VELOCIRAPTOR */

/* Orphan particles should be written out */
/* #undef HAVE_VELOCIRAPTOR_ORPHANS */

/* The MPI VELOCIraptor library has been compiled with the NOMASS option. Only
   useful if running a uniform box. */
/* #undef HAVE_VELOCIRAPTOR_WITH_NOMASS */

/* Support VSX instructions */
/* #undef HAVE_VSX */

/* Define to 1 if you have the <wchar.h> header file. */
#define HAVE_WCHAR_H 1

/* Define to 1 to support eXtended Operations Extensions */
/* #undef HAVE_XOP */

/* Define if you have the UNICOS _rtc() intrinsic. */
/* #undef HAVE__RTC */

/* The __exp10 function is present. */
/* #undef HAVE___EXP10 */

/* The __exp10f function is present. */
/* #undef HAVE___EXP10F */

/* The __sincos function is present. */
#define HAVE___SINCOS 1

/* The __sincosf function is present. */
#define HAVE___SINCOSF 1

/* Pressure-Entropy SPH */
/* #undef HOPKINS_PE_SPH */

/* Pressure-Energy SPH */
/* #undef HOPKINS_PU_SPH */

/* Pressure-Energy SPH with M&M Variable A.V. */
/* #undef HOPKINS_PU_SPH_MONAGHAN */

/* 1D solver */
/* #undef HYDRO_DIMENSION_1D */

/* 2D solver */
/* #undef HYDRO_DIMENSION_2D */

/* 3D solver */
#define HYDRO_DIMENSION_3D 3

/* Hydro scheme with mass fluxes */
/* #undef HYDRO_DOES_MASS_FLUX */

/* Adiabatic index is 2 */
/* #undef HYDRO_GAMMA_2_1 */

/* Adiabatic index is 4/3 */
/* #undef HYDRO_GAMMA_4_3 */

/* Adiabatic index is 5/3 */
#define HYDRO_GAMMA_5_3 5. / 3.

/* Adiabatic index is 7/5 */
/* #undef HYDRO_GAMMA_7_5 */

/* Define to the sub-directory where libtool stores uninstalled libraries. */
#define LT_OBJDIR ".libs/"

/* The maximum number of particle neighbours to be logged */
/* #undef MAX_NUM_OF_NEIGHBOURS */

/* The maximum number of particle neighbours to be logged for sinks */
/* #undef MAX_NUM_OF_NEIGHBOURS_SINKS */

/* The maximum number of particle neighbours to be logged for stars */
/* #undef MAX_NUM_OF_NEIGHBOURS_STARS */

/* Minimal SPH */
/* #undef MINIMAL_SPH */

/* Unstructured Voronoi mesh */
/* #undef MOVING_MESH */

/* Moving mesh hydrodynamics */
/* #undef MOVING_MESH_HYDRO */

/* Do symmetric MPI interactions */
/* #undef MPI_SYMMETRIC_FORCE_INTERACTION_RT */

/* Gravity scheme with per-particle type softening value and background
   particles */
#define MULTI_SOFTENING_GRAVITY 1

/* No mhd */
#define NONE_MHD 1

/* No hydro */
/* #undef NONE_SPH */

/* Name of package */
#define PACKAGE "swift"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "https://gitlab.cosma.dur.ac.uk/swift/swiftsim"

/* Define to the full name of this package. */
#define PACKAGE_NAME "SWIFT"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "SWIFT 2025.04"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "swift"

/* Package web pages */
#define PACKAGE_URL "www.swiftsim.com"

/* Define to the version of this package. */
#define PACKAGE_VERSION "2025.04"

/* Phantom SPH */
/* #undef PHANTOM_SPH */

/* Enable planetary fixed entropy */
/* #undef PLANETARY_FIXED_ENTROPY */

/* Planetary SPH */
/* #undef PLANETARY_SPH */

/* GEAR pressure floor */
/* #undef PRESSURE_FLOOR_GEAR */

/* No pressure floor */
#define PRESSURE_FLOOR_NONE 1

/* Define to necessary symbol if this constant uses a non-standard name on
   your system. */
/* #undef PTHREAD_CREATE_JOINABLE */

/* Quartic spline kernel */
/* #undef QUARTIC_SPLINE_KERNEL */

/* Quintic spline kernel */
/* #undef QUINTIC_SPLINE_KERNEL */

/* REMIX SPH */
/* #undef REMIX_SPH */

/* Exact Riemann solver */
/* #undef RIEMANN_SOLVER_EXACT */

/* Harten-Lax-van Leer-Contact Riemann solver */
/* #undef RIEMANN_SOLVER_HLLC */

/* No Riemann solver */
#define RIEMANN_SOLVER_NONE 1

/* Two Rarefaction Riemann Solver */
/* #undef RIEMANN_SOLVER_TRRS */

/* debugging scheme */
/* #undef RT_DEBUG */

/* GEAR M1 closure scheme */
/* #undef RT_GEAR */

/* Number of photon groups to follow */
/* #undef RT_NGROUPS */

/* No radiative transfer scheme */
#define RT_NONE 1

/* GLF RT riemann solver */
/* #undef RT_RIEMANN_SOLVER_GLF */

/* HLL RT riemann solver */
/* #undef RT_RIEMANN_SOLVER_HLL */

/* No RT riemann solver */
#define RT_RIEMANN_SOLVER_NONE 1

/* SPHM1RT radiative transfer scheme (Chan+21: 2102.08404) */
/* #undef RT_SPHM1RT */

/* Multipole order */
#define SELF_GRAVITY_MULTIPOLE_ORDER 4

/* ShadowSWIFT hydrodynamics */
/* #undef SHADOWSWIFT */

/* Simple, self-contained sink model with only Bondi-Hoyle accretion and no
   SF. */
/* #undef SINK_BASIC */

/* GEAR sink particle model */
/* #undef SINK_GEAR */

/* No sink particle model */
#define SINK_NONE 1

/* The size of `long long int', as computed by sizeof. */
/* #undef SIZEOF_LONG_LONG_INT */

/* SPHENIX SPH */
#define SPHENIX_SPH 1

/* Basic stellar model */
#define STARS_BASIC 1

/* EAGLE stellar model */
/* #undef STARS_EAGLE */

/* GEAR stellar model */
/* #undef STARS_GEAR */

/* No stellar model */
/* #undef STARS_NONE */

/* EAGLE star formation model (Schaye and Dalla Vecchia (2008)) */
/* #undef STAR_FORMATION_EAGLE */

/* GEAR star formation model (Revaz and Jablonka (2018)) */
/* #undef STAR_FORMATION_GEAR */

/* No star formation */
#define STAR_FORMATION_NONE 1

/* Quick Lyman-alpha star formation model) */
/* #undef STAR_FORMATION_QLA */

/* Define to 1 if all of the C90 standard headers exist (not just the ones
   required in a freestanding environment). This macro is provided for
   backward compatibility; new code need not use it. */
#define STDC_HEADERS 1

/* Particles with smaller ID than this will be considered as boundaries. */
/* #undef SWIFT_BOUNDARY_PARTICLES */

/* Enable cell graph */
/* #undef SWIFT_CELL_GRAPH */

/* Flags passed to configure */
#define SWIFT_CONFIG_FLAGS "--enable-compiler-warnings --with-cuda"

/* Enable expensive debugging */
/* #undef SWIFT_DEBUG_CHECKS */

/* Enable task debugging */
/* #undef SWIFT_DEBUG_TASKS */

/* Enable threadpool debugging */
/* #undef SWIFT_DEBUG_THREADPOOL */

/* Enable developer code options */
/* #undef SWIFT_DEVELOP_MODE */

/* Enable dumper thread */
/* #undef SWIFT_DUMPER_THREAD */

/* Particles with smaller ID than this will be considered as boundaries. */
/* #undef SWIFT_FIXED_BOUNDARY_PARTICLES */

/* Enable ghost statistics for hydro, stars and black holes */
/* #undef SWIFT_GHOST_STATS */

/* Enable gravity brute-force checks */
/* #undef SWIFT_GRAVITY_FORCE_CHECKS */

/* Disable calculation of the gravitational potential */
/* #undef SWIFT_GRAVITY_NO_POTENTIAL */

/* Enable hydro density brute-force checks */
/* #undef SWIFT_HYDRO_DENSITY_CHECKS */

/* Make the code run in a way to produce a glass file for gravity/cosmology */
/* #undef SWIFT_MAKE_GRAVITY_GLASS */

/* Enable memory usage reports */
/* #undef SWIFT_MEMUSE_REPORTS */

/* Have /proc/self/statm capability */
#define SWIFT_MEMUSE_STATM 1

/* Enable MPI task reports */
/* #undef SWIFT_MPIUSE_REPORTS */

/* The MPI library name, if known. */
#define SWIFT_MPI_LIBRARY "Open MPI"

/* Particles with smaller ID than this will have zero gravity forces */
/* #undef SWIFT_NO_GRAVITY_BELOW_ID */

/* Value of the random seed. */
#define SWIFT_RANDOM_SEED_XOR 0

/* additional debugging checks for RT */
/* #undef SWIFT_RT_DEBUG_CHECKS */

/* Enable sink density brute-force checks */
/* #undef SWIFT_SINK_DENSITY_CHECKS */

/* Enable stars density brute-force checks */
/* #undef SWIFT_STARS_DENSITY_CHECKS */

/* Makes SWIFT use atomic-free and lock-free tasks. */
/* #undef SWIFT_TASKS_WITHOUT_ATOMICS */

/* Use the same physical constants as Gadget-2 */
/* #undef SWIFT_USE_GADGET2_PHYSICAL_CONSTANTS */

/* Enable use of naive cell interaction functions */
/* #undef SWIFT_USE_NAIVE_INTERACTIONS */

/* Enable use of naive cell interaction functions for stars in RT tasks */
/* #undef SWIFT_USE_NAIVE_INTERACTIONS_RT */

/* Enable use of naive cell interaction functions for stars */
/* #undef SWIFT_USE_NAIVE_INTERACTIONS_STARS */

/* Enable individual timers */
/* #undef SWIFT_USE_TIMERS */

/* Define to 1 if you can safely include both <sys/time.h> and <time.h>. This
   macro is obsolete. */
#define TIME_WITH_SYS_TIME 1

/* Tracers taken from the EAGLE model */
/* #undef TRACERS_EAGLE */

/* No tracers function */
#define TRACERS_NONE 1

/* Enable extensions on AIX 3, Interix.  */
#ifndef _ALL_SOURCE
#define _ALL_SOURCE 1
#endif
/* Enable general extensions on macOS.  */
#ifndef _DARWIN_C_SOURCE
#define _DARWIN_C_SOURCE 1
#endif
/* Enable general extensions on Solaris.  */
#ifndef __EXTENSIONS__
#define __EXTENSIONS__ 1
#endif
/* Enable GNU extensions on systems that have them.  */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif
/* Enable X/Open compliant socket functions that do not require linking
   with -lxnet on HP-UX 11.11.  */
#ifndef _HPUX_ALT_XOPEN_SOCKET_API
#define _HPUX_ALT_XOPEN_SOCKET_API 1
#endif
/* Identify the host operating system as Minix.
   This macro does not affect the system headers' behavior.
   A future release of Autoconf may stop defining this macro.  */
#ifndef _MINIX
/* # undef _MINIX */
#endif
/* Enable general extensions on NetBSD.
   Enable NetBSD compatibility extensions on Minix.  */
#ifndef _NETBSD_SOURCE
#define _NETBSD_SOURCE 1
#endif
/* Enable OpenBSD compatibility extensions on NetBSD.
   Oddly enough, this does nothing on OpenBSD.  */
#ifndef _OPENBSD_SOURCE
#define _OPENBSD_SOURCE 1
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
#define _POSIX_PTHREAD_SEMANTICS 1
#endif
/* Enable extensions specified by ISO/IEC TS 18661-5:2014.  */
#ifndef __STDC_WANT_IEC_60559_ATTRIBS_EXT__
#define __STDC_WANT_IEC_60559_ATTRIBS_EXT__ 1
#endif
/* Enable extensions specified by ISO/IEC TS 18661-1:2014.  */
#ifndef __STDC_WANT_IEC_60559_BFP_EXT__
#define __STDC_WANT_IEC_60559_BFP_EXT__ 1
#endif
/* Enable extensions specified by ISO/IEC TS 18661-2:2015.  */
#ifndef __STDC_WANT_IEC_60559_DFP_EXT__
#define __STDC_WANT_IEC_60559_DFP_EXT__ 1
#endif
/* Enable extensions specified by ISO/IEC TS 18661-4:2015.  */
#ifndef __STDC_WANT_IEC_60559_FUNCS_EXT__
#define __STDC_WANT_IEC_60559_FUNCS_EXT__ 1
#endif
/* Enable extensions specified by ISO/IEC TS 18661-3:2015.  */
#ifndef __STDC_WANT_IEC_60559_TYPES_EXT__
#define __STDC_WANT_IEC_60559_TYPES_EXT__ 1
#endif
/* Enable extensions specified by ISO/IEC TR 24731-2:2010.  */
#ifndef __STDC_WANT_LIB_EXT2__
#define __STDC_WANT_LIB_EXT2__ 1
#endif
/* Enable extensions specified by ISO/IEC 24747:2009.  */
#ifndef __STDC_WANT_MATH_SPEC_FUNCS__
#define __STDC_WANT_MATH_SPEC_FUNCS__ 1
#endif
/* Enable extensions on HP NonStop.  */
#ifndef _TANDEM_SOURCE
#define _TANDEM_SOURCE 1
#endif
/* Enable X/Open extensions.  Define to 500 only if necessary
   to make mbstate_t available.  */
#ifndef _XOPEN_SOURCE
/* # undef _XOPEN_SOURCE */
#endif


/* Version number of package */
#define VERSION "2025.04"

/* Wendland-C2 kernel */
/* #undef WENDLAND_C2_KERNEL */

/* Wendland-C4 kernel */
/* #undef WENDLAND_C4_KERNEL */

/* Wendland-C6 kernel */
/* #undef WENDLAND_C6_KERNEL */

/* csds enabled */
/* #undef WITH_CSDS */

/* Enable FoF */
/* #undef WITH_FOF */

/* Enable custom icbrtf */
/* #undef WITH_ICBRTF */

/* Enable lightcone outputs */
/* #undef WITH_LIGHTCONE */

/* Link against the gperftools profiling library. */
/* #undef WITH_PROFILER */

/* Enable stand-alone FoF */
/* #undef WITH_STAND_ALONE_FOF */

/* Enable hand-written vectorization */
#define WITH_VECTORIZATION 0

/* Hack for min() and max() using g++ 6+ */
#define _GLIBCXX_INCLUDE_NEXT_C_HEADERS 1

/* Needed to get PRIxxx macros from stdint.h when not using C99 */
#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS 1
#endif

/* Define to `__inline__' or `__inline' if that's what the C compiler
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
#define _Restrict
#define __restrict__
#endif
