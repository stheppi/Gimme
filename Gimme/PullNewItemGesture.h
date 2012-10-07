#pragma once

#include "EditItemGestureBase.h"

namespace Gimme
{
	namespace UI
	{
		namespace Gestures
		{

			/// <summary>
			/// Specifies the pull gesture action.
			/// </summary>
			public enum class PullNewItemAction : int
			{
				Started,
				Move,
				Released
			};

			/// <summary>
			/// Provides event data for pull new item event.
			/// </summary>
			public ref class PullNewItemEventArgs sealed
			{
			internal:
				PullNewItemEventArgs(Windows::Foundation::Point start, Windows::Foundation::Point current, PullNewItemAction action) 
					: _startPosition(start), _currentPosition(current), _action(action), _item(nullptr)
				{
				}

			public:

				property Windows::Foundation::Point StartPosition
				{
					Windows::Foundation::Point get()
					{
						return _startPosition;
					}
					void set(Windows::Foundation::Point value)
					{
						_startPosition = value;
					}
				}

				property Windows::Foundation::Point CurrentPosition
				{
					Windows::Foundation::Point get()
					{
						return _currentPosition;
					}
					void set(Windows::Foundation::Point value)
					{
						_currentPosition =value;
					}
				}

				/// <summary>
				/// Gets the pull gesture action.
				/// </summary>
				property PullNewItemAction Action
				{
					PullNewItemAction get()
					{
						return _action;
					}
				}

				/// <summary>
				/// Gets the distance between the gesture starting point and the gesture current point.
				/// </summary>
				property double Distance
				{
					double get()
					{
						return static_cast<double>(_currentPosition.Y - _startPosition.Y);
					}
				}

				/// <summary>
				/// Gets or sets the data item created as a result of the pull gesture release action. Setting the value while the action is not 'released'
				/// will throw a ChangedStateException.
				/// </summary>
				property Platform::Object^ Item
				{
					Platform::Object^ get()
					{
						return _item;
					}
					void set(Platform::Object^ item)
					{
						if(_action != PullNewItemAction::Released)
							throw ref new Platform::ChangedStateException();
						_item = item;
					}
				}

			private:
				PullNewItemAction _action;
				Platform::Object^ _item;
				Windows::Foundation::Point _startPosition;
				Windows::Foundation::Point _currentPosition;
			};

			/// <summary>
			/// Represents the class handling the pull new item gesture.
			/// </summary>
			public ref class PullNewItemGesture sealed : public EditItemGestureBase
			{
			public:
				PullNewItemGesture(void);

				/// <summary>
				/// Gets or sets the area height at the top of the Target control where the gesture can occur.
				/// </summary>
				property double DetectionAreaHeight;

				/// <summary>
				/// Occurs when the gesture is active.
				/// </summary>
				event Windows::Foundation::EventHandler<PullNewItemEventArgs^>^ Notify;
			protected:
				virtual void OnAttached() override;
				virtual void OnDetaching() override;
				virtual void OnGestureCompleted() override;
			private:
								
				/// <summary>
				/// Occurs when the pointer device initiates a Press action within Target. 
				/// </summary>
				/// <param name="sender"></param>
				/// <param name="e"></param>
				void OnPointerPressed ( Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);

				/// <summary>
				/// Occurs when the pointer device that previously initiated a Press action is released.
				/// </summary>
				/// <param name="sender"></param>
				/// <param name="e"></param>
				void OnPointerReleased ( Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);

				/// <summary>
				/// Occurs when a pointer moves while the pointer remains within the Target hit test area.
				/// </summary>
				/// <param name="sender"></param>
				/// <param name="e"></param>
				void OnPointerMoved ( Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);

				/// <summary>
				/// Occurs when Target layout update event is raised. Once an item is added as a result of the pull gesture we need to wait for the layout
				/// to get updated before we can enable the edit.
				/// </summary>
				/// <param name="sender"></param>
				/// <param name="e"></param>
				void OnLayoutUpdated(Platform::Object^ sender, Platform::Object^ args);

				//specifies if the pointer device is still pressed
				bool _isPointerDown;
				Windows::Foundation::Point _startPoint;
								
				Windows::Foundation::EventRegistrationToken _layoutUpdatedToken;

				Windows::UI::Xaml::Input::PointerEventHandler^ _pointerPressedHandler;
				Windows::UI::Xaml::Input::PointerEventHandler^ _pointerMovedHandler;
				Windows::UI::Xaml::Input::PointerEventHandler^ _pointerReleasedHandler;
				Windows::UI::Xaml::Input::Pointer^ _pointer;
			};
		}
	}
}
