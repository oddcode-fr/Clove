using Clove;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Test
{
    public class GameWindow : AnimationWindow
    {
        private class Ball
        {
            public double x;
            public double y;
            public double vx;
            public double vy;
            public double radius;
            public Color32 color;
        }

        private List<Ball> m_balls;

        public GameWindow()
        {
            var rnd = new Random();
            m_balls = new List<Ball>();
            for (int i = 0; i < 1000; i++)
            {
                m_balls.Add(new Ball()
                {
                    x = rnd.NextDouble() * 500 - 100,
                    y = rnd.NextDouble() * 500 - 100,
                    vx = rnd.NextDouble() * 0.2 - 0.1,
                    vy = rnd.NextDouble() * 0.2 - 0.1,
                    radius = rnd.NextDouble() * 10,
                    color = new Color32(
                        (byte)(rnd.Next(155) + 100),
                        (byte)(rnd.Next(155) + 100),
                        (byte)(rnd.Next(155) + 100),
                        0x80
                    )
                });
            }
        }


        protected override void OnDrawRequest(Bitmap32 buffer, TimeSpan elapsed)
        {
            SetTitle(string.Format("FPS: {0:0}", FramesPerSecond));

            var rd = new BasicRenderer32(buffer);

            rd.Clear(new Color32(0xFF, 0x50, 0x50, 0x80));

            foreach (var ball in m_balls)
            {
                ball.x += ball.vx * elapsed.TotalMilliseconds;
                ball.y += ball.vy * elapsed.TotalMilliseconds;

                if (ball.x < 0 || ball.x >= buffer.Width) ball.vx = -ball.vx;
                if (ball.y < 0 || ball.y >= buffer.Height) ball.vy = -ball.vy;

                rd.FillColor = ball.color;
                rd.SolidRectangle((int)(ball.x - ball.radius), (int)(ball.y - ball.radius), (int)(ball.x + ball.radius), (int)(ball.y + ball.radius));
            }
        }

    }
}
