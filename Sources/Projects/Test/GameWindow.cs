using Clove;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Test
{
    public class GameWindow : AnimationWindow
    {
        private class Particle
        {
            public double x;
            public double y;
            public double vx;
            public double vy;
            public double radius;
            public Color32 color;
        }

        private List<Particle> m_particles = new List<Particle>();

        private void AddRandomParticles(int count, double? x = null, double? y = null)
        {
            var rnd = new Random();

            for (int i = 0; i < count; i++)
            {
                m_particles.Add(new Particle()
                {
                    x = x.HasValue ? x.Value : rnd.NextDouble() * 500,
                    y = y.HasValue ? y.Value : rnd.NextDouble() * 500,
                    vx = rnd.NextDouble() * 0.2 - 0.1,
                    vy = rnd.NextDouble() * 0.2 - 0.1,
                    radius = rnd.NextDouble() * 5,
                    color = new Color32(
                        (byte)(rnd.Next(155) + 100),
                        (byte)(rnd.Next(155) + 100),
                        (byte)(rnd.Next(155) + 100),
                        0x80
                    )
                });
            }
        }

        public GameWindow() : base(WindowStyle.FixedWithTitle)
        {
            SetSize(600, 600);

            AddRandomParticles(100);
        }

        protected override void OnMouseButtonDown(int x, int y)
        {
            AddRandomParticles(100, x, y);
        }

        protected override void OnDrawRequest(Bitmap32 buffer, TimeSpan elapsed)
        {
            SetTitle(string.Format("FPS: {0:0}, Particles: {1}", FramesPerSecond, m_particles.Count));

            var rd = new BasicRenderer32(buffer);

            rd.Clear(new Color32(0x50, 0x50, 0x50, 0xFF));

            foreach (var particle in m_particles)
            {
                particle.x += particle.vx * elapsed.TotalMilliseconds;
                particle.y += particle.vy * elapsed.TotalMilliseconds;

                if (particle.x < 0 || particle.x >= buffer.Width) particle.vx = -particle.vx * 0.8;
                if (particle.y < 0 || particle.y >= buffer.Height) particle.vy = -particle.vy * 0.8;

                rd.FillColor = particle.color;
                rd.SolidElipse((int)particle.x, (int)particle.y, (int)particle.radius, (int)particle.radius);
                //rd.SolidRectangle((int)(particle.x - particle.radius), (int)(particle.y - particle.radius), (int)(particle.x + particle.radius), (int)(particle.y + particle.radius));
            }
        }

    }
}
