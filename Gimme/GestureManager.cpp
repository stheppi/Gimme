#include "pch.h"
#include "GestureManager.h"

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Controls;

namespace Gimme
{
	namespace UI
	{
		namespace Gestures
		{
			TypeName GestureManager::_gesturesObservableCollectionType = { IObservableVector<IGesture^>::typeid->FullName, TypeKind::Metadata};
			TypeName GestureManager::_ownerType = { GestureManager::typeid->FullName, TypeKind::Custom};
			TypeName GestureManager::_eventTokenType = { EventRegistrationToken::typeid->FullName, TypeKind::Metadata};

			PropertyMetadata^ GestureManager::_gesturesPropertyMetadata = ref new PropertyMetadata(nullptr /*ref new Vector<IGesture^>()*/, ref new PropertyChangedCallback( &GestureManager::OnGesturesChangedCallback));
			DependencyProperty^ GestureManager::_gesturesProperty = DependencyProperty::RegisterAttached("Gestures", _gesturesObservableCollectionType, _ownerType, _gesturesPropertyMetadata);
			DependencyProperty^ GestureManager::_eventTokenProperty = DependencyProperty::RegisterAttached("EventToken", _eventTokenType, _ownerType, nullptr);

			DependencyProperty^ GestureManager::EventTokenProperty::get()
			{
				return _eventTokenProperty;
			}

			EventRegistrationToken GestureManager::GetEventToken(DependencyObject^ obj)
			{
				auto boxed = dynamic_cast<Platform::Box<EventRegistrationToken>^>( obj->GetValue(_eventTokenProperty));
				if(boxed == nullptr)
					return EventRegistrationToken();

				return boxed->Value;
			}

			void GestureManager::SetEventToken(DependencyObject^ obj, EventRegistrationToken value)
			{
				obj->SetValue(_eventTokenProperty, value);
			}

			void GestureManager::OnGesturesChangedCallback(DependencyObject^ sender, Windows::UI::Xaml::DependencyPropertyChangedEventArgs^ args)
			{
				ItemsControl^ itemsControl = dynamic_cast<ItemsControl^>(sender);
				if(itemsControl != nullptr)
				{
					if(args->OldValue != nullptr)
					{
						IObservableVector<IGesture^>^ gestures = (IObservableVector<IGesture^>^)args->OldValue;
						auto token = GetEventToken(itemsControl);
						gestures->VectorChanged -= token;
					}

					if(args->NewValue != nullptr)
					{
						IObservableVector<IGesture^>^ gestures = dynamic_cast<IObservableVector<IGesture^>^>(args->NewValue);
						for (int i = 0; i < gestures->Size; i++)
						{
							IGesture^ gesture = gestures->GetAt(i);
							if(gesture)
							{
								gesture->Target = itemsControl;
								gesture->IsAllowed = true;
								gesture->Detected +=  ref new EventHandler<bool>([gestures](Object^ s, bool flag)
								{
									IGesture^ g = dynamic_cast<IGesture^>(s);
									g->IsAllowed = true;
									bool isAllowed = !flag;
									for(unsigned int i = 0; i < gestures->Size; ++i)
									{
										auto currentGesture = gestures->GetAt(i);
										if(g != currentGesture)
											currentGesture->IsAllowed = isAllowed;
									}
								});
							}
						}
						EventRegistrationToken token =	gestures->VectorChanged += ref new VectorChangedEventHandler<IGesture^>([itemsControl](IObservableVector<IGesture^>^ source, IVectorChangedEventArgs^ args)
						{
							unsigned int index = args->Index;
							IGesture^ gesture = nullptr;
							switch(args->CollectionChange)
							{
							case CollectionChange::ItemInserted:
								gesture =  source->GetAt(args->Index);
								if(gesture)
								{
									gesture->Target = itemsControl;
									gesture->IsAllowed = true;
									gesture->Detected +=  ref new EventHandler<bool>([source](Object^ s, bool flag)
									{
										IGesture^ g = dynamic_cast<IGesture^>(s);
										g->IsAllowed= true;
										bool isAllowed = !flag;
										for(unsigned int i = 0; i < source->Size; ++i)
										{
											auto currentGesture = source->GetAt(i);
											if(g != currentGesture)
												currentGesture->IsAllowed = isAllowed;
										}
									});
								}
								break;

							case CollectionChange::ItemRemoved:
								break;
							}
						});
						SetEventToken(itemsControl, token);
					}
				}
			}


		}
	}
}