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
		Platform::String^ m_filename;
		Windows::Storage::StorageFolder^ m_selectedFolder;

		Platform::String^ GetSaveFile();
		Windows::Storage::StorageFolder^ GetStorageFolder();

		void SetStorageFolder(Windows::Storage::StorageFolder^ folder);
		void PickAFileButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void PickAFolderButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void RunConversionButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
