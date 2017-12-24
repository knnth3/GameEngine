//
// ConvertPage.xaml.cpp
// Implementation of the ConvertPage class
//

#include "pch.h"
#include "ConvertPage.xaml.h"


using namespace BinaryModelConverter;
using namespace std;
using namespace concurrency;
using namespace Platform;
using namespace Windows::Storage;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage::Provider;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

ConvertPage::ConvertPage()
{
	InitializeComponent();
	rootPage = MainPage::Current;
	m_selectedFolder = nullptr;
	PickAFileButton->Click += ref new RoutedEventHandler(this, &ConvertPage::PickAFileButton_Click);
	PickAFolderButton->Click += ref new RoutedEventHandler(this, &ConvertPage::PickAFolderButton_Click);
	RunConversionButton->Click += ref new RoutedEventHandler(this, &ConvertPage::RunConversionButton_Click);
}

Platform::String^ BinaryModelConverter::ConvertPage::GetSaveFile()
{
	String^ result = "";
	auto folder = GetStorageFolder();
	if (!folder)
		return result;

	if (!m_filename)
		return result;

	result = folder->Path + "\\" + m_filename + ".bin";
	return result;
}

Windows::Storage::StorageFolder ^ BinaryModelConverter::ConvertPage::GetStorageFolder()
{
	return m_selectedFolder;
}

void BinaryModelConverter::ConvertPage::SetStorageFolder(Windows::Storage::StorageFolder ^ folder)
{
	m_selectedFolder = folder;
}

void ConvertPage::PickAFileButton_Click(Object^ sender, RoutedEventArgs^ e)
{

	FileOpenPicker^ openPicker = ref new FileOpenPicker();
	openPicker->ViewMode = PickerViewMode::List;
	openPicker->SuggestedStartLocation = PickerLocationId::DocumentsLibrary;
	openPicker->FileTypeFilter->Append(".obj");
	openPicker->FileTypeFilter->Append(".3ds");

	create_task(openPicker->PickSingleFileAsync()).then([this](StorageFile^ file)
	{
		if (file)
		{
			OutputTextBlock->Text = "Picked photo -> " + file->Name;
			PickAFileButtonResult->Text = file->Name;
			m_filename = file->DisplayName;
		}
	});
}

void BinaryModelConverter::ConvertPage::PickAFolderButton_Click(Platform::Object ^ sender, Windows::UI::Xaml::RoutedEventArgs ^ e)
{

	FolderPicker^ folderPicker = ref new FolderPicker();
	folderPicker->ViewMode = PickerViewMode::List;
	folderPicker->SuggestedStartLocation = PickerLocationId::DocumentsLibrary;
	folderPicker->FileTypeFilter->Append(".bin");

	create_task(folderPicker->PickSingleFolderAsync()).then([this](StorageFolder^ folder)
	{
		if (folder)
		{
			OutputTextBlock->Text = "Picked folder -> " + folder->Name;
			PickAFolderButtonResult->Text = folder->Name;
			SetStorageFolder(folder);
		}
	});
}

void BinaryModelConverter::ConvertPage::RunConversionButton_Click(Platform::Object ^ sender, Windows::UI::Xaml::RoutedEventArgs ^ e)
{
	String^ result = GetSaveFile();
	if (result)
	{
		OutputTextBlock->Text = result;
	}
	else
	{
		OutputTextBlock->Text = "No file and/or path selected.";
		return;
	}
}