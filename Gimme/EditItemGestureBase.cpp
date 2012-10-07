#include "pch.h"
#include <collection.h>

#include "AnimationExtension.h"
#include "EditItemGestureBase.h"
#include "VisualTreeHelperExtension.h"

using namespace Gimme::UI::Extensions;
using namespace Windows::UI::Xaml::Controls;

namespace Gimme
{
	namespace UI
	{
		namespace Gestures
		{
			EditItemGestureBase::EditItemGestureBase(void) :_opacity(0.4)
			{
			}

			void EditItemGestureBase::InitializeEditControls()
			{
				//initialize the controls
				auto container = Target->ItemContainerGenerator->ContainerFromItem(_payload);
				_txtEdit = VisualTreeHelperExtension::FindDescendantBy<TextBox^>(container, "txtEdit");
				_txtBlock = VisualTreeHelperExtension::FindDescendantBy<TextBlock^>(container, "txtBlock");

				if(_txtEdit == nullptr || _txtBlock == nullptr)
					return;

				auto text = _txtBlock->Text;
				_txtEdit->Text = text;
				DisplayEditControls(true);

				_editLostFocusToken = _txtEdit->LostFocus += ref new RoutedEventHandler([this] (Object^ sender, RoutedEventArgs^ e) { OnGestureCompleted();});
				_txtEdit->Focus(FocusState::Keyboard | FocusState::Pointer);
				_txtEdit->Select(text->Length(), 0);
			}

			void EditItemGestureBase::OnGestureCompleted()
			{
				if (_txtEdit != nullptr)
					_txtEdit->LostFocus -= _editLostFocusToken;

				DisplayEditControls(false);
				_txtEdit = nullptr;
				_txtBlock = nullptr;
				_payload = nullptr;
				RaiseDetectedEvent(false);
			}

			void EditItemGestureBase::DisplayEditControls(bool flag)
			{ 
				if(_txtEdit != nullptr)
					_txtEdit->Visibility = flag ? Visibility::Visible : Visibility::Collapsed;

				if(_txtBlock != nullptr)
					_txtBlock->Visibility = flag ? Visibility::Collapsed : Visibility::Visible;
				AnimateItemsOpacity(flag);
			}

			void EditItemGestureBase::AnimateItemsOpacity(bool flag)
			{ 
				double opacity = flag ? _opacity : 1.0;

				std::for_each(begin(Target->Items), end(Target->Items), [this, opacity](Object^ item) 
				{
					if(item == _payload)
						return;
					UIElement^ container = dynamic_cast<UIElement^>(Target->ItemContainerGenerator->ContainerFromItem(item));
					if(container != nullptr)
					{
						container->Opacity = opacity;
					}
				});
			}
		}
	}
}