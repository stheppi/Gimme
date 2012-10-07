#pragma once
#include "GestureBase.h"

namespace Gimme
{
	namespace UI
	{
		namespace Gestures
		{

			/// <summary>
			/// Specifies the swiping direction.
			/// </summary>
			public enum class SwipeDirection : int
			{
				Left,
				Right
			};

			/// <summary>
			/// Represents the method that will handle removing the item associated with the ItemsControl item being swiped.
			/// </summary>
			public delegate void RemoveItemEventHandler(Platform::Object^ item);

			/// <summary>
			/// Represents the method that will handle the event raised when an ItemsControl item is being swiped.
			/// </summary>
			/// <param name="item">The ItemsControl item Content property</param>
			/// <param name="e">Specifies the swipe direction</param>
			/// <returns>true if the item should be removed.</returns>
			public delegate bool ItemSwipedEventHandler(Platform::Object^ item, SwipeDirection direction);

			/// <summary>
			/// Represents the gesture for handling ItemsControl items swiping.
			/// </summary>
			public ref class SwipeGesture sealed : public GestureBase
			{
			public:
				SwipeGesture(void);

				/// <summary>
				/// Gets or sets the duration for the swipe animation.
				/// </summary>
				property int AnimationDuration;

				/// <summary>
				/// Gets or sets the handle for removing an item from the ItemsControl data source.
				/// </summary>
				event RemoveItemEventHandler^ RemoveItem;

				/// <summary>
				/// Gets or sets the handle for swipe event.
				/// </summary>
				event ItemSwipedEventHandler^ ItemSwiped;

			protected:

				virtual void OnAttached() override;
				virtual void OnDetaching() override;

			private:

				/// <summary>
				/// Occurs when the pointer device initiates a Press action within Target. 
				/// </summary>
				/// <param name="sender"></param>
				/// <param name="e"></param>
				void OnPointerPressed(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);

				/// <summary>
				/// Occurs when a pointer moves while the pointer remains within the Target hit test area.
				/// </summary>
				/// <param name="sender"></param>
				/// <param name="e"></param>
				void OnPointerMoved(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);

				/// <summary>
				/// Occurs when the pointer device that previously initiated a Press action is released.
				/// </summary>
				/// <param name="sender"></param>
				/// <param name="e"></param>
				void OnPointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);

				/// <summary>
				/// Occurs when the pointer device that previously initiated a Press action is released.
				/// </summary>
				/// <param name="sender"></param>
				/// <param name="e"></param>
				void OnSliding(Windows::UI::Input::GestureRecognizer^ sender, Windows::UI::Input::CrossSlidingEventArgs^ e);

				/// <summary>
				/// Occurs when the completes.
				/// </summary>
				void OnSlidingAnimationCompleted();

				///Token handler registration instances.
				Windows::Foundation::EventRegistrationToken _pointerPressedToken;
				Windows::Foundation::EventRegistrationToken _pointerReleasedToken;
				Windows::Foundation::EventRegistrationToken _pointerMovedToken;

				//track the gesture start and end point.
				Windows::Foundation::Point _startPoint;
				Windows::Foundation::Point _endPoint;

				//The gesture recognizer instance allowing to identify the swip gesture
				Windows::UI::Input::GestureRecognizer^ _gestureRecognizer;

				//The item being swiped; for now this is supporting ListBox, ListView but not a simple ItemsControl
				Windows::UI::Xaml::Controls::Primitives::SelectorItem^ _currentElement;
			};
		}
	}
}