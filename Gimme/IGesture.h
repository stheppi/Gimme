#pragma once

namespace Gimme
{
	namespace UI
	{
		namespace Gestures
		{
			/// <summary>
			/// Represents a user gesture.
			/// </summary>
			public interface class IGesture
			{
				/// <summary>
				/// Occurs when the gesture has been detected
				/// </summary>
				event Windows::Foundation::EventHandler<bool>^ Detected;

				/// <summary>
				/// Gets or sets the ItemsControl instancet to apply the gesture to.
				/// </summary>
				property Windows::UI::Xaml::Controls::ItemsControl^ Target; 

				/// <summary>
				/// Gets or sets the flag to enable/disable the gesture
				/// </summary>
				property bool IsAllowed;
			};
		}
	}

}