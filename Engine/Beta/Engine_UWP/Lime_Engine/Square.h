#pragma once
#include "DllSettings.h"
#include <Graphics\Model.h>


namespace LIME_ENGINE
{
	namespace Shapes
	{
		class Square:
			public Graphics::Model
		{
		public:
			ENGINE_DLL_API Square();
			ENGINE_DLL_API Square(float length, float width);

			//User functions
			ENGINE_DLL_API void SetDimensions(float length, float width);
			ENGINE_DLL_API void SetLength(float length);
			ENGINE_DLL_API void SetWidth(float width);
			ENGINE_DLL_API void Wireframe(bool value);

			//Overloaded functions
			ENGINE_DLL_API virtual void Scale(float length, float width);

		private:
			void LoadMesh();
			void ScaleToDimensions();


			//Static variables
			static Graphics::MeshID m_meshID;

			//Local variables
			float m_length;
			float m_width;

		};
	}
}

