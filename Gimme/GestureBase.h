#pragma once

#include "IGesture.h"

namespace Gimme
{
	namespace UI
	{
		namespace Gestures
		{
			/// <summary>
			/// Represents the base class for all the classes defining gestures to be applied to an ItemsControl.
			/// </summary>
			public ref class GestureBase: public Windows::UI::Xaml::DependencyObject, public IGesture
			{
			public:			
				/// <summary>
				/// Occurs when the gesture has been detected.
				/// </summary>
				virtual event Windows::Foundation::EventHandler<bool>^ Detected;

				/// <summary>
				/// Gets or sets a value indicating whether the gesture can be detected.
				/// </summary>
				virtual property bool IsAllowed;

				/// <summary>
				/// Gets or sets the ItemsControl instance to apply the gestures to.
				/// </summary>
				virtual property Windows::UI::Xaml::Controls::ItemsControl^ Target
				{
					Windows::UI::Xaml::Controls::ItemsControl^ get() 
					{ 
						return _target;
					}
					void set(Windows::UI::Xaml::Controls::ItemsControl^ value)
					{
						if(_target != nullptr)
							OnDetaching();

						_target = value;
						if(_target != nullptr)
							OnAttached();
					}
				}

			protected:
				/// <summary>
				/// Called when the Target has been set/changed.
				/// </summary>
				virtual void OnAttached();

				/// <summary>
				/// Called when the Target is changed.
				/// </summary>
				virtual void OnDetaching();

				/// <summary>
				/// Triggers the Detected event.
				/// </summary>
				/// <param name="bool"></param>
				void RaiseDetectedEvent(bool flag);

				/// <summary>
				/// Occurs when ScrollViewer control has been initialized. 
				/// </summary>
				/// <param name="ScrollViewer"></param>
				virtual void OnScrollViewerInitialized(Windows::UI::Xaml::Controls::ScrollViewer^ scrollViewer)
				{
				}

				/// <summary>
				/// Gets the instance of the ScrollViewer associated with the ItemsControl.
				/// </summary>
				property Windows::UI::Xaml::Controls::ScrollViewer^ TargetScrollViewer
				{
					Windows::UI::Xaml::Controls::ScrollViewer^ get() { return _scrollViewer;}
				}

			private:
				/// <summary>
				/// Occurs when Target is unloaded. 
				/// </summary>
				/// <param name="sender"></param>
				/// <param name="e"></param>
				void OnTargetUnloaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

				/// <summary>
				/// Occurs when the Target layout has been updated. 
				/// </summary>
				/// <param name="sender"></param>
				/// <param name="e"></param>
				void OnTargetLayoutUpdated(Platform::Object^ sender, Platform::Object^ args);

				//Event registration tokens
				Windows::Foundation::EventRegistrationToken _layoutUpdatedToken;
				Windows::Foundation::EventRegistrationToken _unloadedToken;

				/// <summary>
				/// The instance of the ItemsControl to apply the gestures to. 
				/// </summary>
				Windows::UI::Xaml::Controls::ItemsControl^ _target;

				/// <summary>
				/// The instance of the ScrollViewer associated with the ItemsControl. 
				/// </summary>
				Windows::UI::Xaml::Controls::ScrollViewer^ _scrollViewer;
			};
		}
	}
}