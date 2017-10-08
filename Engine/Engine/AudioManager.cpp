//#include "AudioManager.h"
//
//namespace Lime
//{
//
//    void AudioManager::MyAudioCallback(void * userData, Uint8 * stream, int streamLength)
//    {
//        AudioData* audio = (AudioData*)userData;
//        if (audio->length == 0)
//        {
//            return;
//        }
//        Uint32 length = (Uint32)streamLength;
//        if (length > audio->length)
//        {
//            length = audio->length;
//        }
//        SDL_memcpy(stream, audio->position, length);
//
//        audio->position += length;
//        audio->length -= length;
//    }
//
//    bool AudioManager::CreateAudioDevice()
//    {
//        bool initiated = true;
//        mAudioDeviceID = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, SDL_AUDIO_ALLOW_ANY_CHANGE);
//        if (mAudioDeviceID == 0)
//        {
//            initiated = false;
//            // TODO: Could not find Audio Device to use.
//        }
//        return initiated;
//    }
//
//    bool AudioManager::LoadWAV(std::string filePath)
//    {
//        if (SDL_LoadWAV(filePath.c_str(), &wavSpec, &wavStart, &wavLength) == NULL)
//        {
//            // TODO: correctly error handle.
//            return false;
//        }
//
//        audio.position = wavStart;
//        audio.length = wavLength;
//        wavSpec.callback = AudioManager::MyAudioCallback;
//        wavSpec.userdata = &audio;
//
//        return CreateAudioDevice();
//    }
//
//    void AudioManager::Play(bool statement)
//    {
//        try
//        {
//            SDL_PauseAudioDevice(mAudioDeviceID, !statement);
//        }
//        catch (const std::exception&)
//        {
//            const char* error = SDL_GetError();
//        }
//    }
//
//    bool AudioManager::ClearVertexBuffers()
//    {
//        SDL_CloseAudioDevice(mAudioDeviceID);
//        SDL_FreeWAV(wavStart);
//        return false;
//    }
//
//}