#pragma once

namespace Todo
{
	public ref class BooleanToVisibilityConverter sealed: public Windows::UI::Xaml::Data::IValueConverter
	{
	public:
		virtual Platform::Object^ Convert(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language)
		{
			auto boxedBool = dynamic_cast<Platform::Box<bool>^>(value);
			return boxedBool != nullptr && boxedBool->Value ?  Windows::UI::Xaml::Visibility::Visible : Windows::UI::Xaml::Visibility::Collapsed;
		}

		virtual Platform::Object^ ConvertBack(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language)
		{
			auto visibility = dynamic_cast<Platform::Box<Windows::UI::Xaml::Visibility>^>(value) ;
			return visibility != nullptr && visibility->Value == Windows::UI::Xaml::Visibility::Visible;
		}
	};
}