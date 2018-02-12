# Game-Engine
This project contains programs contains a game engine as well as complimentary dlls to make a multiplayer game.
It's a very basic implementation that is work in progress.
To test it out, clone and open the sln file in Visual Studios.

**Included in Root:**
- Net Module that uses UDP sockets to allow multiplayer connections via server-peer connections.
- SEF Model Converter that converts obj, 3ds, fbx and collada files to .sef using FBX sdk and Assimp.
- Populace is an api to handle RPG game like interations and information.
- Engine built in c++ with DirectX 11 and Direct2D.

**Supported Building of x64 only**

----------------------Demos-----------------------
===================================================
The current version is in the root for all current projects relevant to the Engine.
To open and try out any projects in the above, locate the .sln file in the respective folder name.
At least one test project should be included in each solution. These are extremely simple and offer an easy to understand layout of the project.

Previous Versions
-----------------
Previous versions can be found in the Previous Versions folder in the root. They are not guaranteeed to work as the documentation states.
Because the Engine is in very early development, drastic changes might still be made in the way the api works.
*(This is a learing experience for me to try to better my coding knowledge)*

Engine UWP in Beta
------------------
There is a built Engine for UWP and does follow close to Engine although it has a D3D memory leak and
is not adivsed to build and run.
The Engine here is named Graphics and has a subsequent dll called Lime_Engine that runs in addition to Graphics. 
*(This was changed to a single dll in the most recent version of Engine in Beta)*
The UWP solution is located in the Engine directory inside the Beta folder.

----------------------Notes-----------------------
===================================================
**All projects provided are written in C++ and written using Visual Studios 2015-2017.**
The Engine uses .sef files to represent 3D models which are created by using the SEF model converter.