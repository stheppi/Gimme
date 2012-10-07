
#pragma once
namespace Gimme
{
	namespace UI
	{
		namespace Extensions
		{
			/// <summary>
			/// Represents a method that handles action events.
			/// </summary>
			public delegate void ActionEventHandler();

			/// <summary>
			///Represents the class offering the option to create a DoubleAnimation and animate the given property.
			/// </summary>
			class AnimationExtension sealed
			{
			public:
				/// <summary>
				///Represents the method to animate the given property for the target object.
				/// </summary>
				static void Animate(Windows::UI::Xaml::DependencyObject^ target, double from, double to, 
					Platform::String^ property, int duration, int startOffset, 
					Windows::UI::Xaml::Media::Animation::EasingFunctionBase^ easing = nullptr,
					ActionEventHandler^ completedCallback = nullptr)
				{
					Windows::Foundation::TimeSpan durationTimeSpan;
					//nanoseconds to ms
					durationTimeSpan.Duration = duration * 10000;

					//create the animation
					auto animation = ref new Windows::UI::Xaml::Media::Animation::DoubleAnimation();
					animation->From = from;
					animation->To = to;
					animation->EasingFunction = easing == nullptr ?  ref new Windows::UI::Xaml::Media::Animation::SineEase() : easing;
					animation->Duration = Windows::UI::Xaml::Duration(durationTimeSpan);

					//prepare the callback if present
					if (completedCallback != nullptr)
					{
						Windows::Foundation::EventRegistrationToken token;
						token = animation->Completed += ref new Windows::Foundation::EventHandler<Platform::Object^>([token, completedCallback](Platform::Object^ sender,Platform::Object^ args)
						{
							auto a = (Windows::UI::Xaml::Media::Animation::DoubleAnimation^)sender;
							a->Completed -= token;
							completedCallback();
						});
					}

					Windows::UI::Xaml::Media::Animation::Storyboard::SetTarget(animation, target);
					Windows::UI::Xaml::Media::Animation::Storyboard::SetTargetProperty(animation, property);

					auto storyBoard = ref new Windows::UI::Xaml::Media::Animation::Storyboard();

					Windows::Foundation::TimeSpan beginTimeSpan;
					beginTimeSpan.Duration = startOffset;
					storyBoard->BeginTime = beginTimeSpan;

					storyBoard->Duration  = Windows::UI::Xaml::Duration(durationTimeSpan);
					storyBoard->Children->Append(animation);
					storyBoard->Begin();
				}
			};
		}
	}
}
