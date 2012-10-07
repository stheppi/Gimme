#include "pch.h"
#include "TapEditGesture.h"
#include "VisualTreeHelperExtension.h"


using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Gimme::UI::Extensions;

namespace Gimme
{
	namespace UI
	{
		namespace Gestures
		{
			void TapEditGesture::OnAttached()
			{
				EditItemGestureBase::OnAttached();
				Target->IsDoubleTapEnabled = true;
				_doubleTappedHandler = ref new DoubleTappedEventHandler(this, &TapEditGesture::OnDoubleTapped);
				Target->AddHandler(FrameworkElement::DoubleTappedEvent,  _doubleTappedHandler, true);
			}

			void TapEditGesture::OnDetaching()
			{
				EditItemGestureBase::OnDetaching();
				Target->RemoveHandler(FrameworkElement::DoubleTappedEvent, _doubleTappedHandler);
			}

			void TapEditGesture::OnDoubleTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::DoubleTappedRoutedEventArgs^ e)
			{
				if (!IsAllowed)
					return;

				auto originalSource = (DependencyObject^)e->OriginalSource ;
				auto item = VisualTreeHelperExtension::GetParent<SelectorItem^>(originalSource);
				if (item != nullptr)
				{
					RaiseDetectedEvent(true);
					SetPayload(item->Content);

					InitializeEditControls();
				}
			}
		}
	}
}