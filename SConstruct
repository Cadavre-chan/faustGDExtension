#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")

# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=["src/"])
sources = Glob("src/*.cpp")

# Add the include paths for the godot-cpp headers (including subdirectories)
godot_include_path = os.path.abspath("godot-cpp/include")

# Use os.walk to recursively include all subdirectories inside godot-cpp/include
for dirpath, dirnames, filenames in os.walk(godot_include_path):
    # Add each directory to the include path
    env.Append(CPPPATH=[dirpath])

godot_include_path_bis = os.path.abspath("godot-cpp/gen")

for dirpath, dirnames, filenames in os.walk(godot_include_path_bis):
    # Add each directory to the include path
    env.Append(CPPPATH=[dirpath])

# This block will output the Compilation Database (compile_commands.json)
env['CCFLAGS'] += ['-g', '-Wall']  # Optionally add flags for debugging or warnings
env['CXXFLAGS'] += ['-g', '-Wall']  # Optionally add flags for debugging or warnings

# Explicitly specify the target path for the compile_commands.json file
compile_commands_path = os.path.abspath("compile_commands.json")

# Generate the compilation database (compile_commands.json)
compile_command = env.Command(compile_commands_path, [], [
    'clang -c $SOURCE -o $TARGET -I%s' % godot_include_path
])


# Generate the compilation database (compile_commands.json) without source files as input
# Just use a dummy file (empty list) to generate compile_commands.json without the conflicting source files
env.Depends(compile_command, sources)

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "demo/bin/libgdexample.{}.{}.framework/libgdexample.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
elif env["platform"] == "ios":
    if env["ios_simulator"]:
        library = env.StaticLibrary(
            "demo/bin/libgdexample.{}.{}.simulator.a".format(env["platform"], env["target"]),
            source=sources,
        )
    else:
        library = env.StaticLibrary(
            "demo/bin/libgdexample.{}.{}.a".format(env["platform"], env["target"]),
            source=sources,
        )
else:
    library = env.SharedLibrary(
        "demo/bin/libgdexample{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)
