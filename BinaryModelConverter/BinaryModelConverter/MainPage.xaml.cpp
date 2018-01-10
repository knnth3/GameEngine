//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#include "pch.h"
#include "MainPage.xaml.h"
#include "ConvertPage.g.h"
#include "HomePage.g.h"
#include "Settings.g.h"

using namespace BinaryModelConverter;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::ApplicationModel::Core;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage^ MainPage::Current = nullptr;

MainPage::MainPage()
{
	InitializeComponent();
	AppTitle->Text = Windows::ApplicationModel::Package::Current->DisplayName;
	ContentFrame->Navigate(TypeName(HomePage::typeid));
	CoreApplicationViewTitleBar^ titleBar = CoreApplication::GetCurrentView()->TitleBar;
	titleBar->LayoutMetricsChanged += ref new TypedEventHandler<CoreApplicationViewTitleBar^, Object^>(this, &MainPage::TitleBar_LayoutMetricsChanged);
	Current = this;
	m_bOverwriteSave = false;
}

void BinaryModelConverter::MainPage::SetOverwriteProperty(bool value)
{
	m_bOverwriteSave = value;
}

bool BinaryModelConverter::MainPage::GetOverwriteProperty()
{
	return m_bOverwriteSave;
}

void BinaryModelConverter::MainPage::NavView_SelectionChanged(NavigationView^ sender, NavigationViewSelectionChangedEventArgs^ args)
{
	if (args->IsSettingsSelected)
	{
		ContentFrame->Navigate(TypeName(Settings::typeid));
		NavView->Header = "Settings";
	}
	else
	{
		NavigationViewItem^ item = static_cast<NavigationViewItem^>(args->SelectedItem);
		auto requested = item->Tag->ToString();

		if (requested == "Home")
		{
			ContentFrame->Navigate(TypeName(HomePage::typeid));
			NavView->Header = "Home";
		}
		if (requested == "Convert")
		{
			ContentFrame->Navigate(TypeName(ConvertPage::typeid));
			NavView->Header = "Convert a Model";
		}
	}
}

void BinaryModelConverter::MainPage::TitleBar_LayoutMetricsChanged(CoreApplicationViewTitleBar^ sender, Platform::Object^ args)
{
	Thickness result = Thickness(CoreApplication::GetCurrentView()->TitleBar->SystemOverlayLeftInset + 60, 8, 0, 0);
	AppTitle->Margin = result;
}