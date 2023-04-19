# ConsoleEngine
 CPU based software rasterizer, built from scratch using only c++ std libs and the windows api.

## Controls
WASD to move, J and L to look around, Q and E to change elevation, F and H to change speed, G to reset speed

## TODO-list for later:
- refactor the code, split it into more files / functions, make it reusable, optimise to the best of my ability
- add the option to dynamically create/change meshes (the vertices and triangles array)
- render triangle faces and not just edges
- create ingame console to enter commands for exiting, changing global variables, and modifying meshes
- add config files for global variables, crosshair, font, ...
- add files to specify what meshes should be preloaded into the scene (currently hardcoded in two arrays)
