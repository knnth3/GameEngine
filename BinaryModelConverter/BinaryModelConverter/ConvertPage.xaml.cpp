//
// ConvertPage.xaml.cpp
// Implementation of the ConvertPage class
//

#include "pch.h"
#include "ConvertPage.xaml.h"
//#include "ASSIMP_READER.h"
#include "FBX SDK\FBX_READER.h"
#include <ppltasks.h>


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

#define EXPORT_FILE_EXTENSION ".bin"

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

ConvertPage::ConvertPage()
{
	InitializeComponent();
	LoadTempFile();
	rootPage = MainPage::Current;
	m_selectedFolder = nullptr;
	PickAFileButton->Click += ref new RoutedEventHandler(this, &ConvertPage::PickAFileButton_Click);
	PickAFolderButton->Click += ref new RoutedEventHandler(this, &ConvertPage::PickAFolderButton_Click);
	RunConversionButton->Click += ref new RoutedEventHandler(this, &ConvertPage::RunConversionButton_Click);
}

Windows::Storage::StorageFile^ BinaryModelConverter::ConvertPage::GetOriginalFile()
{
	return m_file;
}

Platform::String^ BinaryModelConverter::ConvertPage::GetSaveFile()
{
	String^ result = "";
	auto folder = GetStorageFolder();
	if (!folder)
		return result;

	if (!m_file)
		return result;

	result = folder->Path + "\\" + m_file->DisplayName + EXPORT_FILE_EXTENSION;
	return result;
}

Windows::Storage::StorageFolder ^ BinaryModelConverter::ConvertPage::GetStorageFolder()
{
	return m_selectedFolder;
}

bool BinaryModelConverter::ConvertPage::CopyContentToTemp()
{
	if (!m_file || !m_temp)
		return false;

	create_task(m_file->CopyAndReplaceAsync(m_temp));
	return true;
}

void BinaryModelConverter::ConvertPage::LoadTempFile()
{
	StorageFolder^ appFolder = Windows::ApplicationModel::Package::Current->InstalledLocation;

	// Get the app's manifest file from the current folder
	String^ name = "models\\model.tmp";
	create_task(appFolder->GetFileAsync(name)).then([=](StorageFile^ file)
	{
		m_temp = file;
		//Do something with the manifest file  
	});
}

std::wstring BinaryModelConverter::ConvertPage::ParseFilePath(std::wstring path)
{
	std::wstring retpath;
	bool sameDir = true;
	for (auto x : path)
	{
		if (sameDir && x == '\\')
		{
			sameDir = false;
			retpath.push_back('/');
		}
		else
		{
			sameDir = true;
			retpath.push_back(x);
		}
	}
	return retpath;
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
	openPicker->FileTypeFilter->Append(".fbx");
	openPicker->FileTypeFilter->Append(".dae");

	create_task(openPicker->PickSingleFileAsync()).then([this](StorageFile^ file)
	{
		if (file)
		{
			OutputTextBlock->Text = "Picked file -> " + file->Name;
			PickAFileButtonResult->Text = file->Name;
			m_file = file;
		}
	});
}

void BinaryModelConverter::ConvertPage::PickAFolderButton_Click(Platform::Object ^ sender, Windows::UI::Xaml::RoutedEventArgs ^ e)
{

	FolderPicker^ folderPicker = ref new FolderPicker();
	folderPicker->ViewMode = PickerViewMode::List;
	folderPicker->SuggestedStartLocation = PickerLocationId::DocumentsLibrary;
	folderPicker->FileTypeFilter->Append("*");

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
		if (CopyContentToTemp())
		{
			const FBX_READER io;
			MeshData mesh;
			String^ error;
			if (!io.LoadModel(m_temp, mesh, m_file->FileType, error))
				OutputTextBlock->Text = error;

			auto folder = GetStorageFolder();
			CreationCollisionOption newFileOption;
			if (rootPage->GetOverwriteProperty())
			{
				newFileOption = CreationCollisionOption::GenerateUniqueName;
			}
			else
			{
				newFileOption = CreationCollisionOption::ReplaceExisting;
			}

			create_task(folder->CreateFileAsync(m_file->DisplayName + EXPORT_FILE_EXTENSION, newFileOption)).then([=](StorageFile^ file)
			{
				io.SaveNewMesh(file, mesh);
				OutputTextBlock->Text = file->Name;
			});
		}
	}
	else
	{
		OutputTextBlock->Text = "No file and/or path selected.";
		return;
	}
}