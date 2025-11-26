# This file is part of SWIFT.
# Copyright (C) 2012 pedro.gonnet@durham.ac.uk.
#               2016 p.w.draper@durham.ac.uk.
#               2025 mladen.ivkovic@durham.ac.uk
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# Init the project.
AC_INIT([SWIFTGPUPACKSIM],[0.1],[https://github.com/mladenivkovic/swiftgpupacksim])
swift_config_flags="$*"

AC_COPYRIGHT
AC_CONFIG_SRCDIR([src/run_simulation.c])
AC_CONFIG_AUX_DIR([.])
AM_INIT_AUTOMAKE([subdir-objects foreign])

# Add local macro collection.
AC_CONFIG_MACRO_DIR([m4])

# Stop default CFLAGS from anyone except the environment.
: ${CFLAGS=""}

# Generate header file.
AC_CONFIG_HEADERS([config.h])


# Find and test the compiler.
AX_CHECK_ENABLE_DEBUG
# Enable POSIX and platform extension preprocessor macros.
AC_USE_SYSTEM_EXTENSIONS
AC_PROG_CC
AM_PROG_CC_C_O

# Find and test the C++ compiler.
AC_PROG_CXX
AC_PROG_CXX_C_O

# We need this for compilation hints and possibly FFTW.
AX_OPENMP

# If debug is selected then we also define SWIFT_DEVELOP_MODE to control
# any developer code options.
if test "x$ax_enable_debug" != "xno"; then
   AC_DEFINE([SWIFT_DEVELOP_MODE],1,[Enable developer code options])
fi

# C++ in GCC 6 and above has an issue with undefined the min() and max()
# macros. This hack works around that.
AC_DEFINE([_GLIBCXX_INCLUDE_NEXT_C_HEADERS],1,[Hack for min() and max() using g++ 6+])

# Enable POSIX and platform extension preprocessor macros.
AC_USE_SYSTEM_EXTENSIONS

# Check for C compiler version and vendor.
AX_COMPILER_VENDOR
AX_COMPILER_VERSION

# Check whether we have a recent enough GCC
if test "$ax_cv_c_compiler_vendor" = "gnu"; then
   AX_COMPARE_VERSION([$ax_cv_c_compiler_version], [ge], [8.1.0],
		      [gcc_handles_avx512="yes"],
		      [gcc_handles_avx512="no"])
fi

#  Restrict support.
AC_C_RESTRICT

# Use best known optimization for the current architecture. Actual optimization
# happens later so we can avoid any issues it introduces with the compiler,
# but we need to know if that will happen now.
AC_ARG_ENABLE([optimization],
   [AS_HELP_STRING([--enable-optimization],
     [Enable compile time optimization flags for host @<:@yes/no@:>@]
   )],
   [enable_opt="$enableval"],
   [enable_opt="yes"]
)

# Interprocedural optimization support. Can need special handling for linking
# and archiving as well as compilation. Needs to be done before libtool is
# configured so we use the correct LD. It can give good improvements for
# clang based compilers, so the default is enabled, but we make that
# disabled when not optimizing or debugging support is enabled.
enable_ipo_default="yes";
if test "x$ax_enable_debug" = "xyes" -o "x$enable_opt" != "xyes"; then
   enable_ipo_default="no";
   AC_MSG_WARN([Interprocedural optimization support default is changed to false])
fi
AC_ARG_ENABLE([ipo],
   [AS_HELP_STRING([--enable-ipo],
     [Enable interprocedural optimization [default=yes unless debugging]]
   )],
   [enable_ipo="$enableval"],
   [enable_ipo="$enable_ipo_default"]
)

if test "$enable_ipo" = "yes"; then
   if test "$ax_cv_c_compiler_vendor" = "intel"; then
      CFLAGS="$CFLAGS -ip -ipo"
      LDFLAGS="$LDFLAGS -ipo"
      AC_CHECK_PROGS([AR], [xiar])
      AC_CHECK_PROGS([LD], [xild])
      AC_MSG_RESULT([added Intel interprocedural optimization support])
   elif test "$ax_cv_c_compiler_vendor" = "oneapi"; then
      CFLAGS="$CFLAGS -ipo"
      LDFLAGS="$LDFLAGS -ipo"
      AC_CHECK_PROGS([AR], [xiar])
      AC_CHECK_PROGS([RANLIB], [llvm-ranlib])
      AC_MSG_RESULT([added oneapi interprocedural optimization support])
   elif test "$ax_cv_c_compiler_vendor" = "gnu"; then
      CFLAGS="$CFLAGS -flto"
      LDFLAGS="$LDFLAGS -flto"
      AX_COMPARE_VERSION($ax_cv_c_compiler_version, [ge], [5.0.0],
         [
         AC_CHECK_PROGS([AR], [gcc-ar])
         AC_CHECK_PROGS([RANLIB], [gcc-ranlib])
         ], [:] )
      AC_MSG_RESULT([added GCC interprocedural optimization support])
   elif test "$ax_cv_c_compiler_vendor" = "clang"; then
      CFLAGS="$CFLAGS -flto"
      LDFLAGS="$LDFLAGS -flto"
      AC_CHECK_PROGS([RANLIB], [llvm-ranlib])
      AC_MSG_RESULT([added LLVM interprocedural optimization support])
   else
      AC_MSG_WARN([Compiler does not support interprocedural optimization])
   fi
fi

# Add libtool support (now that CC is defined). Disable shared libraries by default.
LT_INIT([disable-shared])

# Need C99 and inline support. Only for autoconfs to version 2.69.
m4_version_prereq([2.71], [], [AC_PROG_CC_C99])

# Need inline support.
AC_C_INLINE


# If debugging try to show inlined functions.
if test "x$ax_enable_debug" = "xyes"; then
   #  Show inlined functions.
   if test "$ax_cv_c_compiler_vendor" = "gnu"; then
      # Would like to use -gdwarf and let the compiler pick a good version
      # but that doesn't always work.
      AX_CHECK_COMPILE_FLAG([-gdwarf -fvar-tracking-assignments],
        [inline_EXTRA_FLAGS="-gdwarf -fvar-tracking-assignments"],
        [inline_EXTRA_FLAGS="-gdwarf-2 -fvar-tracking-assignments"])
      CFLAGS="$CFLAGS $inline_EXTRA_FLAGS"
   elif test "$ax_cv_c_compiler_vendor" = "intel"; then
      CFLAGS="$CFLAGS -debug inline-debug-info"
   elif test "$ax_cv_c_compiler_vendor" = "oneapi"; then
      CFLAGS="$CFLAGS -debug inline-debug-info"
   fi
fi

# Check if expensive debugging is on.
AC_ARG_ENABLE([debugging-checks],
   [AS_HELP_STRING([--enable-debugging-checks],
     [Activate expensive consistency checks @<:@yes/no@:>@]
   )],
   [enable_debugging_checks="$enableval"],
   [enable_debugging_checks="no"]
)
if test "$enable_debugging_checks" = "yes"; then
   AC_DEFINE([SWIFT_DEBUG_CHECKS],1,[Enable expensive debugging])
fi

# Check if expensive debugging is on.
AC_ARG_ENABLE([no-cuda-mallochost],
   [AS_HELP_STRING([--enable-no-cuda-mallochost],
     [Disable cudaMallocHost calls and use malloc instead @<:@yes/no@:>@]
   )],
   [enable_no_cuda_mallochost="$enableval"],
   [enable_no_cuda_mallochost="no"]
)
if test "$enable_no_cuda_mallochost" = "yes"; then
   AC_DEFINE([NO_CUDA_MALLOCHOST],1,[Disable cudaMallocHost])
fi


# Check whether we have any of the ARM v8.1 tick timers
AX_ASM_ARM_PMCCNTR
AX_ASM_ARM_CNTVCT

# The system memory report depends on the presence of /proc/self/statm, i.e.
# a linux OS, check for that.
if test -f "/proc/self/statm"; then
   AC_DEFINE([SWIFT_MEMUSE_STATM],1,[Have /proc/self/statm capability])
fi

# Define HAVE_POSIX_MEMALIGN if it works.
AX_FUNC_POSIX_MEMALIGN

# Add address sanitizer options to flags, if requested. Only useful for GCC
# version 4.8 and later and clang.
AC_ARG_ENABLE([sanitizer],
   [AS_HELP_STRING([--enable-sanitizer],
     [Enable memory error detection using address sanitizer @<:@no/yes@:>@]
   )],
   [enable_san="$enableval"],
   [enable_san="no"]
)

if test "$enable_san" = "yes"; then
   if test "$ax_cv_c_compiler_vendor" = "gnu"; then
      AX_COMPARE_VERSION( $ax_cv_c_compiler_version, [ge], [4.8.0],
                          [enable_san="yes"], [enable_san="no"] )
   elif test "$ax_cv_c_compiler_vendor" = "clang"; then
      AX_COMPARE_VERSION( $ax_cv_c_compiler_version, [ge], [3.2.0],
                          [enable_san="yes"], [enable_san="no"] )
   fi
   if test "$enable_san" = "yes"; then
      CFLAGS="$CFLAGS -fsanitize=address -fno-omit-frame-pointer"
      AC_MSG_RESULT([Adding address sanitizer support... yes])

        # Check if we have access to the __lsan_ignore_object() call for
        # marking memory allocations as deliberately leaked.
        AC_LINK_IFELSE([AC_LANG_SOURCE([[
                           #include <stdlib.h>
                           #include <sanitizer/lsan_interface.h>
                           int main(int argc, char *argv[]) {
                              void *p = malloc(1);
                              __lsan_ignore_object(p);
                              return 0;
                           }]])],
           [AC_DEFINE(HAVE_LSAN_IGNORE_OBJECT, 1, [Have __lsan_ignore_object() call])],
           [AC_MSG_WARN([Sanitizer enabled but no __lsan_ignore_object()])])
   else
      AC_MSG_WARN([Compiler does not support address sanitizer option])
   fi
fi

# Add the undefined sanitizer option to flags. Only useful for GCC
# version 4.9 and later and clang to detected undefined code behaviour
# such as integer overflow and memory alignment issues.
AC_ARG_ENABLE([undefined-sanitizer],
   [AS_HELP_STRING([--enable-undefined-sanitizer],
     [Enable detection of code that causes undefined behaviour @<:@no/yes@:>@]
   )],
   [enable_ubsan="$enableval"],
   [enable_ubsan="no"]
)

if test "$enable_ubsan" = "yes"; then
   if test "$ax_cv_c_compiler_vendor" = "gnu"; then
      AX_COMPARE_VERSION( $ax_cv_c_compiler_version, [ge], [4.9.0],
                          [enable_ubsan="yes"], [enable_ubsan="no"] )
   elif test "$ax_cv_c_compiler_vendor" = "clang"; then
      AX_COMPARE_VERSION( $ax_cv_c_compiler_version, [ge], [3.7.0],
                          [enable_ubsan="yes"], [enable_ubsan="no"] )
   fi
   if test "$enable_ubsan" = "yes"; then
      CFLAGS="$CFLAGS -fsanitize=undefined"
      AC_MSG_RESULT([added undefined sanitizer support])
   else
      AC_MSG_WARN([Compiler does not support undefined sanitizer option])
   fi
fi


# Autoconf stuff.
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_EGREP


# Check for the libraries we will need.
AC_CHECK_LIB(m,sqrt,,AC_MSG_ERROR(something is wrong with the math library!))

# Check for pthreads.
AX_PTHREAD([LIBS="$PTHREAD_LIBS $LIBS" CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
    CC="$PTHREAD_CC" LDFLAGS="$LDFLAGS $PTHREAD_LIBS $LIBS"],
    AC_MSG_ERROR([Could not find a working version of
    the pthread library. Make sure you have the library and header files installed
    or use CPPFLAGS and LDFLAGS if the library is installed in a
    non-standard location.]))

# Check whether POSIX thread barriers are implemented (e.g. OSX does not have them)
have_pthread_barrier="no"
AC_CHECK_LIB(pthread, pthread_barrier_init,
	     have_pthread_barrier="yes",
	     AC_MSG_WARN(POSIX implementation does not have barriers. SWIFT will use home-made ones.))
if test "x$have_pthread_barrier" = "xyes"; then
  AC_DEFINE([HAVE_PTHREAD_BARRIERS], [1], [The posix library implements barriers])
fi

# Check whether POSIX file allocation functions exist (e.g. OSX does not have them)
AC_CHECK_LIB(pthread, posix_fallocate,
	     AC_DEFINE([HAVE_POSIX_FALLOCATE], [1], [The posix library implements file allocation functions.]),
	     AC_MSG_WARN(POSIX implementation does not have file allocation functions.))



# Check for CUDA
AC_ARG_WITH([cuda],
    [AS_HELP_STRING([--with-cuda=PATH],
       [root directory where CUDA is installed @<:@yes/no@:>@]
    )],
    [],
    [with_cuda="yes"]
)

if test "x$with_cuda" != "xyes"; then
  # we got a path to a cuda installation, use that.
  # Note: CUDA_*FLAGS get passed to the host compiler.
  # NVCC_*FLAGS get passed to nvcc.
  CUDA_CFLAGS="$CUDA_CFLAGS -I$with_cuda/include"
  CUDA_LDFLAGS="-L$with_cuda/lib -L$with_cuda/lib64"
  CUDA_LIBS="-lcudadevrt -lcudart"
  NVCC="$with_cuda/bin/nvcc"
  NVCC_FLAGS="$NVCC_FLAGS"
  NVCC_LDFLAGS="$NVCC_LDFLAGS"
  NVCC_LIBS="$NVCC_LIBS"
else
  # Get default installation
  AC_PATH_PROG([NVCC],[nvcc])
  echo "Found nvcc = $NVCC"
  if test -n "$NVCC"; then
    CUDA_HOME="`dirname $NVCC`/.."
    CUDA_CFLAGS="$CUDA_CFLAGS -I${CUDA_HOME}/include"
    CUDA_LDFLAGS="-L${CUDA_HOME}/lib -L${CUDA_HOME}/lib64"
    CUDA_LIBS="-lcudadevrt -lcudart"
    NVCC_FLAGS="$NVCC_FLAGS"
    NVCC_LDFLAGS="$NVCC_LDFLAGS"
    NVCC_LIBS="$NVCC_LIBS"
  fi
fi

# if test "x$have_cuda" != "xno"; then
AC_DEFINE([HAVE_CUDA], 1, [The CUDA compiler is installed.])

# disable optimization?
if test "$enable_opt" != "yes"; then
  # use fast approximation mode for divisions et al.
  # Don't want numerical output to change our behaviour.
  NVCC_FLAGS="$NVCC_FLAGS"" -O0 "
else
  # Add optimization flags. We have to do this here because nvcc doesn't
  # have something like --no-use_fast_math.
  NVCC_FLAGS="$NVCC_FLAGS"" --use_fast_math --extra-device-vectorization"
  if test "x$with_gpu_debug" == "xno"; then
    # Add link time optimization if we're not using debug flags.
    # Can't have -G and -lto simultaneously, that's a nvcc fatal error.
    # Again because nvcc doesn't have something like --no-lto.
    NVCC_FLAGS="$NVCC_FLAGS"" --lto"
  fi
fi

# add debug symbols?
if test "x$with_gpu_debug" != "xno"; then
  NVCC_FLAGS="$NVCC_FLAGS"" -g -G --ftz=true"
fi

NVCC_FLAGS="$NVCC_FLAGS"" -arch=${with_cuda_arch}"
# fi
#
AC_SUBST(CUDA_CFLAGS)
AC_SUBST(CUDA_LDFLAGS)
AC_SUBST(CUDA_LIBS)
AC_SUBST(NVCC)
AC_SUBST(NVCC_FLAGS)
AC_SUBST(NVCC_LDFLAGS)
AC_SUBST(NVCC_LIBS)


# Check for special allocators
have_special_allocator="no"

#  Check for tcmalloc a fast malloc that is part of the gperftools.
have_tcmalloc="no"
AC_ARG_WITH([tcmalloc],
   [AS_HELP_STRING([--with-tcmalloc=PATH],
      [use tcmalloc library or specify the directory with lib @<:@yes/no@:>@]
   )],
   [with_tcmalloc="$withval"],
   [with_tcmalloc="no"]
)
if test "x$with_tcmalloc" != "xno" -a "x$have_special_allocator" != "xno"; then
   AC_MSG_ERROR("Cannot activate more than one alternative malloc library")
fi

if test "x$with_tcmalloc" != "xno"; then
   if test "x$with_tcmalloc" != "xyes" -a "x$with_tcmalloc" != "x"; then
      tclibs="-L$with_tcmalloc -ltcmalloc"
   else
      tclibs="-ltcmalloc"
   fi
   AC_CHECK_LIB([tcmalloc],[tc_cfree],[have_tcmalloc="yes"],[have_tcmalloc="no"],
                $tclibs)

   #  Could just have the minimal version.
   if test "$have_tcmalloc" = "no"; then
      if test "x$with_tcmalloc" != "xyes" -a "x$with_tcmalloc" != "x"; then
         tclibs="-L$with_tcmalloc -ltcmalloc_minimal"
      else
         tclibs="-ltcmalloc_minimal"
      fi
      AC_CHECK_LIB([tcmalloc],[tc_cfree],[have_tcmalloc="yes"],[have_tcmalloc="no"],
                   $tclibs)
   fi

   if test "$have_tcmalloc" = "yes"; then
      TCMALLOC_LIBS="$tclibs"

      AC_DEFINE([HAVE_TCMALLOC],1,[The tcmalloc library appears to be present.])

      have_special_allocator="tcmalloc"

      # Prevent compilers that replace the calls with built-ins (GNU 99) from doing so.
      case "$ax_cv_c_compiler_vendor" in
        intel | gnu | clang | oneapi)
             CFLAGS="$CFLAGS -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free"
          ;;
      esac

   else
      TCMALLOC_LIBS=""
   fi
fi
AC_SUBST([TCMALLOC_LIBS])
AM_CONDITIONAL([HAVETCMALLOC],[test -n "$TCMALLOC_LIBS"])

#  Check for jemalloc another fast malloc that is good with contention.
have_jemalloc="no"
AC_ARG_WITH([jemalloc],
   [AS_HELP_STRING([--with-jemalloc=PATH],
      [use jemalloc library or specify the directory with lib @<:@yes/no@:>@]
   )],
   [with_jemalloc="$withval"],
   [with_jemalloc="no"]
)
if test "x$with_jemalloc" != "xno" -a "x$have_special_allocator" != "xno"; then
   AC_MSG_ERROR("Cannot activate more than one alternative malloc library")
fi

if test "x$with_jemalloc" != "xno"; then
   if test "x$with_jemalloc" != "xyes" -a "x$with_jemalloc" != "x"; then
      jelibs="-L$with_jemalloc -ljemalloc"
   else
      jelibs="-ljemalloc"
   fi
   AC_CHECK_LIB([jemalloc],[malloc_usable_size],[have_jemalloc="yes"],[have_jemalloc="no"],
                $jelibs)

   if test "$have_jemalloc" = "yes"; then
      JEMALLOC_LIBS="$jelibs"

      AC_DEFINE([HAVE_JEMALLOC],1,[The jemalloc library appears to be present.])

      have_special_allocator="jemalloc"

      # Prevent compilers that replace the regular calls with built-ins (GNU 99) from doing so.
      case "$ax_cv_c_compiler_vendor" in
        intel | gnu | clang | oneapi)
             CFLAGS="$CFLAGS -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free"
          ;;
      esac

   else
      JEMALLOC_LIBS=""
   fi
fi
AC_SUBST([JEMALLOC_LIBS])
AM_CONDITIONAL([HAVEJEMALLOC],[test -n "$JEMALLOC_LIBS"])

#  Check for tbbmalloc, Intel's fast and parallel allocator
have_tbbmalloc="no"
AC_ARG_WITH([tbbmalloc],
   [AS_HELP_STRING([--with-tbbmalloc=PATH],
      [use tbbmalloc library or specify the directory with lib @<:@yes/no@:>@]
   )],
   [with_tbbmalloc="$withval"],
   [with_tbbmalloc="no"]
)
if test "x$with_tbbmalloc" != "xno" -a "x$have_special_allocator" != "xno"; then
   AC_MSG_ERROR("Cannot activate more than one alternative malloc library")
fi

if test "x$with_tbbmalloc" != "xno"; then
   if test "x$with_tbbmalloc" != "xyes" -a "x$with_tbbmalloc" != "x"; then
      tbblibs="-L$with_tbbmalloc -ltbbmalloc_proxy -ltbbmalloc"
   else
      tbblibs="-ltbbmalloc_proxy -ltbbmalloc"
   fi
   AC_CHECK_LIB([tbbmalloc],[scalable_malloc],[have_tbbmalloc="yes"],[have_tbbmalloc="no"],
                $tbblibs)

   if test "$have_tbbmalloc" = "yes"; then
      TBBMALLOC_LIBS="$tbblibs"

      AC_DEFINE([HAVE_TBBMALLOC],1,[The TBBmalloc library appears to be present.])

      have_special_allocator="TBBmalloc"

      # Prevent compilers that replace the calls with built-ins (GNU 99) from doing so.
      case "$ax_cv_c_compiler_vendor" in
        intel | gnu | clang | oneapi)
             CFLAGS="$CFLAGS -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free"
          ;;
      esac

   else
      TBBMALLOC_LIBS=""
   fi
fi
AC_SUBST([TBBMALLOC_LIBS])
AM_CONDITIONAL([HAVETBBMALLOC],[test -n "$TBBMALLOC_LIBS"])



# Check for setaffinity.
AC_CHECK_FUNC(pthread_setaffinity_np, AC_DEFINE([HAVE_SETAFFINITY],[1],
    [Defined if pthread_setaffinity_np exists.]) )
AM_CONDITIONAL(HAVESETAFFINITY,
    [test "$ac_cv_func_pthread_setaffinity_np" = "yes"])



# Check for Intel and PowerPC intrinsics header optionally used by vector.h.
AC_CHECK_HEADERS([immintrin.h], [], [],
[#ifdef HAVE_IMMINTRIN_H
# include <immintrin.h>
#endif
])
AC_CHECK_HEADERS([altivec.h], [], [],
[#ifdef HAVE_ALTIVEC_H
# include <altivec.h>
#endif
])

# Check for timing functions needed by cycle.h.
AC_CHECK_HEADERS_ONCE([sys/time.h])
if test $ac_cv_header_sys_time_h = yes; then
  AC_DEFINE([TIME_WITH_SYS_TIME],[1],[Define to 1 if you can safely include both <sys/time.h>
	     and <time.h>.  This macro is obsolete.])
fi

AC_CHECK_HEADERS([sys/time.h], [], [],
[#ifdef HAVE_SYS_TIME_H
# include <sys/time.h>
#endif
])
AC_CHECK_HEADERS([c_asm.h], [], [],
[#ifdef HAVE_C_ASM_H
# include <c_asm.h>
#endif
])
AC_CHECK_HEADERS([intrinsics.h], [], [],
[#ifdef HAVE_INTRINSICS_H
# include <intrinsics.h>
#endif
])
AC_CHECK_HEADERS([mach/mach_time.h], [], [],
[#ifdef HAVE_MACH_MACH_TIME_H
# include <mach/mach_time.h>
#endif
])

AC_CHECK_TYPE([hrtime_t],[AC_DEFINE(HAVE_HRTIME_T, 1, [Define to 1 if hrtime_t
is defined in <sys/time.h>])],,
[#if HAVE_SYS_TIME_H
#include <sys/time.h>
#endif])
AC_CHECK_FUNCS([gethrtime read_real_time time_base_to_time clock_gettime mach_absolute_time])
AC_MSG_CHECKING([for _rtc intrinsic])
rtc_ok=yes
AC_LINK_IFELSE([AC_LANG_PROGRAM(
[[#ifdef HAVE_INTRINSICS_H
#include <intrinsics.h>
#endif]],
[[_rtc()]])],
[AC_DEFINE(HAVE__RTC,1,[Define if you have the UNICOS _rtc() intrinsic.])],[rtc_ok=no])
AC_MSG_RESULT($rtc_ok)

# Special timers for the ARM v7 platforms (taken from FFTW-3 to match their cycle.h)
AC_ARG_ENABLE(armv7a-cntvct, [AS_HELP_STRING([--enable-armv7a-cntvct],[enable the cycle counter on Armv7a via the CNTVCT register])], have_armv7acntvct=$enableval)
if test "$have_armv7acntvct"x = "yes"x; then
      AC_DEFINE(HAVE_ARMV7A_CNTVCT,1,[Define if you have enabled the CNTVCT cycle counter on ARMv7a])
fi

AC_ARG_ENABLE(armv7a-pmccntr, [AS_HELP_STRING([--enable-armv7a-pmccntr],[enable the cycle counter on Armv7a via the PMCCNTR register])], have_armv7apmccntr=$enableval)
if test "$have_armv7apmccntr"x = "yes"x; then
      AC_DEFINE(HAVE_ARMV7A_PMCCNTR,1,[Define if you have enabled the PMCCNTR cycle counter on ARMv7a])
fi

# Check if we have native exp10 and exp10f functions. If not fallback to our
# implementations. On Apple/CLANG/Homebrew gcc we have __exp10, so also check for that
# if the compiler is clang.
AC_CHECK_LIB([m],[exp10], [AC_DEFINE([HAVE_EXP10],1,[The exp10 function is present.])])
AC_CHECK_LIB([m],[exp10f], [AC_DEFINE([HAVE_EXP10F],1,[The exp10f function is present.])])
AC_CHECK_LIB([m],[__exp10], [AC_DEFINE([HAVE___EXP10],1,[The __exp10 function is present.])])
AC_CHECK_LIB([m],[__exp10f], [AC_DEFINE([HAVE___EXP10F],1,[The __exp10f function is present.])])

# Check if we have native sincos and sincosf functions. If not fallback to our
# implementations. On Apple/CLANG we have __sincos, so also check for that
# if the compiler is clang.
AC_CHECK_LIB([m],[sincos], [AC_DEFINE([HAVE_SINCOS],1,[The sincos function is present.])])
AC_CHECK_LIB([m],[sincosf], [AC_DEFINE([HAVE_SINCOSF],1,[The sincosf function is present.])])
if test "$ax_cv_c_compiler_vendor" = "clang"; then
      AC_CHECK_LIB([m],[__sincos], [AC_DEFINE([HAVE___SINCOS],1,[The __sincos function is present.])])
      AC_CHECK_LIB([m],[__sincosf], [AC_DEFINE([HAVE___SINCOSF],1,[The __sincosf function is present.])])
fi

# On Apple (CLANG or Homebrew gcc), check for __sincos and __sincosf inline functions in the headers.
AC_CHECK_HEADERS([math.h], [
  AC_CHECK_DECLS([__sincos, __sincosf], [
    AC_DEFINE([HAVE___SINCOS], 1, [The __sincos function is present.])
    AC_DEFINE([HAVE___SINCOSF], 1, [The __sincosf function is present.])
  ], [], [#include <math.h>])
])

# The aocc compiler has optimized maths libraries that we should use. Check
# any clang for this support. Note do this after the basic check for maths
# as we need to make sure -lm follows. Also note needs -Ofast or -ffast-math
# so only when optimizing.
if test "$enable_opt" = "yes" -a "$ax_cv_c_compiler_vendor" = "clang"; then
   have_almfast="yes"
   AC_CHECK_LIB([almfast],[amd_fastexp],[LIBS="-fveclib=AMDLIBM -fsclrlib=AMDLIBM -lalmfast -lamdlibm $LIBS"],[have_almfast="no"],[-lamdlibm -lm])
   if test "$have_almfast" = "no"; then
      # Less optimized version.
      AC_CHECK_LIB([amdlibm],[sqrt],,,[-lm])
   fi
fi

# Check for glibc extension backtrace().
AC_CHECK_FUNCS([backtrace backtrace_symbols])

# Add warning flags by default, if these can be used. Option =error adds
# -Werror to GCC, clang and Intel.  Note do this last as compiler tests may
# become errors, if that's an issue don't use CFLAGS for these, use an AC_SUBST().
AC_ARG_ENABLE([compiler-warnings],
   [AS_HELP_STRING([--enable-compiler-warnings],
     [Enable compile time warning flags, if compiler is known @<:@error/no/yes)@:>@]
   )],
   [enable_warn="$enableval"],
   [enable_warn="error"]
)
if test "$enable_warn" != "no"; then

    # AX_CFLAGS_WARN_ALL does not give good warning flags for the Intel compiler
    # We will do this by hand instead and only default to the macro for unknown compilers
    case "$ax_cv_c_compiler_vendor" in
          gnu | clang | oneapi)
             CFLAGS="$CFLAGS -Wall -Wextra -Wno-unused-parameter -Wshadow"
          ;;
	  intel)
             CFLAGS="$CFLAGS -w2 -Wunused-variable -Wshadow"
          ;;

	  *)
	     AX_CFLAGS_WARN_ALL
	  ;;
    esac

    # Add a "choke on warning" flag if it exists
    if test "$enable_warn" = "error"; then
       case "$ax_cv_c_compiler_vendor" in
          intel | clang | oneapi)
             CFLAGS="$CFLAGS -Werror"
	  ;;
	  gnu)
             #  Fix for issue with IPO and GCC 14
             CFLAGS="$CFLAGS -Werror -Wno-alloc-size-larger-than"
          ;;
       esac
    fi

    # We want strict-prototypes, but this must still work even if warnings
    # are an error.
    AX_CHECK_COMPILE_FLAG([-Wstrict-prototypes],[CFLAGS="$CFLAGS -Wstrict-prototypes"],
                          [CFLAGS="$CFLAGS"],[$CFLAGS],[AC_LANG_SOURCE([int main(void){return 0;}])])
fi


# Hydro scheme.
AC_ARG_WITH([hydro],
   [AS_HELP_STRING([--with-hydro=<scheme>],
      [Hydro dynamics to use @<:@gadget2, minimal, pressure-entropy, pressure-energy, pressure-energy-monaghan, phantom, gizmo-mfv, gizmo-mfm, shadowswift, planetary, remix, sphenix, gasoline, anarchy-pu default: sphenix@:>@]
   )],
   [with_hydro="$withval"],
   [with_hydro="sphenix"]
)

case "$with_hydro" in
   none)
      AC_DEFINE([NONE_SPH], [1], [No hydro])
   ;;
   gadget2)
      AC_DEFINE([GADGET2_SPH], [1], [Gadget-2 SPH])
   ;;
   minimal)
      AC_DEFINE([MINIMAL_SPH], [1], [Minimal SPH])
   ;;
   pressure-entropy)
      AC_DEFINE([HOPKINS_PE_SPH], [1], [Pressure-Entropy SPH])
   ;;
   pressure-energy)
      AC_DEFINE([HOPKINS_PU_SPH], [1], [Pressure-Energy SPH])
   ;;
   pressure-energy-monaghan)
      AC_DEFINE([HOPKINS_PU_SPH_MONAGHAN], [1], [Pressure-Energy SPH with M&M Variable A.V.])
   ;;
   phantom)
      AC_DEFINE([PHANTOM_SPH], [1], [Phantom SPH])
   ;;
   gizmo-mfv)
      AC_DEFINE([GIZMO_MFV_SPH], [1], [GIZMO MFV SPH])
      need_riemann_solver=yes
      hydro_does_mass_flux=yes
   ;;
   gizmo-mfm)
      AC_DEFINE([GIZMO_MFM_SPH], [1], [GIZMO MFM SPH])
      need_riemann_solver=yes
   ;;
   shadowswift)
      AC_DEFINE([SHADOWSWIFT], [1], [ShadowSWIFT hydrodynamics])
      AC_DEFINE([MOVING_MESH_HYDRO], [1], [Moving mesh hydrodynamics])
      need_moving_mesh=yes
      need_riemann_solver=yes
      hydro_does_mass_flux=yes
   ;;
   planetary)
      AC_DEFINE([PLANETARY_SPH], [1], [Planetary SPH])
   ;;
   remix)
      AC_DEFINE([REMIX_SPH], [1], [REMIX SPH])
   ;;
   sphenix)
      AC_DEFINE([SPHENIX_SPH], [1], [SPHENIX SPH])
   ;;
   gasoline)
      AC_DEFINE([GASOLINE_SPH], [1], [Gasoline SPH])
   ;;
   anarchy-pu)
      AC_DEFINE([ANARCHY_PU_SPH], [1], [ANARCHY (PU) SPH])
   ;;

   *)
      AC_MSG_ERROR([Unknown hydrodynamics scheme: $with_hydro])
   ;;
esac




# Handle .in files.
AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile])

# Save the compilation options
AC_DEFINE_UNQUOTED([SWIFT_CONFIG_FLAGS],["$swift_config_flags"],[Flags passed to configure])

#  Need to define this, instead of using fifth argument of AC_INIT, until
#  2.64. Defer until now as this redefines PACKAGE_URL, which can emit a
#  compilation error when testing with -Werror.
AC_DEFINE([PACKAGE_URL],["https://github.com/mladenivkovic/swiftgpupacksim"], [Package web pages])

# Generate output.
AC_OUTPUT

# Report general configuration.
AC_MSG_RESULT([
 ------- Summary --------

   $PACKAGE_NAME v.$PACKAGE_VERSION

   Compiler             : $CC
    - vendor            : $ax_cv_c_compiler_vendor
    - version           : $ax_cv_c_compiler_version
    - flags             : $CFLAGS
   Special allocators   : $have_special_allocator
   Pthread barriers     : $have_pthread_barrier
   Atomic operations    : $enable_atomics_within_tasks
   Debugging checks     : $enable_debugging_checks
   Hydro scheme         : $with_hydro

 ------------------------]
)

#  Report any unrecognised options loudly.
if test "$ac_unrecognized_opts" != ""; then
AC_MSG_RESULT([
  *** WARNING: unrecognized options: $ac_unrecognized_opts ***
])
fi
