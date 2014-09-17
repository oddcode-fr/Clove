
#include <vcclr.h>

#include "Window.h"

namespace Clove
{
	Window::Window()
	{
		m_proc = gcnew WindowProcDelegate(this, &Window::Proc);

		HINSTANCE instance = GetModuleHandle(NULL);

		WNDCLASS wc;
		wc.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
		wc.lpfnWndProc = (WNDPROC)(void*)System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(m_proc);
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = instance;
		wc.hIcon = NULL;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = L"CLOVE";

		m_class = RegisterClass(&wc);

		m_handle = CreateWindow(
			(LPCTSTR)m_class,
			L"",
			WS_OVERLAPPEDWINDOW,
			0, 0, 800, 600,
			NULL, NULL, instance, NULL
		);
	}

	Window::~Window()
	{
		DestroyWindow(m_handle);
		UnregisterClass((LPCTSTR)m_class, GetModuleHandle(NULL));
	}

	LRESULT Window::Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_CLOSE:
			OnCloseRequest();
			return 0;

		case WM_SIZE:
			RECT r;
			GetClientRect(hwnd, &r);
			if(r.right != 0 && r.bottom != 0)
			{
				OnSizeChanged(r.right, r.bottom);
			}
			return 0;

		case WM_PAINT:
			PAINTSTRUCT ps;
			BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps);
			OnDrawRequest(); // TODO invalidated rect
			return 0;

		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}	
	}

	void Window::Open()
	{
		if(m_current == nullptr)
		{
			m_current = this;

			try {

				ShowWindow(m_handle, SW_SHOW);

				MSG msg;

				while(GetMessage(&msg, NULL, 0, 0) > 0)
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				ShowWindow(m_handle, SW_HIDE);

			} 
			finally {
				m_current = nullptr;
			}
		}
	}

	void Window::Close()
	{
		if(m_current == this)
		{
			PostQuitMessage(0);
		}
	}

	void Window::Draw(Bitmap32^ bitmap, int srcX, int srcY, int dstX, int dstY , int width, int height)
	{
		Buffer32^ buffer = bitmap->m_buffer;

		BITMAPINFO bmi;
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = buffer->Width;
		bmi.bmiHeader.biHeight = buffer->Height;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;
		bmi.bmiHeader.biSizeImage = 0;
		bmi.bmiHeader.biXPelsPerMeter = 0;
		bmi.bmiHeader.biYPelsPerMeter = 0;
		bmi.bmiHeader.biClrUsed = 0;
		bmi.bmiHeader.biClrImportant = 0;


		HDC hdc = GetDC(m_handle);

		SetDIBitsToDevice(
			hdc, 
			dstX, dstY, 
			width, height, 
			srcX, srcY, 
			0, height,
			bitmap->m_buffer->Data,
			&bmi, 
			DIB_RGB_COLORS
		);

		ReleaseDC(m_handle, hdc);
	}

	void Window::SetSize(int width, int height)
	{
		RECT r = { 0, 0, width, height };
		DWORD style = GetWindowLong(m_handle, GWL_STYLE);
		AdjustWindowRect(&r, style, FALSE);
		SetWindowPos(m_handle, NULL, 0, 0, r.right - r.left, r.bottom - r.top, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
	}

	void Window::SetTitle(String^ title)
	{
		pin_ptr<const wchar_t> wtitle = PtrToStringChars(title);
		SetWindowText(m_handle, wtitle);
	}

}