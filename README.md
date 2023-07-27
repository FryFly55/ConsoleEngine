# (not any more Console-)Engine
CPU based software rasterizer, built from scratch using only c++ std libs and the windows api.

At least that was true a while ago, until windows decided to deprecate some old api features, which
lead to me having to resort to openGL. I only use it to draw pixels to the screen though, no use is
made of the render pipeline, I only use ```glDrawPixels()``` to draw my own buffers to the screen. All the 
other code is still either mine or the std libs of c++

## Controls
WASD to move, Q/E to change elevation, and either the mouse/touchpad or the J/L keys to look around.
touchpads can sometimes be buggy, where you will look around very slowly, since the raw motion is never 0.
I will test if this Issue persists with mice and fix it if it does. Idc for touchpads that much.

## TODO-list for later:
- face rendering
- two pipelines, one "ram-efficient" one where each triangle goes through the whole pipeline after one
another, and one where triangles can actually affect each other since all vertices in the different stages
are in ram at the same time.
- read scene data from files
- write scene data to files
- make ```drawPixelMaps()``` be able to draw .png and .jpg (this one will get tricky if I stick to the no
external libraries rule)

