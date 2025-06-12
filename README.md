# FaustGDExtension - an integration of Faust in Godot

## Grame, Centre National de Creation Musicale: <https://www.grame.fr>

Build status: 

## Introduction

Introducing custom audio effects to Godot can be a bit tiresome, this GDExtension allows you to dynamically load any audio effect / DSP and add it to an audio bus of your choice, provided that it comes compiled as a shared object and has been made using Faust / respects Faust's C++ standards.

Please note that this is a very early build (not named yet). As such, using this extension on your system may require some tinkering.

## Versions and Branches

The only relevant branche at the moment is the main one. Future commits might share a pre-compiled version of the extension for ease of use, as well as Windows and MacOS ports.

## Overview of the repository

This is an overview of the content of this repository. If a file / folder isn't mentionned here, it is either not important enough, too self-explanatory, or part of the universal GDExtension architecture.

	./src/                 : extension C++ source code
	./compile.sh           : compilation script, will be replaced
	./demo                 :  demo Godot project for the extension
	./godot-cpp/           : Godot C++ bindings submodule
    ./demo/dsp/faust/      : Faust's official repository submodule


## Compilation and Installation

At the moment, compilation is done using a simple bash script, though it will be replaced in the future by a more robust build system such as CMake.

## Using the extension

Once the extension is compiled, you should be able to launch the Godot project in the demo folder. This project contains a script that you're going to update with the path of your DSP. After launching the "game" you'll notice a smaller window within the main one, containing all the possible controls of your DSP (providing that the second argument of loadDSP is set to true). At this point, the example audio file contained within this repository should play while having your DSP applied to it.

## Documentation and Resources

* `/documentation/faust-quick-reference.pdf` contains the most up-to-date documentation of Faust.
* `/libraries/doc/library.pdf` contains the documentation of the Faust DSP libraries.
* The [Faust website](https://faust.grame.fr) contains useful resources around Faust.
* The [Faust online course](https://ccrma.stanford.edu/~rmichon/faustWorkshops/course2015/) or the [Faust Hero in 2 Hours tutorial](https://ccrma.stanford.edu/~rmichon/faustTutorials/#faust-hero-in-2-hours) might be good starting points for beginners to learn Faust.
* [Julius Smith's website](https://ccrma.stanford.edu/~jos) is an incredible resource on various topics around DSP and Faust.
* [Official GDExtension documentation](https://docs.godotengine.org/fr/4.x/tutorials/scripting/gdextension/index.html) is the place to look at if you have trouble making the extension work

## Acknowledgements

This project was started by Axel Chypre in the context of a 4 months long 3rd year internship in april of 2025, under the supervision of Stéphane Letz.

## Questions and suggestions

If you have questions suggestions and comments, or if you want to
contribute to the project, feel free to send an e-mail to the following adress:
axel.chypre@epitech.eu, or alternatively, letz@grame.fr
