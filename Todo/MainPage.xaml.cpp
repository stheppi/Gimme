//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace Todo;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

using namespace Gimme::UI::Gestures;

MainPage::MainPage() 
	:_viewModel (ref new TodoTaskViewModel())
{

	_viewModel->Tasks->VectorChanged += ref new VectorChangedEventHandler<TodoTask^>(this, &MainPage::OnTodoTasksChanged);
	InitializeTasks();
	DataContext = _viewModel;

	InitializeComponent();
	InitializeGestures();

	/*todoList->DragOver += ref new DragEventHandler([](Object^ sender, DragEventArgs^ e)
	{
	int i = 0;
	i++;
	});*/
}

void MainPage::OnTodoTasksChanged(IObservableVector<TodoTask^>^ sender, IVectorChangedEventArgs^ e)
{
	switch(e->CollectionChange)
	{
	case CollectionChange::ItemInserted:
	case CollectionChange::ItemRemoved:
		//sender->GetAt(e->Index)->Index = e->Index + 1;
		for(unsigned int i = e->Index; i < sender->Size; ++i)
		{
			sender->GetAt(i)->Index = static_cast<int>(i) + 1;
		}
		break;


	case CollectionChange::ItemChanged:
		sender->GetAt(e->Index)->Index = e->Index + 1;
		break;
	}
}

void MainPage::InitializeGestures()
{
	auto gestures = GestureManager::GetGestures(todoList);

	swipeGesture = dynamic_cast<SwipeGesture^>(gestures->GetAt(1));
	swipeGesture->ItemSwiped += ref new ItemSwipedEventHandler([](Object^ item, SwipeDirection direction)
	{
		if(direction == SwipeDirection::Right)
		{
			TodoTask^ task = dynamic_cast<TodoTask^>(item);
			task->IsCompleted = true;
			return false;
		}
		return true;
	});

	swipeGesture->RemoveItem += ref new RemoveItemEventHandler([this](Object^ item)
	{
		TodoTask^ task = dynamic_cast<TodoTask^>(item);
		unsigned int index = 0;
		if(_viewModel->Tasks->IndexOf(task,&index))
		{
			auto tasks = _viewModel->Tasks;
			auto size = tasks->Size;
			_viewModel->Tasks->RemoveAt(index);
		}
	});

	pullNewItemGesture =  dynamic_cast<PullNewItemGesture^>(gestures->GetAt(2));

	pullNewItemGesture->Notify += ref new EventHandler<PullNewItemEventArgs^>([this](Object^ sender, PullNewItemEventArgs^ e)
	{
		const double minDistance = 50;
		Windows::Foundation::Point point;
		switch(e->Action)
		{
		case PullNewItemAction::Started:

			point = e->StartPosition;

			Gimme::UI::Extensions::FrameworkElementExtension::TranslateX(pullStart, point.X - pullStart->ActualWidth / 2);
			Gimme::UI::Extensions::FrameworkElementExtension::TranslateY(pullStart, point.Y - pullStart->ActualHeight / 2);

			Gimme::UI::Extensions::FrameworkElementExtension::TranslateX(pullArrow, point.X - pullArrow->ActualWidth / 2);
			Gimme::UI::Extensions::FrameworkElementExtension::TranslateY(pullArrow, point.Y );

			pullNotification->Text= L"Pull to add a new item...";
			Gimme::UI::Extensions::FrameworkElementExtension::TranslateX(pullNotification, point.X - pullNotification->ActualWidth / 2);
			Gimme::UI::Extensions::FrameworkElementExtension::TranslateY(pullNotification, point.Y + pullArrow->ActualHeight + 2);

			pullArrow->Visibility = Windows::UI::Xaml::Visibility::Visible;
			pullStart->Visibility = Windows::UI::Xaml::Visibility::Visible;
			pullNotification->Visibility = Windows::UI::Xaml::Visibility::Visible;
			break;

		case PullNewItemAction::Move:

			if(e->Distance > 0)
			{	
				Gimme::UI::Extensions::FrameworkElementExtension::TranslateY(pullStart, min(e->StartPosition.Y + minDistance, e->CurrentPosition.Y));
				if(e->Distance >= minDistance)
					pullNotification->Text= L"Release to add a new item...";
			}
			else
			{
				Gimme::UI::Extensions::FrameworkElementExtension::TranslateY(pullStart, 0);
			}

			break;

		case PullNewItemAction::Released:
			pullStart->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
			pullArrow->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
			pullNotification->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
			if(e->Distance >= minDistance)
			{
				TodoTask^ task = ref new TodoTask();
				_viewModel->Tasks->InsertAt(0, task);
				e->Item = task;
			}
			break;
		}
	});

	dragArrangeGesture = dynamic_cast<DragArrangeGesture^>(gestures->GetAt(3));

	dragArrangeGesture->MoveItem += ref new MoveItemEventHandler([this](int from, int to)
	{
		auto task = _viewModel->Tasks->GetAt(from);
		_viewModel->Tasks->RemoveAt(from);
		_viewModel->Tasks->InsertAt(to, task);
		return true;
	});

	dragArrangeGesture->Adorner = dragAdorner;
}

void MainPage::InitializeTasks()
{
	_viewModel->Tasks->Append(ref new TodoTask("Book flight to Barcelona"));
	_viewModel->Tasks->Append(ref new TodoTask("Return library books"));
	_viewModel->Tasks->Append(ref new TodoTask("Book restaurant for anniversary"));
	_viewModel->Tasks->Append(ref new TodoTask("Update app presentation"));
	_viewModel->Tasks->Append(ref new TodoTask("Make dentist appointment"));
	_viewModel->Tasks->Append(ref new TodoTask("Book Brazil F1 tickets"));
	_viewModel->Tasks->Append(ref new TodoTask("Pay electricity bill"));
	_viewModel->Tasks->Append(ref new TodoTask("Buy gift for Alex"));
	_viewModel->Tasks->Append(ref new TodoTask("Order new camera lenses"));
	_viewModel->Tasks->Append(ref new TodoTask("Clean the kitchen"));
	_viewModel->Tasks->Append(ref new TodoTask("Renew gym membership"));
	_viewModel->Tasks->Append(ref new TodoTask("Buy groceries"));
	_viewModel->Tasks->Append(ref new TodoTask("Pick up dry cleaning"));
	_viewModel->Tasks->Append(ref new TodoTask("Take out the trash"));
	_viewModel->Tasks->Append(ref new TodoTask("Meet up with Jane"));

}

/// <summary>
/// Invoked when this page is about to be displayed in a Frame.
/// </summary>
/// <param name="e">Event data that describes how this page was reached.  The Parameter
/// property is typically used to configure the page.</param>
void MainPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	(void) e;	// Unused parameter
}
