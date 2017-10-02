//#pragma once
//#include <SDL2\SDL.h>
//#include <string>
//#include "Primitives.h"
//
//namespace Lime
//{
//
//    class AudioManager
//    {
//    public:
//        DLL_EXPORT bool LoadWAV(std::string filePath);
//        DLL_EXPORT void Play(bool statement);
//        DLL_EXPORT bool Close();
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