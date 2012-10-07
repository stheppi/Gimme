#pragma once

namespace Todo
{
	public ref class ColorBrushConverter sealed: public Windows::UI::Xaml::Data::IValueConverter
	{
	public:
		virtual Platform::Object^ Convert(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language)
		{
			auto colorBoxed = dynamic_cast<Platform::Box<Windows::UI::Color>^>(value);
			if(colorBoxed == nullptr)
				return Windows::UI::Xaml::DependencyProperty::UnsetValue;
			return ref new  Windows::UI::Xaml::Media::SolidColorBrush(colorBoxed->Value);
		}

		virtual Platform::Object^ ConvertBack(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language)
		{
			auto brush =  dynamic_cast<Windows::UI::Xaml::Media::SolidColorBrush^>(value);
			if(brush == nullptr)
				return Windows::UI::Xaml::DependencyProperty::UnsetValue;
			return brush->Color;
		}
	};
}
