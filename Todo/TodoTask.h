#pragma once

#include "BindableBase.h"

namespace Todo
{
	[Windows::UI::Xaml::Data::Bindable]
	public ref class TodoTask sealed : public BindableBase
	{
	public:
		TodoTask();
		TodoTask(Platform::String^ description);

		property Platform::String^ Description
		{
			Platform::String^ get();
			void set(Platform::String^ value);
		}

		property bool IsCompleted
		{
			bool get();
			void set(bool value);
		}

		property int Index
		{
			int get();
			void set(int value);
		}

	private:
		void FillCustomProperty();

		bool _isCompleted;
		int _index;
		Platform::String^ _description;

	};
}