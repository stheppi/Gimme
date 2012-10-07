#pragma once

#include "ControlDraggedAdorner.h"
#include "GestureBase.h"

namespace Gimme
{
	namespace UI
	{
		namespace Gestures
		{
			/// <summary>
			/// Represents the method that will handle the event asking to reposition one element in the source collection.
			/// </summary>
			/// <param name="fromIndex">The index of the item to be repositioned in the source collection</param>
			/// <param name="toIndex">The index to move the item to</param>
			public delegate bool MoveItemEventHandler(int fromIndex, int toIndex);

		
			/// <summary>
			/// Represents the base class for handling the item drag gesture.
			/// </summary>
			public ref class DragArrangeGesture sealed : public GestureBase
			{
			public:
				DragArrangeGesture();
				virtual ~DragArrangeGesture();

				/// <summary>
				/// Occurs when the gesture has completed and the item has been repositioned.
				/// </summary>
				/// </summary>
				event MoveItemEventHandler^ MoveItem;

				/// <summary>
				/// Gets or sets the adorner used to display the current dragged item.
				/// </summary>
				property Gimme::UI::Controls::ControlDraggedAdorner^ Adorner
				{
					Gimme::UI::Controls::ControlDraggedAdorner^ get();
					void set(Gimme::UI::Controls::ControlDraggedAdorner^ value);
				}

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
				/// Occurs when an otherwise unhandled Hold interaction occurs over the hit test area of this element.
				/// </summary>
				void OnHolding(Platform::Object^ sender, Windows::UI::Xaml::Input::HoldingRoutedEventArgs^ e);

				/// <summary>
				/// Occurs when the drag gesture starts.
				/// </summary>
				void HandleGestureStart(Windows::UI::Xaml::Input::HoldingRoutedEventArgs^ e);

				/// <summary>
				/// Occurs when the drag gesture ends.
				/// </summary>
				void HandleGestureCompleted(Windows::UI::Xaml::Input::Pointer^ pointer);

				/// <summary>
				/// Scrolls the ItemsControll upwards/downwards while the gesture is active and the current drag position is within the auto-scroll detection area (top or bottom of the ItemsControl).
				/// </summary>
				void PerformAutoScroll();

				/// <summary>
				/// Occurs when the adorner Opacity animation has completed.
				/// </summary>
				void TranslateItems();

				/// <summary>
				/// Returns the item index based on the current drag location. This only works with StackPanel as ItemsTemplate
				/// </summary>
				int GetIndex();

				/// <summary>
				/// Occurs when the adorner Opacity animation has completed.
				/// </summary>
				void AdornerOpacityAnimationCompleted();

				/// <summary>
				/// Occurs when the adorner is slided into the appropriate SelectorItem slot.
				/// </summary>
				void AdornerTranslationAnimationCompleted();

				/// <summary>
				/// Specifies the Pointer release event has not been raised.
				/// </summary>
				bool _isHolding;

				/// <summary>
				/// Contains the index of the item being dragged.
				/// </summary>
				int _startIndex;

				/// <summary>
				/// Contains the item index whose location matches the current drag location.
				/// </summary>
				int _currentDragIndex;

				/// <summary>
				/// Specifies the auto scroll detection area height.
				/// </summary>
				const int _AUTO_SCROLL_DETECTION_HEIGHT;

				Windows::UI::Xaml::Input::PointerEventHandler^ _pointerPressedHandler;
				Windows::UI::Xaml::Input::PointerEventHandler^ _pointerReleasedHandler;
				Windows::UI::Xaml::Input::PointerEventHandler^ _pointerMovedHandler;
				Windows::Foundation::EventRegistrationToken _adornerPointerReleasedToken;
				Windows::UI::Xaml::Input::HoldingEventHandler^ _holdingHandler;

				/// <summary>
				/// The instance of the item being dragged.
				/// </summary>
				Windows::UI::Xaml::Controls::Primitives::SelectorItem^ _item;

				/// <summary>
				/// The timer used for auto scrolling the ItemsControl.
				/// </summary>
				Windows::UI::Xaml::DispatcherTimer^ _autoScrollTimer;

				/// <summary>
				/// Instance of the adorner displaying the current item. 
				/// </summary>
				Gimme::UI::Controls::ControlDraggedAdorner^ _adorner;

				/// <summary>
				/// Contains the pointer device .
				/// </summary>
				Windows::UI::Xaml::Input::Pointer^ _pointer;
			};
		}
	}
}