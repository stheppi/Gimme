#pragma once

#include "BindableBase.h"

namespace Todo
{
	[Windows::UI::Xaml::Data::Bindable]
	[Windows::Foundation::Metadata::WebHostHiddenAttribute]
	public ref class TodoTaskViewModel sealed : public BindableBase
	{
	public:
		TodoTaskViewModel();
		virtual ~TodoTaskViewModel();

		property Windows::Foundation::Collections::IObservableVector<TodoTask^>^ Tasks
		{
			Windows::Foundation::Collections::IObservableVector<TodoTask^>^ get()
			{
				return _tasks;
			}
		}

	private:
		Platform::Collections::Vector<TodoTask^>^ _tasks;
	};
}