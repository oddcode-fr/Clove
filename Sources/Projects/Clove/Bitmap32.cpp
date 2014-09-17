
#include <wincodec.h>
#include <shlwapi.h>
#include <vcclr.h>

#include "Bitmap32.h"


using namespace System;

namespace Clove
{

	Bitmap32::Bitmap32(Int32 width, Int32 height)
	{
		m_buffer = gcnew Buffer32(width, height);
		m_x = 0;
		m_y = 0;

		m_bitmap = new NativeBitmap32(
			m_buffer->Data,
			width,
			height,
			-int(m_buffer->Stride)
		);
	}

	Bitmap32::Bitmap32(Bitmap32^ parent, Int32 x, Int32 y, Int32 width, Int32 height)
	{
		m_buffer = parent->m_buffer;
		m_x = parent->m_x + x;
		m_y = parent->m_y + y;

		m_bitmap = new NativeBitmap32(
			m_buffer->Data + (m_buffer->Height - m_y - height) * m_buffer->Stride + m_x * 4,
			width,
			height,
			-int(m_buffer->Stride)
		);
	}

	Bitmap32::~Bitmap32()
	{
		delete m_bitmap;
	}

	void Bitmap32::Save(String^ filename)
	{
		HRESULT hr;
		IStream* stream = NULL;
		IWICImagingFactory* factory = NULL;
		IWICBitmapEncoder* encoder = NULL;
		IWICBitmapFrameEncode* frame = NULL;

		try {
			pin_ptr<const wchar_t> wfilename = PtrToStringChars(filename);

			hr = SHCreateStreamOnFileEx(wfilename, STGM_CREATE | STGM_WRITE, FILE_ATTRIBUTE_NORMAL, TRUE, NULL, &stream);
			if (!SUCCEEDED(hr)) {
				throw gcnew Exception(String::Format(L"SHCreateStreamOnFileW failed : 0x{0:X}", hr));
			}

			hr = CoCreateInstance(CLSID_WICImagingFactory1, NULL, CLSCTX_INPROC_SERVER,
				IID_IWICImagingFactory, (LPVOID*)&factory);
			if (!SUCCEEDED(hr)) {
				throw gcnew Exception(String::Format(L"CoCreateInstance failed : 0x{0:X}", hr));
			}

			hr = factory->CreateEncoder(GUID_ContainerFormatPng, NULL, &encoder);
			if (!SUCCEEDED(hr))
			{
				throw gcnew Exception(String::Format(L"IWICImagingFactory::CreateEncoder failed : 0x{0:X}", hr));
			}

			hr = encoder->Initialize(stream, WICBitmapEncoderNoCache);
			if (!SUCCEEDED(hr))
			{
				throw gcnew Exception(String::Format(L"IWICBitmapEncoder::Initialize failed : 0x{0:X}", hr));
			}

			hr = encoder->CreateNewFrame(&frame, NULL);
			if (!SUCCEEDED(hr))
			{
				throw gcnew Exception(String::Format(L"IWICBitmapEncoder::CreateNewFrame failed : 0x{0:X}", hr));
			}

			hr = frame->Initialize(NULL);
			if (!SUCCEEDED(hr))
			{
				throw gcnew Exception(String::Format(L"IWICBitmapFrameEncode::Initialize failed : 0x{0:X}", hr));
			}

			GUID format = GUID_WICPixelFormat32bppBGRA;

			hr = frame->SetPixelFormat(&format);
			if (!SUCCEEDED(hr))
			{
				throw gcnew Exception(String::Format(L"IWICBitmapFrameEncode::SetSize failed : 0x{0:X}", hr));
			}
			
			hr = frame->SetSize(this->Width, this->Height);
			if (!SUCCEEDED(hr))
			{
				throw gcnew Exception(String::Format(L"IWICBitmapFrameEncode::SetSize failed : 0x{0:X}", hr));
			}

			for (unsigned int y = 0; y < m_bitmap->m_rbuf.height(); y++)
			{
				hr = frame->WritePixels(
					1,
					m_bitmap->m_rbuf.stride_abs(),
					m_bitmap->m_rbuf.stride_abs(),
					(BYTE*)m_bitmap->m_rbuf.row(y).ptr
				);
				if (!SUCCEEDED(hr))
				{
					throw gcnew Exception(String::Format(L"IWICBitmapFrameEncode::WritePixels failed : 0x{0:X}", hr));
				}
			}


			hr = frame->Commit();
			if (!SUCCEEDED(hr))
			{
				throw gcnew Exception(String::Format(L"IWICBitmapFrameEncode::Commit failed : 0x{0:X}", hr));
			}

			hr = encoder->Commit();
			if (!SUCCEEDED(hr))
			{
				throw gcnew Exception(String::Format(L"IWICBitmapEncoder::Commit failed : 0x{0:X}", hr));
			}

		}
		finally {
			if (frame) frame->Release();
			if (encoder) encoder->Release();
			if (factory) factory->Release();
			if (stream) stream->Release();
		}
	}
}