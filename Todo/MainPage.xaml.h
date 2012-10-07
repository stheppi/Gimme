//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"
#include "TodoTaskViewModel.h"



namespace Todo
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();

	protected:
		virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

	private:
		void InitializeTasks();
		void InitializeGestures();
		void OnTodoTasksChanged(Windows::Foundation::Collections::IObservableVector<TodoTask^>^ sender, Windows::Foundation::Collections::IVectorChangedEventArgs^ e);
		Platform::Collections::Vector<Gimme::UI::Gestures::IGesture^>^ _gestures;
		TodoTaskViewModel^ _viewModel;
	};
}
