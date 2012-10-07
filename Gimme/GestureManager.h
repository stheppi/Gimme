#pragma once
#include <collection.h>
#include "IGesture.h"

namespace Gimme
{
	namespace UI
	{
		namespace Gestures
		{
			[Windows::UI::Xaml::Data::Bindable]
			[Windows::Foundation::Metadata::WebHostHidden]
			public ref class GestureManager sealed
			{
			public:
		
				static property Windows::UI::Xaml::DependencyProperty^ GesturesProperty
				{
					Windows::UI::Xaml::DependencyProperty^ get()
					{
						return _gesturesProperty;
					}
				}

				static Windows::Foundation::Collections::IObservableVector<IGesture^>^ GestureManager::GetGestures(Windows::UI::Xaml::DependencyObject^ target)
				{
					auto gestures= (Windows::Foundation::Collections::IObservableVector<IGesture^>^) target->GetValue(_gesturesProperty);
					if( gestures == nullptr)
					{
						
						gestures = ref new Platform::Collections::Vector<IGesture^>();
						SetGestures(target, gestures);
					}
					return gestures;
				}

				static void GestureManager::SetGestures(Windows::UI::Xaml::DependencyObject^ obj, Windows::Foundation::Collections::IObservableVector<IGesture^>^ value)
				{
					obj->SetValue(_gesturesProperty, value);
				}

			private:
				GestureManager();

				static property Windows::UI::Xaml::DependencyProperty^ EventTokenProperty
				{
					Windows::UI::Xaml::DependencyProperty^ get();
				}

				static void OnGesturesChangedCallback(Windows::UI::Xaml::DependencyObject^ sender,  Windows::UI::Xaml::DependencyPropertyChangedEventArgs^ args);

				static Windows::Foundation::EventRegistrationToken GetEventToken(Windows::UI::Xaml::DependencyObject^ obj);
				static void SetEventToken(Windows::UI::Xaml::DependencyObject^ obj, Windows::Foundation::EventRegistrationToken value);

				static Windows::UI::Xaml::Interop::TypeName _gesturesObservableCollectionType;
				static Windows::UI::Xaml::Interop::TypeName _ownerType;
				static Windows::UI::Xaml::Interop::TypeName _eventTokenType;

				static Windows::UI::Xaml::PropertyMetadata^ _gesturesPropertyMetadata;
				static Windows::UI::Xaml::DependencyProperty^ _gesturesProperty;
				static Windows::UI::Xaml::DependencyProperty^ _eventTokenProperty;
			};
		}
	}
}
