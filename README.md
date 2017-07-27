# wireCAM

This is a cam tool originally designed for hot wire cutting foam with a 4-axis cnc machine, 
but the framework is extensible to other machine types.

The code is currently targeting a 3 axis gantry with a rotary axis, the 4th machine axis, mounted where a spindle would usually go.
Heated wire loops were mounted on the 'fourth axis'.  A toolpath for this machine requires
both a path in space and a wire orientation which is determined by the normal of the surface being cut.

# Disclaimer

Use at your own risk!


## Dependencies

 - oce - open cascade community edition (0.16.1 or greater?)
 - cmake
 - gtest


## Building the application

Go the the project directory. Make a directory called something like build

    cd build
    cmake ..
    
Now in you should be able to execute the test suite
    
    ./test
    

## Workflow

The real workhorse of this application is a binary executable file, wireCAM.  It should show up in the build directory
after compilation. wireCAM is invoked with one argument, the name of the input file, which is a json file.  The main
json inputfile may in turn point to other cad files.  STEP and BREP files are the primary file types supported now but 
in some places only STEP can be used and in others, only BREP.  That can and should be fixed...
Until farther notice, the best way to learn the input file format is to look at things in the test directory.

Output consists of several files.  One is the gcode file.  Another is a STEP or BREP (don't remember at the moment! the output 
type could be made configurable with a little coding) file that will contain a series of wires (wires in a 3D geometric model sense) that show a preview of the cutting paths that were generated.

## About the Algorithms

### Surface Profiling
Here a heated loop of wire with shape of the outline of the silhouette of a ball nose endmill is assumed.

wireCAM will read a cad file containing some kind of geometry.  Generally it's good to extract the surface of interest and
export it to its own file.  There's a bounding box functionality too that will allow you to pick a section of a model, if you
want to get tricky.

In the input file the user specifies slicing planes with a normal direction and spacing.  The intersection between slicing planes
and the surfaces is found and these wires are the basis of the tool paths to be computed.  The wire orientation
is computed at each point base tessilation of each wire.  "Operations" are then applied to make refinements based on different criteria, for example, the distance from point to point must be less that a thershold value, the 4 axis angle must not change
excessively from point to point ect.  There's an operation base class and new rules can be made by making a class that inherets it.  Operations and their parameters are selectable in the input file.

### Pocketing

Here the input cad files contain faces.

### knife cutting

This is a new feature to be implemented.  Here the input cad files will contain any ammount wires (a series of connected edges).
The base tessilation of each wire will be used to compute x,y,z points and a knife angle.  Operations will then be perforemed for
refinement.
