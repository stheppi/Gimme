#pragma once

namespace Gimme
{
	namespace UI
	{
		namespace Extensions
		{
			public ref class FrameworkElementExtension sealed
			{
			public:
				
				static void TranslateX(Windows::UI::Xaml::FrameworkElement^ fe, double offset)
				{
					auto transformation = dynamic_cast<Windows::UI::Xaml::Media::TranslateTransform^>(fe->RenderTransform);
					if (transformation == nullptr)
					{
						transformation = ref new Windows::UI::Xaml::Media::TranslateTransform();
						fe->RenderTransform = transformation;
					}
					transformation->X = offset;
				}



				static double GetTranslateX(Windows::UI::Xaml::FrameworkElement^ fe)
				{
					auto transform = dynamic_cast<Windows::UI::Xaml::Media::TranslateTransform^>(fe->RenderTransform);
					if (transform == nullptr)
						return 0;
					return transform->X;
				}

				static void TranslateY(Windows::UI::Xaml::FrameworkElement^ fe, double offset)
				{
					auto transformation = dynamic_cast<Windows::UI::Xaml::Media::TranslateTransform^>(fe->RenderTransform);
					if (transformation == nullptr)
					{
						transformation = ref new Windows::UI::Xaml::Media::TranslateTransform();
						fe->RenderTransform = transformation;
					}
					transformation->Y = offset;
				}


				static double GetTranslateY(Windows::UI::Xaml::FrameworkElement^ fe)
				{
					auto transform = dynamic_cast<Windows::UI::Xaml::Media::TranslateTransform^>(fe->RenderTransform);
					if (transform == nullptr)
						return 0;
					return transform->Y;
				}


				static Windows::UI::Xaml::Media::TranslateTransform^ GetTranslateTransform(Windows::UI::Xaml::FrameworkElement^ target)
				{
					auto transform = dynamic_cast<Windows::UI::Xaml::Media::TranslateTransform^>(target->RenderTransform);
					if (transform == nullptr)
					{
						transform = ref new Windows::UI::Xaml::Media::TranslateTransform();
						target->RenderTransform = transform;
					}
					return transform;
				}

			private:
				FrameworkElementExtension();
			};
		}
	}
}