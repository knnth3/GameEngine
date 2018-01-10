# Game-Engine
This project is an engine that currently uses the DirectX 11 API.

It's a very basic implementation that is work in progress.
To test it out, clone and open the sln file in Visual Studios.

#Included in Root:
-Networking model that uses UDP sockets.
-BinaryModelConverter that converts obj, 3ds, fbx and collada files using FBX sdk to a custom format Engine can read.
-Populace is an api to handle RPG game like interations and information.

#Supported Building of x64 only
#*************Engine Documentation*************

Engine is a Dll based graphics api based on the DirectX 11 interface.
This means that this program can only run on Windows OS.
-It is possible to create a renderer that does not use DirectX 11 although a new RenderBatch must be created (More info Below)

#-----Standard Build----------------------------------------------------------------------------------------------------------
-In the main function, a GraphicsWindow is created.
int main(){
	int val = 0;
	WindowApp window("My Application");
	if (window.Initialize())
	{
		val = window.Run();
	}
	return val;
}
*The Run() function returns when the window closes.
-The return value currently depends on the post close function for windows

*The WindowApp is a user made class that inherits "GraphicsWindow"
-GraphicsWindow requires a few functions to be present in WindowApp for it to work.
-The implementation of those are completely user defined and are called by the WindowApp public functions like Initialize() and Run().
#Required:
virtual void Update(void);
virtual void Render(const std::shared_ptr<Graphics::GraphicsDevice>& graphics);
virtual void Resume(void);
virtual void Suspend(void);

*When using the GraphicsWindow, you also gain access to the WindowResources static class.
-This contains all of the nessesary interfaces that the window creates.
#Included by default:
Input
Timer
Graphics (also passed as an argument in the Render() function)

-If the user would like to add other "Add-ons" to the WindowResources, there are functions provided to do so.
-Keep in mind that anything given to WindowResources becomes "Global" and needs to be thread safe to avoid any undefined behavior.
-More info can be found by hovering over the WindowResources object (in VS) or looking up the specified header file.

#-----UWP Build-------------------------------------------------------------------------------------------------------------------
*It is possible to build as a UWP project although some modifications may need to be made.
*WindowApp cannot be used if building as UWP
-This means that WindowResources must be included in the self-implementation and needs to handle passed data accordingly 
(releasing it's data before closing graphics down)
#Features that can be added to the user-definded Window Implementation:
-Input
-Graphics(required HWND)
-Timer
*GraphicsDevice should still be able to work if you pass the apps HWND to it. (later on I may add a separate function to GraphicsDevice to allow for a CoreWindow instead)

#-----Custom GraphicsDevice-------------------------------------------------------------------------------------------------------
*It is possible to create a custom GraphicsDevice and pass it in at run time if desired
-Keep in mind it may be better to also create a new suitable window class
-I will later on make the GraphicsDevice virtual and easily inheritable to then be able to implement a new renderer like opengl
-Any new Graphics renderer can be passed to the WindowResources static class and be used throught the app

#-----Custom Input-----------------------------------------------------------------------------------------------------------------
*Any new class impementations derived of InputDevice can be passed to the Input class AttatchKeyboard() function and will be used instead of the default implementation
-Mouse input cannot yet be replaced but will be in the future.


#**********************Usage******************************

*Engine supports both 3D and 2D rendering and the draw calls do not need to be in a specific order
-2D rendering draws according to the painter's algorithm
-3D rendering draws behind 2D

*include <GraphicsWindow>
#Objects that can be drawn: (in Graphics namespace)
2D--
Line
Square
Text
3D--
Model

-3D custom objects can be created/loaded through the MeshLoader static class (reads in .bin files created by the Binary Model Converter in root).
-These objects pass a MeshID when loaded/Created that can be given to a Model using SetMesh() function.
-By default, a cube will be loaded as a place holder.

-These objects can only be drawn in the Render() function!
Ex: (order from back to front = model->text->square->line)
void WindowApp::Render(const std::shared_ptr<Graphics::GraphicsDevice>& graphics)
{
	graphics->Draw(text);
	graphics->Draw(square);
	graphics->Draw(model);
	graphics->Draw(line);
}

-Each drawable object has similar set functions to control the color, rotation, scale, position, etc.
-In addition, the camera can be attatched to any 3D model and will then follow it. (can be obtained through graphics->GetCamera())

*Textures, currently disabled for 3D (working on new api)
-Textures for 2D can be obtained through the BrushManager static class *include<BrushManager.h>
-Options are avaiable there to give an object colors, and textures. 
-The interface works like the MeshLoader in that the BrushManager gives an ID to access it's resources
-The ID is given in a std::string that can be passed to a 2D object to be used.

*Multiple/Custom Shaders currently disabled (working on new api)
-Effects for 2D can be created using the Effects_2D virtual class *include <Effects_2D.h>
-Using the CreateFunction, you may chain effects by setting different m_beginEffect and m_finalEffect.
-Effects are added to the BrushManager as well to be applied and are cleared from there as well

*Wireframe
-Wireframe can be enabled as an option through the GraphicsDevice

*Text
-Text inherits from std::string and can easily be passed a string to display
-The settings are not changed with new string values passed

*Custom Data Structures
-Library (*include <Library.h>) is a template class that stores and handles objects creation and destruction using shared pointers.
-Univeral_map (*include <universal_map.h>) is a unordered_map that stores any object wraped in a shared pointer using universal_struct.

#**********************Demo**************************
-The current version in the root is the most stable version of the Engine although it has not been updated in a while and may not include the features listed above
-The new version will be updated very soon to match the documentation
-To access the newest version of Engine, navigate to Root->Engine->Beta->Engine->Engine.sln
-This sln will include another project other than Engine that can be set as "start up project" and build as x64.
-These additional projects are extremely simple and used to test new features in Engine.

#---------Previous Versions--------------------------
-Previous versions can be found in the beta folder as well but they are also not guaranteeed to work as the documentation states.
-Because the Engine is in very early development, drastic changes might still be made in the way the api works.
(This is a learing experience for me to try to better my coding knowledge)

#---------Engine UWP in Beta-------------------------
-There is an already built Engine for UWP and does follow close to the Engine sln in Beta although it has a D3D memory leak and is not adivsed to run.
-The Engine here is named Graphics and has a subsequent dll called Lime_Engine that runs in addition to Graphcis. 
(This was changed to a single dll in the most recent version of Engine in Beta)