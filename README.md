## Anrhem HSLab rig

Read color and depth feeds from D435(i) camera and pump them through NDI.

You will need `openFrameworks v0.10.1` and the [IDArnhem/ofxRealsense](https://github.com/IDArnhem/ofxRealsense) addon.

Program contains a simple ImGui UI that uses aconfig file. Cannot run headless yet.

To build on Jetson Nano you must use `make` the qbs build doesn't seem to work.


