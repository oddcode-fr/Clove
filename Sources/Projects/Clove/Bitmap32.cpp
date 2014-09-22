
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

	array<Byte,1>^ Bitmap32::Save()
	{
		HRESULT hr;
		IStream* stream = NULL;
		IWICImagingFactory* factory = NULL;
		IWICBitmapEncoder* encoder = NULL;
		IWICBitmapFrameEncode* frame = NULL;

		try {
			hr = CreateStreamOnHGlobal(NULL, TRUE, &stream);
			if (!SUCCEEDED(hr)) {
				throw gcnew Exception(String::Format(L"CreateStreamOnHGlobal failed : 0x{0:X}", hr));
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

			HGLOBAL hg;
			hr = GetHGlobalFromStream(stream, &hg);
			if (!SUCCEEDED(hr))
			{
				throw gcnew Exception(String::Format(L"GetHGlobalFromStream failed : 0x{0:X}", hr));
			}

			STATSTG stat;
			hr = stream->Stat(&stat, STATFLAG_NONAME);
			if (!SUCCEEDED(hr))
			{
				throw gcnew Exception(String::Format(L"IStream::Stat failed : 0x{0:X}", hr));
			}

			int size = int(stat.cbSize.QuadPart);
			array<Byte, 1>^ ret = gcnew array<Byte, 1>(size);

			LARGE_INTEGER l;
			l.QuadPart = 0;
			hr = stream->Seek(l, STREAM_SEEK_SET, NULL);
			if (!SUCCEEDED(hr))
			{
				throw gcnew Exception(String::Format(L"IStream::Seek failed : 0x{0:X}", hr));
			}

			pin_ptr<Byte> p = &ret[0];

			ULONG read;
			hr = stream->Read(p, size, &read);
			if (!SUCCEEDED(hr))
			{
				throw gcnew Exception(String::Format(L"IStream::Read failed : 0x{0:X}", hr));
			}
			return ret;
		}
		finally {
			if (stream) stream->Release();
			if (frame) frame->Release();
			if (encoder) encoder->Release();
			if (factory) factory->Release();
		}
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
				throw gcnew Exception(String::Format(L"SHCreateStreamOnFileEx failed : 0x{0:X}", hr));
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

	Bitmap32^ Bitmap32::Load(array<Byte, 1>^ content)
	{
		HRESULT hr;
		IStream* stream = NULL;
		IWICImagingFactory* factory = NULL;
		IWICBitmapDecoder* decoder = NULL;
		IWICBitmapFrameDecode* frame = NULL;
		IWICBitmapSource* source = NULL;

		try {
			hr = CreateStreamOnHGlobal(NULL, TRUE, &stream);
			if (!SUCCEEDED(hr)) {
				throw gcnew Exception(String::Format(L"CreateStreamOnHGlobal failed : 0x{0:X}", hr));
			}

			pin_ptr<Byte> p = &content[0];
			ULONG cwritten;
			hr = stream->Write(p, content->Length, &cwritten);
			if (!SUCCEEDED(hr)) {
				throw gcnew Exception(String::Format(L"Stream::Write failed : 0x{0:X}", hr));
			}

			hr = stream->Seek(LARGE_INTEGER(), SEEK_SET, NULL);
			if (!SUCCEEDED(hr)) {
				throw gcnew Exception(String::Format(L"Stream::Seek failed : 0x{0:X}", hr));
			}

			hr = CoCreateInstance(CLSID_WICImagingFactory1, NULL, CLSCTX_INPROC_SERVER,
				IID_IWICImagingFactory, (LPVOID*)&factory);
			if (!SUCCEEDED(hr)) {
				throw gcnew Exception(String::Format(L"CoCreateInstance failed : 0x{0:X}", hr));
			}

			hr = factory->CreateDecoder(GUID_ContainerFormatPng, NULL, &decoder);
			if (!SUCCEEDED(hr)) {
				throw gcnew Exception(String::Format(L"IWICImagingFactory::CreateDecoder failed : 0x{0:X}", hr));
			}

			hr = decoder->Initialize(stream, WICDecodeMetadataCacheOnDemand);
			if (!SUCCEEDED(hr)) {
				throw gcnew Exception(String::Format(L"IWICBitmapDecoder::Initialize failed : 0x{0:X}", hr));
			}

			hr = decoder->GetFrame(0, &frame);
			if (!SUCCEEDED(hr)) {
				throw gcnew Exception(String::Format(L"IWICBitmapDecoder::GetFrame failed : 0x{0:X}", hr));
			}

			UINT width;
			UINT height;
			WICPixelFormatGUID format;

			hr = frame->GetSize(&width, &height);
			if (!SUCCEEDED(hr)) {
				throw gcnew Exception(String::Format(L"IWICBitmapFrameDecode::GetSize failed : 0x{0:X}", hr));
			}

			hr = frame->GetPixelFormat(&format);
			if (!SUCCEEDED(hr)) {
				throw gcnew Exception(String::Format(L"IWICBitmapFrameDecode::GetSize failed : 0x{0:X}", hr));
			}

			hr = frame->QueryInterface<IWICBitmapSource>(&source);
			if (!SUCCEEDED(hr)) {
				throw gcnew Exception(String::Format(L"IWICBitmapFrameDecode::QueryInterface(IWICBitmapSource) failed : 0x{0:X}", hr));
			}

			if (format != GUID_WICPixelFormat32bppBGRA)
			{
				IWICFormatConverter* converter = NULL;

				try {

					hr = factory->CreateFormatConverter(&converter);
					if (!SUCCEEDED(hr)) {
						throw gcnew Exception(String::Format(L"IWICImagingFactory::CreateConverter failed : 0x{0:X}", hr));
					}

					BOOL canConvert;
					hr = converter->CanConvert(format, GUID_WICPixelFormat32bppBGRA, &canConvert);
					if (!SUCCEEDED(hr)) {
						throw gcnew Exception(String::Format(L"IWICFormatConverter::CanConvert failed : 0x{0:X}", hr));
					}

					if (canConvert)
					{
						hr = converter->Initialize(
							source, GUID_WICPixelFormat32bppBGRA,
							WICBitmapDitherTypeNone, NULL, 1,
							WICBitmapPaletteTypeCustom
							);
						if (!SUCCEEDED(hr)) {
							throw gcnew Exception(String::Format(L"IWICFormatConverter::Initialize failed : 0x{0:X}", hr));
						}

						source->Release();
						source = NULL;

						hr = converter->QueryInterface<IWICBitmapSource>(&source);
						if (!SUCCEEDED(hr)) {
							throw gcnew Exception(String::Format(L"IWICFormatConverter::QueryInterface(IWICBitmapSource) failed : 0x{0:X}", hr));
						}
					}
					else {
						throw gcnew Exception(String::Format(L"Unsupported bitmap fomat", hr));
					}

				} finally
				{
					if (converter) converter->Release();
				}
			}

			Bitmap32^ ret = gcnew Bitmap32(width, height);
			Buffer32^ buffer = ret->m_buffer;

			for (int y = 0; y < buffer->Height; y++)
			{
				WICRect r;
				r.X = 0;
				r.Y = y;
				r.Width = buffer->Width;
				r.Height = 1;

				source->CopyPixels(&r, buffer->Stride, buffer->Stride, (BYTE*)ret->m_bitmap->m_rbuf.row(y).ptr);
			}

			return ret;
		}
		finally {
			if (source) source->Release();
			if (frame) frame->Release();
			if (decoder) decoder->Release();
			if (factory) factory->Release();
			if (stream) stream->Release();
		}
	}
}