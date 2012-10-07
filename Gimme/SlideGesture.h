#pragma once

#include "GestureBase.h"
#include "BasicTimer.h"

namespace Gimme
{
	namespace UI
	{
		namespace Gestures
		{
			/// <summary>
			/// Represents the gesture for handling vertical sliding. This gesture should be used with DragArrangeGesture because ItemsTemplate( StackPanel)  can't stopped to scroll while dragging the item up and down;
			/// </summary>
			public ref class SlideGesture sealed : public GestureBase
			{
			public:
				SlideGesture(void);
				virtual ~SlideGesture(void);

				/// <summary>
				/// Gets or sets the value to add or substract from the ScrollViewer VerticalOffset during the user interaction.
				/// </summary>
				property double SlideStep;

			protected:
				virtual void OnAttached() override;
				virtual void OnDetaching() override;

			private:
				/// <summary>
				/// Scrolls the Target control vertically. Based on user sliding gesture(inertia) an timer is started in order to simulate the vertical scrolling manipulation
				/// </summary>
				/// <param name="sender"></param>
				/// <param name="e"></param>
				void OnAnimation(Platform::Object^ sender, Platform::Object^ e);

				/// <summary>
				/// Occurs when the pointer device initiates a Press action within the Target control. 
				/// </summary>
				/// <param name="sender"></param>
				/// <param name="e"></param>
				void OnPointerPressed(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);

				/// <summary>
				/// Occurs when a pointer moves while the pointer remains within the hit test area of the Target. 
				/// </summary>
				/// <param name="sender"></param>
				/// <param name="e"></param>
				void OnPointerMoved(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);

				/// <summary>
				/// Occurs when the pointer device that previously initiated a Press action is released, while within the Target.
				/// </summary>
				/// <param name="sender"></param>
				/// <param name="e"></param>
				void OnPointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);

				/// <summary>
				/// Occurs when a hold interaction occurs over the hit test area of the Target.
				/// </summary>
				/// <param name="sender"></param>
				/// <param name="e"></param>
				void OnHolding(Platform::Object^ sender, Windows::UI::Xaml::Input::HoldingRoutedEventArgs^ e);

				/// <summary>
				/// Specifies if the pointer device has initiated a Press action but the Release one hasn't fired yet 
				/// </summary>
				bool _isMouseDown;

				/// <summary>
				/// Specifies if the IGesture::Detected event has been raised; The gesture is determined in the pointer moved event handler and it should be raised once.
				/// </summary>
				bool _detectionRaised;

				/// <summary>
				/// Stores the high resolution timer elapsed period. This
				/// </summary>
				double _moveDuration;

				/// <summary>
				/// Stores the starting vertical offset value.
				/// </summary>
				double _animationStartValue;

				/// <summary>
				/// Stores the final vertical offset value.
				/// </summary>
				double _animationFinalValue;

				/// <summary>
				/// Stores the vertical offset animation duration.
				/// </summary>
				double _animationDuration;

				/// <summary>
				/// Stores the vertical offset animation elapsed duration.
				/// </summary>
				double _animationElapsedDuration;

				//stores the pointer device coordinates 
				Windows::Foundation::Point _previousPoint;
				Windows::Foundation::Point _currentPoint;

				//the event handlers for the Pointer Device
				Windows::UI::Xaml::Input::PointerEventHandler^ _pointerPressedHandler;
				Windows::UI::Xaml::Input::PointerEventHandler^ _pointerReleasedHandler;
				Windows::UI::Xaml::Input::PointerEventHandler^ _pointerMovedHandler;
				Windows::UI::Xaml::Input::HoldingEventHandler^ _holdingEventHandler;

				/// <summary>
				/// Instance of the high resolution timer
				/// </summary>
				BasicTimer^ _timer;

				/// <summary>
				/// Used to create the sliding animation based on the user touch inertia
				/// </summary>
				Windows::UI::Xaml::DispatcherTimer^ _dispatcherAnimation;
			};
		}
	}
}