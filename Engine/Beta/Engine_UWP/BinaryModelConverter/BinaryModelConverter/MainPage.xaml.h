//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"

namespace BinaryModelConverter
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();

		void SetOverwriteProperty(bool value);
		bool GetOverwriteProperty();

	internal:
		static MainPage^ Current;

	private:
		bool m_bOverwriteSave;

		void NavView_SelectionChanged(Windows::UI::Xaml::Controls::NavigationView^ sender, Windows::UI::Xaml::Controls::NavigationViewSelectionChangedEventArgs^ args);
		void TitleBar_LayoutMetricsChanged(Windows::ApplicationModel::Core::CoreApplicationViewTitleBar ^ sender, Platform::Object ^ args);
	};
}
