#pragma once
#include "Primitives.h"

namespace Lime
{
    class ModelLoader
    {
    public:
        DLL_EXPORT static std::shared_ptr<ModelData> LoadModel(std::string filename, unsigned int id);
    private:
        static unsigned int AssignID();
    };


}