#include "pch.h"
#include <algorithm>
#include "PullNewItemGesture.h"

using namespace Platform;
using namespace Windows::Devices::Input;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;


namespace Gimme
{
	namespace UI
	{
		namespace Gestures
		{
			PullNewItemGesture::PullNewItemGesture(void)
			{
				DetectionAreaHeight = 5;
			}

			void PullNewItemGesture::OnAttached()
			{
				EditItemGestureBase::OnAttached();

				Target->AddHandler(UIElement::PointerReleasedEvent, _pointerReleasedHandler = ref new PointerEventHandler(this, &PullNewItemGesture::OnPointerReleased),true);
				Target->AddHandler(UIElement::PointerMovedEvent, _pointerMovedHandler = ref new PointerEventHandler(this, &PullNewItemGesture::OnPointerMoved), true);
				Target->AddHandler(UIElement::PointerPressedEvent, _pointerPressedHandler = ref new PointerEventHandler(this, &PullNewItemGesture::OnPointerPressed), true);
			}

			void PullNewItemGesture::OnDetaching()
			{
				EditItemGestureBase::OnDetaching();
				Target->RemoveHandler(UIElement::PointerReleasedEvent, _pointerReleasedHandler);
				Target->RemoveHandler(UIElement::PointerMovedEvent, _pointerMovedHandler);
				Target->RemoveHandler(UIElement::PointerPressedEvent, _pointerPressedHandler);
			}

			void PullNewItemGesture::OnPointerReleased(Object^ sender, PointerRoutedEventArgs^ args)
			{
				if (!IsAllowed)
					_isPointerDown = false;

				if (!_isPointerDown)
					return;

				auto currentPosition = args->GetCurrentPoint(Target)->Position;
				_isPointerDown = false;
				//raise the notification event
				PullNewItemEventArgs^ e = ref new PullNewItemEventArgs(_startPoint, currentPosition, PullNewItemAction::Released);
				Notify(this, e);
				//was an item created
				if(e->Item != nullptr)
				{
					SetPayload(e->Item);
					_layoutUpdatedToken =  Target->LayoutUpdated += ref new EventHandler<Object^>(this, &PullNewItemGesture::OnLayoutUpdated);
				}
				else
				{
					RaiseDetectedEvent(false);
				}
			}

			void PullNewItemGesture::OnPointerPressed(Object^ sender, PointerRoutedEventArgs^ args)
			{
				_startPoint = args->GetCurrentPoint(Target)->Position;
				//check we are within the detection area
				if(_startPoint.Y <= DetectionAreaHeight)
				{
					_isPointerDown = true;
					//notify the gesture has been detected
					RaiseDetectedEvent(true);
					PullNewItemEventArgs^ e = ref new PullNewItemEventArgs(_startPoint, _startPoint, PullNewItemAction::Started);
					Notify(this, e);
				}
			}

			void PullNewItemGesture::OnPointerMoved(Object^ sender, PointerRoutedEventArgs^ args)
			{
				if (!IsAllowed || !_isPointerDown)
					return;

				auto point = args->GetCurrentPoint(Target)->Position;
				PullNewItemEventArgs^ e = ref new PullNewItemEventArgs(_startPoint, point, PullNewItemAction::Move);
				Notify(this, e);
			}

			void PullNewItemGesture::OnGestureCompleted()
			{
				if(_layoutUpdatedToken.Value > 0)
				{
					Target->LayoutUpdated -= _layoutUpdatedToken;
					_layoutUpdatedToken.Value = 0;
				}

				EditItemGestureBase::OnGestureCompleted();
			}

			void PullNewItemGesture::OnLayoutUpdated(Object^ sender, Object^ args)
			{
				//we could get the gesture complete before the layout update
				if(_layoutUpdatedToken.Value == 0)
					return;
				Target->LayoutUpdated -= _layoutUpdatedToken;
				InitializeEditControls();
				AnimateItemsOpacity(true);
			}
		}
	}
}