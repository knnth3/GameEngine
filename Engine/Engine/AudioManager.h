//#pragma once
//#include <SDL2\SDL.h>
//#include <string>
//#include "Shared.h"
//
//namespace Lime
//{
//
//    class AudioManager
//    {
//    public:
//        AppDLL_API bool LoadWAV(std::string filePath);
//        AppDLL_API void Play(bool statement);
//        AppDLL_API bool ClearVertexBuffers();
//    private:
//        static void MyAudioCallback(void * userData, Uint8 * stream, int streamLength);
//        bool CreateAudioDevice();
//
//        struct AudioData
//        {
//            Uint8* position;
//            Uint32 length;
//        };
//
//        SDL_AudioDeviceID mAudioDeviceID;
//        SDL_AudioSpec wavSpec;
//        Uint8* wavStart;
//        Uint32 wavLength;
//        AudioData audio;
//    };
//
//
//}