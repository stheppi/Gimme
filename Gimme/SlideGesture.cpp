#include "pch.h"
#include <math.h>
#include "SlideGesture.h"

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Input;

namespace Gimme
{
	namespace UI
	{
		namespace Gestures
		{
			static double QuadEaseOut(double t, double b, double c, double d)
			{
				return -c * (t /= d) * (t - 2) + b;
			}

			/// <summary>
			/// 
			/// </summary>
			/// <param name="t">Current elapsed duration.</param>
			/// <param name="b">Start value.</param>
			/// <param name="c">Final value.</param>
			/// <param name="d">Total duration.</param>
			/// <returns></returns>
			static double CubicEaseOut(double t, double b, double c, double d)
			{
				return c * ((t = t / d - 1) * t * t + 1) + b;
			}


			SlideGesture::SlideGesture(void) 
				: _isMouseDown(false),_detectionRaised(false), _animationDuration(30), _animationElapsedDuration(0), _animationStartValue(0), _animationFinalValue(0),
				_timer(ref new BasicTimer()), _dispatcherAnimation(ref new DispatcherTimer())
			{
				SlideStep = 8;

				TimeSpan interval;
				interval.Duration = 5 * 10000; //5ms
				_dispatcherAnimation->Interval = interval;
				_dispatcherAnimation->Tick += ref new EventHandler<Object^>(this, &SlideGesture::OnAnimation);
			}


			SlideGesture::~SlideGesture(void)
			{
			}

			void SlideGesture::OnAttached()
			{
				GestureBase::OnAttached();

				_pointerReleasedHandler = ref new PointerEventHandler(this, &SlideGesture::OnPointerPressed);
				Target->AddHandler(UIElement::PointerPressedEvent, _pointerReleasedHandler, true);

				_pointerMovedHandler = ref new PointerEventHandler(this, &SlideGesture::OnPointerMoved);
				Target->AddHandler(UIElement::PointerMovedEvent, _pointerMovedHandler, true);

				_pointerReleasedHandler = ref new PointerEventHandler(this, &SlideGesture::OnPointerReleased);
				Target->AddHandler(UIElement::PointerReleasedEvent, _pointerReleasedHandler, true);

				_holdingEventHandler = ref new HoldingEventHandler(this, &SlideGesture::OnHolding);
				Target->AddHandler(ItemsControl::HoldingEvent, _holdingEventHandler, true);
			}


			void SlideGesture::OnDetaching()
			{
				GestureBase::OnDetaching();

				Target->RemoveHandler(UIElement::PointerPressedEvent, _pointerReleasedHandler);
				Target->RemoveHandler(UIElement::PointerPressedEvent, _pointerMovedHandler);
				Target->RemoveHandler(UIElement::PointerPressedEvent, _pointerPressedHandler);
				Target->RemoveHandler(FrameworkElement::HoldingEvent, _holdingEventHandler);
			}

			void SlideGesture::OnPointerPressed(Object^ sender, PointerRoutedEventArgs^ e)
			{
				_isMouseDown = true;
				_moveDuration = 0;
				_previousPoint = _currentPoint = e->GetCurrentPoint(Target)->Position;
				//get the current time
				_timer->Start();
			}

			void SlideGesture::OnPointerMoved(Object^ sender, PointerRoutedEventArgs^ e)
			{
				if (!IsAllowed || !_isMouseDown )
					return;

				//determine the X and Y axis delta
				auto current = e->GetCurrentPoint(Target)->Position;
				double deltaY = (current.Y - _currentPoint.Y);
				double deltaX = (current.X - _currentPoint.X);

				//we only take look
				if (abs(deltaX) < abs(deltaY) && abs(deltaY) > 3)
				{
					//have we raised the Detected event already?!
					if (!_detectionRaised)
					{
						RaiseDetectedEvent(true);
						_detectionRaised = true;
					}

					//scroll based ond the Y axis delta
					double offset = TargetScrollViewer->VerticalOffset - deltaY;// (deltaY >= 0 ? -SlideStep : SlideStep);
					TargetScrollViewer->ScrollToVerticalOffset(offset);
					_previousPoint = _currentPoint;
					_currentPoint = current;
					_timer->Stop();
					//retain the timer elapsed value
					_moveDuration = _timer->Duration;
					//reset timer
					_timer->Start();
				}
			}

			void SlideGesture::OnPointerReleased(Object^ sender, PointerRoutedEventArgs^ e)
			{
				_isMouseDown = false;
				_detectionRaised = false;
				if (!IsAllowed)
					return;

				if (_moveDuration != 0)
				{
					double deltaY = _currentPoint.Y - _previousPoint.Y;
					double deltaX = _currentPoint.X - _previousPoint.X;

					//stop the gesture if we have a movement on X axis bigger than Y 
					if (abs(deltaX) > abs(deltaY))
					{
						RaiseDetectedEvent(false);
						return;
					}

					//work out the gesture velocity
					double velocity = floor(min(2000, abs(deltaY) / _moveDuration));
					if (velocity > 350)
					{
						double steps = velocity / 15;
						double offset = SlideStep * steps;

						//set the value as if the timer triggered to avoid stopping the scrolling
						_animationElapsedDuration  = _dispatcherAnimation->Interval.Duration / 10000;

						_animationStartValue = TargetScrollViewer->VerticalOffset;
						_animationFinalValue = (deltaY >= 0 ? -offset : offset);
						_dispatcherAnimation->Start();

						//calculate the current vertical offset value
						double value = CubicEaseOut(_animationElapsedDuration, _animationStartValue, _animationFinalValue, _animationDuration);
						TargetScrollViewer->ScrollToVerticalOffset(value);
					}
					else
					{
						RaiseDetectedEvent(false);
					}
				}
				else
				{
					RaiseDetectedEvent(false);
				}
			}

			void SlideGesture::OnHolding(Object^ sender, HoldingRoutedEventArgs^ e)
			{
				//fake pointer release to stop this gesture
				_isMouseDown= false;
			}

			void SlideGesture::OnAnimation(Object^ sender, Object^ e)
			{
				_animationElapsedDuration += (_dispatcherAnimation->Interval.Duration / 10000);

				//calculate the current vertical offset value and scroll to it
				auto value = CubicEaseOut(_animationElapsedDuration, _animationStartValue, _animationFinalValue, _animationDuration);
				TargetScrollViewer->ScrollToVerticalOffset(value);

				//not ideal since we can't guarantee the interval between Ticks is the same as set. 
				//TODO: move to a high resolution timer
				if (_animationElapsedDuration >= _animationDuration)
					_dispatcherAnimation->Stop();
			}
		}
	}
}