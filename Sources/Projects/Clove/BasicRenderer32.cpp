
#include "BasicRenderer32.h"


namespace Clove
{

	BasicRenderer32::BasicRenderer32(Bitmap32^ bitmap)
	{
		m_bitmap = bitmap;
		m_renderer = new NativeBasicRenderer32(bitmap->m_bitmap);

		m_renderer->m_ren.clear(agg::rgba(0.8, 0.8, 1.0));

		m_renderer->m_renp.line_color(agg::rgba(1.0, 0.5, 0.0));
		m_renderer->m_renp.fill_color(agg::rgba(0.0, 0.5, 1.0));

		m_renderer->m_renp.outlined_ellipse(100, 100, 300, 200);

		m_renderer->m_renp.line(0, 0, 200 * 256, 300 * 256, true);
	}

	BasicRenderer32::~BasicRenderer32()
	{
		delete m_renderer;
	}

}