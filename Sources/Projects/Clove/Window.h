#ifndef __WINDOW_H
#define __WINDOW_H

#include <Windows.h>

#include "Bitmap32.h"

using namespace System;

namespace Clove
{
	public ref class Window abstract
	{
	private:

		delegate LRESULT WindowProcDelegate(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		static Window^ m_current;
		WindowProcDelegate^ m_proc;
		ATOM m_class;
		HWND m_handle;

		LRESULT Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		
	public:

		Window();
		~Window();

		static property Window^ Current { Window^ get() { return m_current; } }

		void Open();
		void Close();

		void Draw(Bitmap32^ bitmap, int srcX, int srcY, int dstX, int dstY , int width, int height);
		void Draw(Bitmap32^ bitmap) { Draw(bitmap, 0, 0, 0, 0, bitmap->Width, bitmap->Height); }

		void Invalidate() { InvalidateRect(m_handle, NULL, false); }

	protected:
		virtual void OnCloseRequest() { Close(); }
		virtual void OnSizeChanged(int width, int height) { }
		virtual void OnDrawRequest() { }
		
		void SetSize(int width, int height);
		void SetTitle(String^ title);

	};
	
	public ref class AnimationWindow abstract : Window 
	{
	private:
		Bitmap32^ m_buffer;
		Nullable<DateTime> m_previousFrame;
		double m_averageSecondsPerFrame;

	protected:

		property double FramesPerSecond { double get() { return 1.0 / m_averageSecondsPerFrame; } }

		virtual void OnSizeChanged(int width, int height) override sealed
		{
			if(m_buffer == nullptr || m_buffer->Width != width || m_buffer->Height != height)
			{
				m_buffer = gcnew Bitmap32(width, height);
			}
		}

		virtual void OnDrawRequest() override sealed
		{
			if(m_buffer != nullptr)
			{
				DateTime now = DateTime::Now;
				TimeSpan elapsed;
				
				if(m_previousFrame.HasValue)
				{
					elapsed = now - m_previousFrame.Value;
				} else {
					elapsed = TimeSpan();
				}
				
				m_averageSecondsPerFrame = (m_averageSecondsPerFrame * 99 + elapsed.TotalSeconds) / 100;

				m_previousFrame = now;

				OnDrawRequest(m_buffer, elapsed);
				Draw(m_buffer);
				
			}

			Invalidate();
		}

		virtual void OnDrawRequest(Bitmap32^ buffer, TimeSpan elapsed) { }
	};

	public ref class DisplayWindow sealed : Window 
	{
	private:
		Bitmap32^ m_bitmap;
		
		DisplayWindow(Bitmap32^ bitmap) : m_bitmap(bitmap)
		{
		}

	public:
		static void Show(Bitmap32^ bitmap)
		{
			DisplayWindow^ window = gcnew DisplayWindow(bitmap);
			window->SetSize(bitmap->Width, bitmap->Height);
			window->Open();
		}

	protected:
		virtual void OnDrawRequest() override
		{
			Draw(m_bitmap);
		}
	};
}

#endif