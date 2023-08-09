# (not any more Console-)Engine
CPU based software rasterizer, built from scratch using only c++ std libs and the windows api.

At least that was true a while ago, until windows decided to deprecate some old api features, which
lead to me having to resort to openGL. I only use it to draw pixels to the screen though, no use is
made of the render pipeline, I only use ```glDrawPixels()``` to draw my own buffers to the screen. All the 
other code is still either mine or the std libs of c++

## Controls
**WASD** to move, **Q/E** to change elevation, and either the **mouse/touchpad** or the **J/L** keys to 
look around.

## Open bugs for me to fix
- the touchpad/mouse controls are buggy, where even if you don't move your mouse you will always slowly look
around.
- lines disappear when the vertex they are being drawn to goes out of frame. (But only sometimes??)

## TODO-list for later:
- face rendering
- two pipelines, one "ram-efficient" one where each triangle goes through the whole pipeline after one
another, and one where triangles can actually affect each other since all vertices in the different stages
are in ram at the same time.
- read scene data from files
- write scene data to files
- make ```drawPixelMaps()``` be able to draw .png and .jpg (this one will get tricky if I stick to the no
external libraries rule)

