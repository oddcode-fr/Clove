#ifndef __RENDERER32_H
#define __RENDERER32_H

#include "Agg\agg_rendering_buffer.h"
#include "Agg\agg_pixfmt_rgba.h"
#include "Agg\agg_renderer_base.h"
#include "Agg\agg_renderer_primitives.h"

#include "Bitmap32.h"

namespace Clove
{
	struct NativeBasicRenderer32
	{
		typedef agg::renderer_base<agg::pixfmt_bgra32> renderer_type;

		renderer_type m_ren;
		agg::renderer_primitives<renderer_type> m_renp;

		NativeBasicRenderer32(NativeBitmap32* bitmap) :
			m_ren(bitmap->m_pixf),
			m_renp(m_ren)
		{

		}
	};

	public ref class BasicRenderer32
	{
	private:
		NativeBasicRenderer32* m_renderer;
		Bitmap32^ m_bitmap;

	public:
		BasicRenderer32(Bitmap32^ bitmap);
		~BasicRenderer32();

		property UInt32 Width { UInt32 get() { return m_renderer->m_ren.width(); } }
		property UInt32 Height { UInt32 get() { return m_renderer->m_ren.height(); } }

	};
}

#endif