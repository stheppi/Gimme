#pragma once

#include "EditItemGestureBase.h"

namespace Gimme
{
	namespace UI
	{
		namespace Gestures
		{
			/// <summary>
			/// Represents the gesture for handling item edit.
			/// </summary>
			public ref class TapEditGesture sealed: public EditItemGestureBase
			{
			protected:
				virtual void OnAttached() override;
				virtual void OnDetaching() override;

			private:
				/// <summary>
				/// Occurs when the ItemsControl item is double tapped. This starts the edit gesture. 
				/// </summary>
				/// <param name="sender"></param>
				/// <param name="e"></param>
				void OnDoubleTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::DoubleTappedRoutedEventArgs^ e);

				//Instance of the 
				Windows::UI::Xaml::Input::DoubleTappedEventHandler^ _doubleTappedHandler;
			};
		}
	}
}