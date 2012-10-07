#pragma once

#include "GestureBase.h"

namespace Gimme
{
	namespace UI
	{
		namespace Gestures
		{
			/// <summary>
			/// Represents the base class for handling the edit item gesture.
			/// </summary>
			public ref class EditItemGestureBase: public GestureBase
			{
			public:
				/// <summary>
				/// Gets or sets the degree of the ItemsControl items opacity while editing one item.
				/// </summary>
				property double Opacity
				{
					double get()
					{
						return _opacity;
					}
					void set(double value) 
					{
						if(value < 0 || value > 1)
						{
							value = 0;
						}
						_opacity= value;
					}
				}

			internal:
				EditItemGestureBase(void);

			protected:
				/// <summary>
				/// Called to initialize the edit controls.
				/// </summary>
				void InitializeEditControls();

				/// <summary>
				/// Shows/Displays the edit control.
				/// </summary>
				void DisplayEditControls(bool flag);

				/// <summary>
				/// Animate the ItemsControl items opacity.
				/// </summary>
				void AnimateItemsOpacity(bool flag);

				/// <summary>
				/// Called to complete the edit gesture.
				/// </summary>
				virtual void OnGestureCompleted();

				/// <summary>
				/// Called to set the current item content.
				/// </summary>
				void SetPayload(Platform::Object^ payload)
				{
					_payload = payload;
				}

			private:
				/// <summary>
				/// Opacity degree to be applied to the ItemsControl items while editing.
				/// </summary>
				double _opacity;

				/// <summary>
				/// The token resulted from subscribing to the _txtEdit::FocusLost event.
				/// </summary>
				Windows::Foundation::EventRegistrationToken _editLostFocusToken;

				/// <summary>
				/// Represents the Content of the ItemsControl item being edited.
				/// </summary>
				Platform::Object^ _payload;

				/// TextBlock/TextBox instances 
				Windows::UI::Xaml::Controls::TextBlock^ _txtBlock;
				Windows::UI::Xaml::Controls::TextBox^ _txtEdit;
			};
		}
	}
}
