#include "pch.h"
#include "SwipeGesture.h"
#include "DependencyObjectExtension.h"
#include "FrameworkElementExtension.h"
#include "AnimationExtension.h"

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::UI::Input;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Media::Animation;

using namespace Gimme::UI::Extensions;

namespace Gimme
{
	namespace UI
	{
		namespace Gestures
		{

			SwipeGesture::SwipeGesture(void) : _gestureRecognizer(ref new GestureRecognizer())
			{
				AnimationDuration = 500;

				_gestureRecognizer->CrossSlideHorizontally = true;
				_gestureRecognizer->CrossSlideExact = false;

				_gestureRecognizer->GestureSettings = GestureSettings::CrossSlide;//| GestureSettings.ManipulationTranslateX;
				_gestureRecognizer->CrossSliding += ref new TypedEventHandler<GestureRecognizer^, CrossSlidingEventArgs^>(this, &SwipeGesture::OnSliding);
			}

			void SwipeGesture::OnAttached()
			{
				GestureBase::OnAttached();

				Target->AddHandler(UIElement::PointerPressedEvent,ref new PointerEventHandler(this, &SwipeGesture::OnPointerPressed), true);
				Target->AddHandler(UIElement::PointerMovedEvent,ref new PointerEventHandler(this, &SwipeGesture::OnPointerMoved), true);
				Target->AddHandler(UIElement::PointerReleasedEvent, ref new PointerEventHandler(this, &SwipeGesture::OnPointerReleased), true);
			}

			void SwipeGesture::OnDetaching()
			{
				GestureBase::OnDetaching();

			}

			void SwipeGesture::OnPointerPressed(Object^ sender, PointerRoutedEventArgs^ e)
			{
				if (!IsAllowed)
					return;

				UIElement^ source = dynamic_cast<UIElement^>(sender);
				//get the target ItemsSelector
				DependencyObject^ origSource = dynamic_cast<DependencyObject^>(e->OriginalSource);
				_currentElement = DependencyObjectExtension::GetParent<SelectorItem^>(origSource);
				if (_currentElement == nullptr)
					return;

				//reset the gesture recognizer
				_gestureRecognizer->CompleteGesture();
				auto  point = e->GetCurrentPoint(Target);
				_startPoint = point->Position;
				_gestureRecognizer->ProcessDownEvent(point);
			}

			void SwipeGesture::OnPointerMoved(Object^ sender, PointerRoutedEventArgs^ e)
			{    
				if (_currentElement == nullptr || !IsAllowed)
					return;
				auto points = e->GetIntermediatePoints(Target);
				_endPoint = e->GetCurrentPoint(Target)->Position;
				_gestureRecognizer->ProcessMoveEvents(points);
			}

			void SwipeGesture::OnPointerReleased(Object^ sender, PointerRoutedEventArgs^ e)
			{
				if (_currentElement == nullptr || !IsAllowed)
					return;
				auto point = e->GetCurrentPoint(Target);
				_endPoint = point->Position;
				_gestureRecognizer->ProcessUpEvent(point);
				_gestureRecognizer->CompleteGesture();
			}

			void SwipeGesture::OnSliding(GestureRecognizer^ sender, CrossSlidingEventArgs^ args)
			{  
				switch (args->CrossSlidingState)
				{
				case CrossSlidingState::Started:
					//Initialize the TranslateTransform for the current element
					FrameworkElementExtension::TranslateX(_currentElement, 0);
					RaiseDetectedEvent(true);
					break;

				case CrossSlidingState::Dragging:
					//translate the item horizontally
					FrameworkElementExtension::TranslateX(_currentElement, _endPoint.X - _startPoint.X);
					break;

				case CrossSlidingState::Completed:
					FrameworkElementExtension::TranslateX(_currentElement, _endPoint.X - _startPoint.X);

					//get the data associated with the current element
					auto item = Target->ItemContainerGenerator->ItemFromContainer(_currentElement);
					auto trans = dynamic_cast<TranslateTransform^>( _currentElement->RenderTransform);

					auto ease = ref new SineEase();
					

					//chekc to see if the item should be removed
					if (!ItemSwiped(item, _endPoint.X > _startPoint.X ? SwipeDirection::Right : SwipeDirection::Left))
					{
						ease->EasingMode = EasingMode::EaseOut;
						//animate the current item TranslateTransform::X
						AnimationExtension::Animate(trans, trans->X, 0, "X", AnimationDuration, 0, ease, ref new ActionEventHandler([this](){ RaiseDetectedEvent(false); }));
						_currentElement = nullptr;
					}
					else
					{
						ease->EasingMode = EasingMode::EaseInOut;
						//animate the current item TranslateTransform::X
						AnimationExtension::Animate(trans, trans->X, -(_currentElement->ActualWidth + 50), "X", AnimationDuration, 0, ease, ref new ActionEventHandler(this, &SwipeGesture::OnSlidingAnimationCompleted));
					}

					break;
				}
			}


			void SwipeGesture::OnSlidingAnimationCompleted()
			{
				RemoveItem(Target->ItemContainerGenerator->ItemFromContainer(_currentElement));
				_currentElement = nullptr;
				RaiseDetectedEvent(false);
			}
		}
	}
}