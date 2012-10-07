#include "pch.h"
#include <algorithm>
#include "AnimationExtension.h"
#include "DragArrangeGesture.h"
#include "FrameworkElementExtension.h"
#include "ItemsControlExtension.h"

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Input;

using namespace Gimme::UI::Controls;
using namespace Gimme::UI::Extensions;

namespace Gimme
{
	namespace UI
	{
		namespace Gestures
		{
			DragArrangeGesture::DragArrangeGesture(): _AUTO_SCROLL_DETECTION_HEIGHT(80), _autoScrollTimer(ref new DispatcherTimer())
			{
				TimeSpan duration;
				//every 40ms
				duration.Duration = 40 * 10000;

				_autoScrollTimer->Interval = duration;
				_autoScrollTimer->Tick += ref new EventHandler<Object^>([this](Object^ sender, Object^ e) 
				{
					PerformAutoScroll();
					TranslateItems();
				});
			}


			DragArrangeGesture::~DragArrangeGesture() 
			{
			}

			ControlDraggedAdorner^ DragArrangeGesture::Adorner::get()
			{
				return _adorner;
			}

			void DragArrangeGesture::Adorner::set(ControlDraggedAdorner^ value)
			{
				if(_adorner != nullptr)
					_adorner->PointerReleased -= _adornerPointerReleasedToken;
				_adorner = value;
				if(_adorner != nullptr)
					_adornerPointerReleasedToken = _adorner->PointerReleased += ref new PointerEventHandler(this, &DragArrangeGesture::OnPointerReleased);
			}

			void DragArrangeGesture::OnAttached()
			{
				GestureBase::OnAttached();

				Target->AddHandler(UIElement::PointerPressedEvent, _pointerPressedHandler = ref new PointerEventHandler(this, &DragArrangeGesture::OnPointerPressed), true);
				Target->AddHandler(UIElement::PointerReleasedEvent, _pointerReleasedHandler = ref new PointerEventHandler(this, &DragArrangeGesture::OnPointerReleased), true);
				Target->AddHandler(UIElement::PointerMovedEvent, _pointerMovedHandler = ref new PointerEventHandler(this, &DragArrangeGesture::OnPointerMoved), true);

				Target->IsHoldingEnabled = true;

				Target->AddHandler(UIElement::HoldingEvent, _holdingHandler =  ref new HoldingEventHandler(this, &DragArrangeGesture::OnHolding), true);
			}

			void DragArrangeGesture::OnDetaching()
			{
				GestureBase::OnDetaching();

				Target->RemoveHandler(UIElement::PointerPressedEvent, _pointerPressedHandler);
				Target->RemoveHandler(UIElement::PointerReleasedEvent, _pointerReleasedHandler);
				Target->RemoveHandler(UIElement::PointerMovedEvent, _pointerMovedHandler); 
			}

			int DragArrangeGesture::GetIndex()
			{
				double location = FrameworkElementExtension::GetTranslateY(_adorner) + (TargetScrollViewer != nullptr ? TargetScrollViewer->VerticalOffset : 0) + _adorner->ActualHeight / 2;
				//int firstVisibleIndex = ItemsControlExtension::GetFirstVisibleIndex(Target);
				//we assume all elements have the same height!
				int index = (int)(location / _adorner->ActualHeight) ;//+ max( firstVisibleIndex - 1, 0);
				return min((int)Target->Items->Size-1, index);
			}

			void DragArrangeGesture::PerformAutoScroll()
			{
				double draglocation = FrameworkElementExtension::GetTranslateY(_adorner) + _adorner->ActualHeight / 2;
				//check if the current adorner is within scroll detection area
				if (draglocation < _AUTO_SCROLL_DETECTION_HEIGHT)
				{
					double location = (_AUTO_SCROLL_DETECTION_HEIGHT - draglocation);
					//ask the scroll viewer to scroll to the appropriate offset
					TargetScrollViewer->ScrollToVerticalOffset(TargetScrollViewer->VerticalOffset - location);
				}
				else if (draglocation > Target->ActualHeight - _AUTO_SCROLL_DETECTION_HEIGHT)
				{
					double location = (_AUTO_SCROLL_DETECTION_HEIGHT - (Target->ActualHeight - draglocation));
					//ask the scroll viewer to scroll to the appropriate offset
					TargetScrollViewer->ScrollToVerticalOffset(TargetScrollViewer->VerticalOffset + location);
				}
			}

			void TranslateItem(double offset, FrameworkElement^ target)
			{
				double targetLocation = target->Tag != nullptr ? (double)target->Tag : 0;
				if (targetLocation != offset)
				{
					TranslateTransform^ transform = FrameworkElementExtension::GetTranslateTransform(target);
					AnimationExtension::Animate(transform, transform->Y, offset, "Y", 150, 0);
					target->Tag = offset;
				}
			}

			void DragArrangeGesture::TranslateItems()
			{
				// find the index of the item the pointer device is on top of
				int dragIndex = GetIndex();

				// iterate over the items in the list and translate them vertically as required
				double offset = _adorner->ActualHeight;
				for (int i = 0; i < Target->Items->Size; i++)
				{
					FrameworkElement^ item = dynamic_cast<FrameworkElement^>(Target->ItemContainerGenerator->ContainerFromIndex(i));
					if (item == nullptr || item->Visibility == Visibility::Collapsed)
						continue;
					// determine if the item should be moved up or down
					if (i <=  dragIndex && i > _startIndex)
					{
						TranslateItem(-offset, item);
					}
					else if (i >=  dragIndex && i < _startIndex)
					{
						TranslateItem(offset, item);
					}
					else
					{
						TranslateItem(0, item);
					}
				}
			}

			void DragArrangeGesture::AdornerOpacityAnimationCompleted()
			{
				_adorner->Visibility = Visibility::Collapsed;
				//reset the adorner content
				_adorner->Content->Content = nullptr;
				//set the opacity back to 1 (we just animated to zero)
				_adorner->Opacity = 1;

				Target->Opacity = 1.0;

				//signal de gesture completed
				RaiseDetectedEvent(false);
			}

			void DragArrangeGesture::AdornerTranslationAnimationCompleted()
			{
				if (_currentDragIndex != _startIndex)
				{
					//Raise the event to move the item to the new position
					MoveItem(_startIndex, _currentDragIndex);

					//reset the RenderTransform for all the ItemsControl items
					for (int i = Target->Items->Size - 1; i >= 0; --i)
					{
						FrameworkElement^ item = dynamic_cast<FrameworkElement^>( Target->ItemContainerGenerator->ContainerFromIndex(i));
						if (item == nullptr || item->Visibility == Visibility::Collapsed)
							continue;
						TranslateItem(0, item);
					}
				}
				_item->Opacity = 1;

				// fade out the dragged item and collapse on completion
				AnimationExtension::Animate( _adorner, 1.0, 0.0, "Opacity", 1000, 0, nullptr, ref new ActionEventHandler(this, &DragArrangeGesture::AdornerOpacityAnimationCompleted));
			}


			void DragArrangeGesture::HandleGestureStart(HoldingRoutedEventArgs^ e)
			{
				if (e->HoldingState == HoldingState::Started  && _adorner != nullptr)
				{
					_item = VisualTreeHelperExtension::GetParent<SelectorItem^>(static_cast<DependencyObject^>(e->OriginalSource));
					if (_item != nullptr)
					{
						//capture the pointer device to keep receiving events
						Target->CapturePointer(_pointer);

						//set the adorner size
						_adorner->Width = _item->ActualWidth;
						_adorner->Height = _item->ActualHeight;

						//force the control template to be applied
						_adorner->ApplyTemplate();

						//let the adorner display the item being dragged
						_adorner->Content->Content = _item->Content;
						_adorner->Content->ContentTemplate = _item->ContentTemplate;

						_adorner->Visibility = Visibility::Visible;

						//position the adorner at the same location as the current item
						auto point = _item->TransformToVisual(Target)->TransformPoint(Point(0,0));
						FrameworkElementExtension::TranslateY(_adorner, point.Y);
						FrameworkElementExtension::TranslateX(_adorner, point.X);

						unsigned int index = 0;
						Target->Items->IndexOf(_item->Content, &index);
						_item->Opacity = 0;
						_startIndex = index;// GetItemIndex(_item->Content);

						//Change the ItemsControl opacity
						Target->Opacity = 0.3;
						_adorner->Focus(FocusState::Pointer | FocusState::Keyboard);

						_isHolding = true;

						//start the auto-scroll timer
						_autoScrollTimer->Start();

						//signal the gesture has been detected
						RaiseDetectedEvent(true);
					}
				}
			}

			void DragArrangeGesture::HandleGestureCompleted(Pointer^ pointer)
			{
				if (_isHolding)
				{
					_autoScrollTimer->Stop();
					_isHolding = false;
					Target->ReleasePointerCapture(pointer);

					//slide the adorner into the appropriate item location
					_currentDragIndex = GetIndex();
					double  location = _currentDragIndex * _adorner->ActualHeight - TargetScrollViewer->VerticalOffset;
					auto trans = FrameworkElementExtension::GetTranslateTransform(_adorner);
					AnimationExtension::Animate(trans, FrameworkElementExtension::GetTranslateY(_adorner), location, "Y", 200, 0, nullptr,ref new ActionEventHandler(this, &DragArrangeGesture::AdornerTranslationAnimationCompleted));
				}
			}

			void DragArrangeGesture::OnHolding(Object^ sender, HoldingRoutedEventArgs^ e)
			{
				HandleGestureStart(e);
			}

			void DragArrangeGesture::OnPointerReleased(Object^ sender, PointerRoutedEventArgs^ e)
			{
				HandleGestureCompleted(e->Pointer);
			}

			void DragArrangeGesture::OnPointerPressed(Object^ sender, PointerRoutedEventArgs^ e)
			{
				if (Adorner == nullptr)
					return;
				_pointer = e->Pointer;
			}

			void DragArrangeGesture::OnPointerMoved(Object^ sender, PointerRoutedEventArgs^ e)
			{
				if (!_isHolding)
					return;

				//foreach (var p in e.GetIntermediatePoints(_itemsControl))
				//{
				//    _adorner.TranslateY(p.Position.Y - _item.ActualHeight/2);
				//}
				double y = min(Target->ActualHeight - _item->ActualHeight, max(0.0, (double)e->GetCurrentPoint(Target)->Position.Y - _item->ActualHeight / 2));

				//reposition the adorner
				FrameworkElementExtension::TranslateY(_adorner,y);
			}
		}
	}
}