#pragma once
#include <algorithm>
#include "VisualTreeHelperExtension.h"

namespace Gimme
{
	namespace UI
	{
		namespace Extensions
		{

			class ItemsControlExtension sealed
			{
			public :
				static  Windows::Foundation::Collections::IIterable<Windows::UI::Xaml::FrameworkElement^>^ GetVisibleItems(Windows::UI::Xaml::Controls::ItemsControl^ itemsControl)
				{
					Windows::UI::Xaml::Controls::Panel^ itemsHostPanel;
					if (itemsControl->Tag == nullptr)
					{
						auto descendants = VisualTreeHelperExtension::Descendants<Windows::UI::Xaml::Controls::Panel^>(itemsControl);//.Cast<Panel>().Where(p => p.IsItemsHost).SingleOrDefault();
						auto iter = std::find_if(begin(descendants), end(descendants), [](Windows::UI::Xaml::Controls::Panel^ panel) {return panel->IsItemsHost;});
						iter++;
						itemsControl->Tag = *iter;
						itemsHostPanel =*iter;
					}
					else
						itemsHostPanel = (Windows::UI::Xaml::Controls::Panel^)itemsControl->Tag;

					Windows::UI::Xaml::Controls::VirtualizingStackPanel^ panel =dynamic_cast<Windows::UI::Xaml::Controls::VirtualizingStackPanel^>(itemsHostPanel);

					Platform::Collections::Vector<Windows::UI::Xaml::FrameworkElement^>^ items = ref new Platform::Collections::Vector<Windows::UI::Xaml::FrameworkElement^>();
					if (panel != nullptr)
					{
						int firstVisibleIndex = (int)panel->VerticalOffset;
						int visibleItemsCount = (int)panel->ViewportHeight;

						for (int i = firstVisibleIndex; i < firstVisibleIndex + visibleItemsCount + 5; i++)
						{
							auto item = dynamic_cast<Windows::UI::Xaml::FrameworkElement^>(itemsControl->ItemContainerGenerator->ContainerFromIndex(i));
							if(item != nullptr)
								items->Append(item);
						}
					}
					else
					{
						for (int i = 0; i < itemsControl->Items->Size; i++)
						{
							auto item = dynamic_cast<Windows::UI::Xaml::FrameworkElement^>(itemsControl->ItemContainerGenerator->ContainerFromIndex(i));
							if(item != nullptr)
							{
								Windows::Foundation::Point point = item->TransformToVisual(itemsControl)->TransformPoint(Windows::Foundation::Point());
								if(point.Y + item->ActualHeight > 0 && point.Y - item->ActualHeight < itemsControl->ActualHeight)
									items->Append(item);
							}
						}
					}

					return items;
				}

				static int GetFirstVisibleIndex(Windows::UI::Xaml::Controls::ItemsControl^ itemsControl)
				{
					Windows::UI::Xaml::Controls::Panel^ itemsHostPanel;
					if (itemsControl->Tag == nullptr)
					{
						auto descendants = VisualTreeHelperExtension::Descendants<Windows::UI::Xaml::Controls::StackPanel^>(itemsControl);
						auto iter = std::find_if(begin(descendants), end(descendants), [](Windows::UI::Xaml::Controls::StackPanel^ panel) {return panel->IsItemsHost;});
						//iter++;
						itemsControl->Tag = *iter;
						itemsHostPanel = *iter;
					}
					else
						itemsHostPanel = (Windows::UI::Xaml::Controls::Panel^)itemsControl->Tag;

					Windows::Foundation::Point transformedPoint(0,0);
					for (int i = 0; i < itemsControl->Items->Size; i++)
					{
						auto item = dynamic_cast<Windows::UI::Xaml::FrameworkElement^>(itemsControl->ItemContainerGenerator->ContainerFromIndex(i));
						if(item != nullptr && item->Visibility == Windows::UI::Xaml::Visibility::Visible)
						{
							auto point = item->TransformToVisual(itemsControl)->TransformPoint(transformedPoint);
							if(point.Y + item->ActualHeight >= 0 && point.Y < itemsControl->ActualHeight)
								return i;
						}
					}
					return 0;
				}
			private:
				ItemsControlExtension(void);

				~ItemsControlExtension(void);
			};
		}
	}
}