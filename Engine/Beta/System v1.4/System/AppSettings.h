#pragma once
#include "DllSettings.h"
#include <NVIDIA_GSA\NvGsa.h>


namespace System
{
	class AppSettings
	{
	public:
		AppSettings(std::string programPath);
		~AppSettings();

		bool Initialize();

		void printGsaVersion(const NvGsaVersion *version);

		bool initApplication(NvGsaApplication *app, const wchar_t *exePath);
		void releaseApplication(NvGsaApplication *app);

		bool initOptions(const NvGsaNamedOption **options, size_t *numOptions);
		void releaseOptions(const NvGsaNamedOption *options);

		bool initResolutions(const NvGsaResolution **resolutions, size_t *numResolutions);
		void releaseResolutions(const NvGsaResolution *resolutions);

		void printOptionValue(const NvGsaVariant *value, const NvGsaNamedOption *option);
		void cycleOptionValue(NvGsaVariant *result, const NvGsaVariant *value, const NvGsaNamedOption *option);

		void printResolution(const NvGsaResolution *resolution);
		void cycleResolution(NvGsaResolution *resolution, const NvGsaResolution *availableResolutions, size_t numResolutions);

	private:
		std::wstring ToWstring(std::string str);

		std::string m_programPath;
		const NvGsaNamedOption *options = NULL;
		size_t numOptions = 0;
		const NvGsaResolution *resolutions = NULL;
		size_t numResolutions = 0;

		NvGsaApplication app;
		NvGsaStatus status;
	};

}
