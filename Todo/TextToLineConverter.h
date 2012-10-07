#pragma once

namespace Todo
{
	public ref class TextToLineConverter sealed : public Windows::UI::Xaml::Data::IValueConverter
	{
	public:
		virtual Platform::Object^ Convert(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language)
		{
			auto text = dynamic_cast<Platform::String^>(value);
			if(text == nullptr)
				return "";

			auto rvalue = ref new Platform::String( std::wstring(text->Length() + 2, '_').c_str());
			return rvalue;
		}

		virtual Platform::Object^ ConvertBack(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language)
		{
			throw ref new  Platform::NotImplementedException();
		}
	};
}
