#pragma once
#include <collection.h>

using namespace Platform::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::Foundation::Collections;

namespace Gimme
{
	namespace UI
	{
		namespace Extensions
		{
			class VisualTreeHelperExtension sealed
			{
			public:
				static IIterable<DependencyObject^>^ Descendants(DependencyObject^ source)
				{
					Vector<DependencyObject^>^ children = ref new Vector<DependencyObject^>();
					int count = VisualTreeHelper::GetChildrenCount(source);		
					for(int i = 0; i <count; ++i)
					{
						DependencyObject^ current=  VisualTreeHelper::GetChild(source, i);
						children->Append(current);

						IIterable<DependencyObject^>^ currentChildren = Descendants(current);
						std::for_each(begin(currentChildren), end(currentChildren),
							[&children](DependencyObject^ c)
						{
							children->Append(c);
						});
					}
					return children;
				}


				template<typename T>
				static IIterable<T>^ Descendants(DependencyObject^ source)
				{
					Vector<T>^ children = ref new Vector<T>();
					int count = VisualTreeHelper::GetChildrenCount(source);		
					for(int i = 0; i <count; ++i)
					{
						DependencyObject^ current=  VisualTreeHelper::GetChild(source, i);
						T t = dynamic_cast<T>(current);
						if(t != nullptr)
							children->Append(t);

						IIterable<T>^ currentChildren = Descendants<T>(current);
						std::for_each(begin(currentChildren), end(currentChildren),
							[&children](T c)
						{
							children->Append(c);
						});
					}
					return children;
				}

				template<typename T>
				static T FindDescendantBy(DependencyObject^ root, Platform::String^ name)
				{
					auto descendants = Descendants<T>(root);
					auto iter = std::find_if(begin(descendants), end(descendants),[&name](T d){ return d->Name == name;});
					if(iter == end(descendants))
						return nullptr;

					return *iter;
				}

				template<typename T>
				static T GetParent(DependencyObject^ source)
				{
					DependencyObject^ parent = VisualTreeHelper::GetParent(source);
					while (parent != nullptr)
					{
						if (dynamic_cast<T>(parent) != nullptr)
							break;
						parent = VisualTreeHelper::GetParent(parent);
					}
					return (T)parent;
				}
			};
		}
	}
}