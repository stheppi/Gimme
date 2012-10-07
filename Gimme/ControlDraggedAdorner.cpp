//
// ControlDraggedAdorner.cpp
// Implementation of the ControlDraggedAdorner class.
//

#include "pch.h"
#include "ControlDraggedAdorner.h"

using namespace Gimme;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Documents;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;

namespace Gimme
{
	namespace UI
	{
		namespace Controls
		{
			ControlDraggedAdorner::ControlDraggedAdorner()
			{
				DefaultStyleKey = "Gimme.UI.Controls.ControlDraggedAdorner";
			}

			void ControlDraggedAdorner::OnApplyTemplate()
			{
				_content = dynamic_cast<ContentControl^>(GetTemplateChild(L"PART_Content"));
			}
		}
	}
}