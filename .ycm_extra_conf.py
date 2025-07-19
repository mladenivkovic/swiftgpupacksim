import os

DIR_OF_THIS_SCRIPT = os.path.abspath( os.path.dirname( __file__ ) )


def _add_include(path, include="include"):
    """
    Add the "include" subdir to a path. E.g.
    /some/path -> /some/path/include
    """
    return os.path.join(os.path.abspath(path), include)


def _parse_config_h(srcdir=DIR_OF_THIS_SCRIPT):
    """
    SWIFT has a ton of header files, and YCM/clangd can't really
    figure out all the include structure if you just open the
    header file to work with.
    So something that's chronically missing is all the macros in
    the config.h
    This parses the config.h file and returns all the defined
    macros as cmdline defines.

    returns: defines
        list of compiler cmdline flags in the form of -DMYVAR=X
    """

    conffile = os.path.join(srcdir, "config.h")
    if not os.path.exists(conffile):
        raise Warning("No config.h file in", conffile)
        return []

    defines = []

    filedata = open(conffile, "r")
    lines = filedata.readlines()

    for line in lines:
        if not line.startswith("#define"):
            continue

        l = line.strip()
        splits = l.split()
        val = "".join(splits[2:])
        newdef = "".join(("-D", splits[1], "=", val))

        defines.append(newdef)

    filedata.close()

    return defines


try:
    COMPILER = os.environ["CC"]
except KeyError:
    COMPILER = "gcc"

flags = [
    '-Wall',
    '-Wextra',
    # THIS IS IMPORTANT! Without the '-x' flag, Clang won't know which language to
    # use when compiling headers. So it will guess. Badly. So C++ headers will be
    # compiled as C headers. You don't want that so ALWAYS specify the '-x' flag.
    # For a C project, you would set this to 'c' instead of 'c++'.
    '-x', 'c',
    '-Wno-unused-includes',
    '-Wno-unused-parameter',
    '-Wno-unused-function',
    '-std=c11',
    '-DHAVE_CUDA=1', # cheat for the bloody linter
    '-DCONFIG_LOCAL_HP_ICX=1',
]


include = [
    "-I", DIR_OF_THIS_SCRIPT,       # add config.h
    "-I", os.path.join(DIR_OF_THIS_SCRIPT, "src"),
        ]

try:
    CUDA_PATH = os.environ["CUDA_PATH"]
    include.append("-I")
    include.append(os.path.join(CUDA_PATH, "include"))
except KeyError:
    try:
        # cosma
        CUDA_PATH = os.environ["NVHPC_ROOT"]
        include.append("-I")
        include.append(os.path.join(CUDA_PATH, "cuda", "include"))
    except KeyError:
        pass




def Settings( **kwargs ):

    if kwargs[ 'language' ] == 'cfamily':

        #  all_defines = _parse_config_h()
        all_defines = []
        all_includes = include

        # add current directory of the file
        fname = kwargs[ "filename" ]
        absfname = os.path.abspath(fname)
        fdir = os.path.dirname(absfname)
        all_includes.append("-I")
        all_includes.append(fdir)

        #  for lib in libs:
        #      all_includes.append("-I")
        #      all_includes.append(_add_include(lib))

        final_flags = flags + all_defines + all_includes

        return {
            'flags': final_flags,
          }
