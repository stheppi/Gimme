#include "pch.h"
#include "TodoTaskViewModel.h"

using namespace Platform::Collections;

namespace Todo
{
	TodoTaskViewModel::TodoTaskViewModel(void) 
		:_tasks(ref new Vector<TodoTask^>())
	{
	}


	TodoTaskViewModel::~TodoTaskViewModel(void)
	{
	}
}