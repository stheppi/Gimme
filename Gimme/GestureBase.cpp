#include "pch.h"
#include "GestureBase.h"
#include "VisualTreeHelperExtension.h"

using namespace Platform;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;

using namespace Windows::Foundation;
using namespace Gimme::UI::Extensions;

namespace Gimme
{
	namespace UI
	{
		namespace Gestures
		{
			void GestureBase::OnAttached()
			{
				_unloadedToken = _target->Unloaded += ref new RoutedEventHandler(this, &GestureBase::OnTargetUnloaded);
				_layoutUpdatedToken = _target->LayoutUpdated += ref new EventHandler<Object^>(this, &GestureBase::OnTargetLayoutUpdated);
			}

			void GestureBase::OnDetaching()
			{
				_target->Unloaded -= _unloadedToken;
			}

			void GestureBase::RaiseDetectedEvent(bool flag)
			{
				Detected(this, flag);
			}

			void GestureBase::OnTargetUnloaded(Object^ sender, RoutedEventArgs^ args)
			{
				OnDetaching();
			}

			void GestureBase::OnTargetLayoutUpdated(Object^ sender, Object^ args)
			{
				Target->LayoutUpdated -= _layoutUpdatedToken;

				//search for the scrollviewer
				IIterable<ScrollViewer^>^ descendats = VisualTreeHelperExtension::Descendants<ScrollViewer^>(this->Target);
				auto iter = descendats->First();
				if(iter->HasCurrent)
				{
					_scrollViewer = iter->Current;
					OnScrollViewerInitialized(_scrollViewer);
				}

			}
		}
	}
}