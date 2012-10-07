//
// ControlDraggedAdorner.h
// Declaration of the ControlDraggedAdorner class.
//

#pragma once

namespace Gimme
{
	namespace UI
	{
		namespace Controls
		{
			[Windows::UI::Xaml::TemplatePartAttribute]//(Name = "PART_Content", Type = {Windows::UI::Xaml::Controls::ContentControl::typeid->FullName,  Windows::UI::Xaml::Interop::TypeKind::Custom })]
			public ref class ControlDraggedAdorner sealed : public Windows::UI::Xaml::Controls::Control
			{
			public:
				ControlDraggedAdorner();

				property Windows::UI::Xaml::Controls::ContentControl^ Content
				{
					Windows::UI::Xaml::Controls::ContentControl^ get() { return _content; }
				}
				virtual void OnApplyTemplate() override;

			private:
				Windows::UI::Xaml::Controls::ContentControl^ _content;
			};
		}
	}
}