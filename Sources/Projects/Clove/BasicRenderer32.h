#ifndef __RENDERER32_H
#define __RENDERER32_H

#include "Agg\agg_rendering_buffer.h"
#include "Agg\agg_pixfmt_rgba.h"
#include "Agg\agg_renderer_base.h"
#include "Agg\agg_renderer_primitives.h"

#include "Bitmap32.h"
#include "Color.h"

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

		property Color32 FillColor {
			Color32 get() { return Color32(m_renderer->m_renp.fill_color()); }
			void set(Color32 value) { m_renderer->m_renp.fill_color(value.ToAggColor()); }
		}

		property Color32 LineColor {
			Color32 get() { return Color32(m_renderer->m_renp.line_color()); }
			void set(Color32 value) { m_renderer->m_renp.line_color(value.ToAggColor()); }
		}

		void Elipse(int x, int y, int rx, int ry) { m_renderer->m_renp.ellipse(x, y, rx, ry); }
		void SolidElipse(int x, int y, int rx, int ry) { m_renderer->m_renp.solid_ellipse(x, y, rx, ry); }
		void OutlinedElipse(int x, int y, int rx, int ry) { m_renderer->m_renp.outlined_ellipse(x, y, rx, ry); }

		void Rectangle(int x1, int y1, int x2, int y2) { m_renderer->m_renp.rectangle(x1, y1, x2, y2); }
		void SolidRectangle(int x1, int y1, int x2, int y2) { m_renderer->m_renp.solid_rectangle(x1, y1, x2, y2); }
		void OutlinedRectangle(int x1, int y1, int x2, int y2) { m_renderer->m_renp.outlined_rectangle(x1, y1, x2, y2); }

		void Line(int x1, int y1, int x2, int y2) { m_renderer->m_renp.line(x1 << 8, y1 << 8, x2 << 8, y2 << 8, true); }

		void Clip(int x1, int y1, int x2, int y2) { m_renderer->m_ren.clip_box(x1, y1, x2, y2); }
		void Clip() { m_renderer->m_ren.reset_clipping(true); }

		void Clear(Color32 c) { m_renderer->m_ren.clear(c.ToAggColor()); }
	};
}

#endif