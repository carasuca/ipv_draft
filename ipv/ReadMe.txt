========================================================================
    DYNAMIC LINK LIBRARY : ipv Project Overview
========================================================================

Image Processing and Visualisation Framework (ImProVise, IPV) library.
The library is dynamically linked with framework application and all plugins.

Consists of the following headers:

ipv.h            - main header includes all other headers
ipv/config.h     - macros for conditional configuration
ipv/framework.h  - interface definition
ipv/plugin.h     - utilities for plugins
ipv/image.h      - image structure used throughout the framework

ipv/internal.h - used by the framework, not included in plugin SDK

                   ############################
                   ######      DONE       #####
                   ############################

static framework connection with instane()/inject(framework_i*, enum action = read)
NOTE: what is a better name for injecting the context?



                   ############################
                   ######      BUGS       #####
                   ############################

Note that ipv.dll is loaded statically by plugins and mainframe 
- they have to reside in the same directories


                   ############################
                   ######      TODO       #####
                   ############################

# GUI
- consider using Qt
- check C++/CLI integration, how to implement unmanaged C++ interfaces in CLI, C#

# API
- add parameter/string conversion
- add ipv::run(string cmd) or ipv::cmd(string)
- - the console interface will just generate command_i:data_i to framework_i::set()
- add argv parser so plugin authors can use it.

# CLI
- add user options to ask for parameters
- "verbose:0"
- add assignment operator
- add expression parsing with brackets/braces
- add logging
- - just set(L"message", L"log") // stdlog
- - just set(L"message", L"err") //stderr
- - just get(data_i(string), L"cin") to read a string

# IMAGE
- simple structure with *easy* syntax
- based on std::vector or local ipv::vector implementation
- add walker helper object
- - operate directly on image_i
- how to convert between depths?
- - maybe a function array,
- - union of pointers?
- - add a "functionized" callback copy_n + pass lambdas
- - 

# IMAGE IO
- use WIC for transport as part of API
- how to deal with format specifics (rgb - bgr)?
- - static load(image_i&, int* opt_flags = NULL)
- - maybe add these flags to image interface

# MP
- design guards, no more infinite locks!
- throw on deadlock? or fail gracefully
- 

# PLUGINS
- add extensions:
1. trivial "main" plugin = static/unbound
- dllexport macros, search for main/wmain

2. named static plugins
3. add dynamic, bounded plugin interface
- move to plugins/ folder and don't rely on statically loaded ipv.dll
- - might be hard, because ipv::set cannot use ipv::instance() anymore
4. specialize interfaces: importers, shaders, 
5. add MP support: buckets / block processors
- will require shared image access and ROI definition
6. raytrace_i - for rendering per pixel based on some model


