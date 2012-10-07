#pragma once

namespace Todo
{
	public ref class BindableBase :  public Windows::UI::Xaml::DependencyObject, public Windows::UI::Xaml::Data::INotifyPropertyChanged
	{
	public:
		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler^ PropertyChanged;

	protected:

		virtual void OnPropertyChanged(Platform::String^ propertyName) sealed
		{
			PropertyChanged(this, ref new Windows::UI::Xaml::Data::PropertyChangedEventArgs(propertyName));
		}
	};
}