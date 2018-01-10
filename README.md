# Game-Engine
This project is an engine that currently uses the DirectX 11 API.

It's a very basic implementation that is work in progress.
To test it out, clone and open the sln file in Visual Studios.

**Included in Root:**
- Networking model that uses UDP sockets.
- BinaryModelConverter that converts obj, 3ds, fbx and collada files using FBX sdk to a custom format Engine can read.
- Populace is an api to handle RPG game like interations and information.

**Supported Building of x64 only**

----------------------Demos-----------------------
===================================================
The current version in the root is the most stable version of the Engine although it has not been updated in a while and may not include the features listed above.
The new version will be updated very soon to match the documentation.
To access the newest version of Engine, navigate to Root->Engine->Beta->Engine->Engine.sln.
This sln will include another project other than Engine that can be set as "start up project" and build as x64.
These additional projects are extremely simple and used to test new features in Engine.

Previous Versions
-----------------
Previous versions can be found in the beta folder as well but they are also not guaranteeed to work as the documentation states.
Because the Engine is in very early development, drastic changes might still be made in the way the api works.
*(This is a learing experience for me to try to better my coding knowledge)*

Engine UWP in Beta
------------------
There is an already built Engine for UWP and does follow close to the Engine sln in Beta although it has a D3D memory leak and
is not adivsed to build and run.
The Engine here is named Graphics and has a subsequent dll called Lime_Engine that runs in addition to Graphcis. 
*(This was changed to a single dll in the most recent version of Engine in Beta)*

