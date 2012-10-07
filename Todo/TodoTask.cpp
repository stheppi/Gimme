#include "pch.h"
#include "TodoTask.h"


using namespace Platform;

namespace Todo
{

	TodoTask::TodoTask() : _isCompleted(false), _description(nullptr)
	{
	}

	TodoTask::TodoTask(Platform::String^ description): _isCompleted(false), _description(description)
	{}


	String^ TodoTask::Description::get()
	{
		return _description;
	}

	void TodoTask::Description::set(Platform::String^ value)
	{
		if(_description != value)
		{
			_description = value;
			OnPropertyChanged(L"Description");
		}
	}

	bool TodoTask::IsCompleted::get()
	{
		return _isCompleted;
	}

	void TodoTask::IsCompleted::set(bool value)
	{
		if(_isCompleted != value)
		{
			_isCompleted = value;
			OnPropertyChanged(L"IsCompleted");
		}
	}

	int TodoTask::Index::get()
	{
		return _index;
	}

	void TodoTask::Index::set(int value)
	{
		if(_index != value)
		{
			_index= value;
			OnPropertyChanged("Index");
		}
	}
}