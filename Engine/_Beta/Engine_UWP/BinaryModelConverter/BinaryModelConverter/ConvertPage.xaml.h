//
// ConvertPage.xaml.h
// Declaration of the ConvertPage class
//

#pragma once

#include "ConvertPage.g.h"
#include "MainPage.xaml.h"

namespace BinaryModelConverter
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ConvertPage sealed
	{
	public:
		ConvertPage();

	private:
		MainPage^ rootPage;
		Windows::Storage::StorageFile^ m_file;
		Windows::Storage::StorageFile^ m_temp;
		Windows::Storage::StorageFolder^ m_selectedFolder;

		Windows::Storage::StorageFile^ GetOriginalFile();
		Platform::String^ GetSaveFile();
		Windows::Storage::StorageFolder^ GetStorageFolder();
		bool CopyContentToTemp();
		void LoadTempFile();

		std::wstring ParseFilePath(std::wstring path);
		void SetStorageFolder(Windows::Storage::StorageFolder^ folder);
		void PickAFileButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void PickAFolderButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void RunConversionButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
