#pragma once
#include "DllSettings.h"
#include <Graphics\Model.h>



namespace LIME_ENGINE
{
	namespace Shapes
	{
		class Cube:
			public Graphics::Model
		{
		public:
			ENGINE_DLL_API Cube();
			ENGINE_DLL_API Cube(float length, float width, float height);

			//User functions
			ENGINE_DLL_API void SetDimensions(float length, float width, float height);
			ENGINE_DLL_API void SetLength(float length);
			ENGINE_DLL_API void SetWidth(float width);
			ENGINE_DLL_API void SetHeight(float height);
			ENGINE_DLL_API void Wireframe(bool value);

			//Overloaded functions
			ENGINE_DLL_API virtual void Scale(float length, float width, float height);

		private:
			void ScaleToDimensions();

			float m_length;
			float m_width;
			float m_height;
		};

	}
}