#pragma once

namespace Gimme
{
	namespace UI
	{
		namespace Extensions
		{

			class DependencyObjectExtension sealed
			{
			public:
				/// <summary>
				/// 
				/// <summary>
				template<typename T>
				static T GetParent(Windows::UI::Xaml::DependencyObject^ source)
				{
					auto parent = Windows::UI::Xaml::Media::VisualTreeHelper::GetParent(source);
					while(parent != nullptr)
					{
						T t = dynamic_cast<T>(parent) ;
						if( nullptr != t)
							return t;

						parent = Windows::UI::Xaml::Media::VisualTreeHelper::GetParent(parent);
					}

					return nullptr;
				}
			};
		}
	}
}