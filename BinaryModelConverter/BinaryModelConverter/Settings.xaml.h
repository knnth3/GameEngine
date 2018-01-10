//
// Settings.xaml.h
// Declaration of the Settings class
//

#pragma once

#include "Settings.g.h"
#include "MainPage.xaml.h"

namespace BinaryModelConverter
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class Settings sealed
	{
	public:
		Settings();

	private:
		MainPage^ rootPage;

		void LoadDefaults();
		void Overwrite_Toggled(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
