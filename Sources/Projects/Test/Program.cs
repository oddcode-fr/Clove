using Clove;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Test
{
    class Program
    {
        [STAThread]
        static void Main(string[] args)
        {
            using (var w = new GameWindow())
            {
                
                w.Open();
            }

            var random = new Random();
            var bm = new Bitmap32(640, 480);
            var rd = new BasicRenderer32(bm);
            
            rd.Clear(new Color32(0x20, 0x20, 0x20, 0xFF));

            rd.FillColor = new Color32(0xFF, 0x00, 0x00, 0x30);
            for (int i = 0; i < 100; i++)
            {
                var r = random.Next(50) + 10;
                var x = random.Next(bm.Width - r * 2) + r;
                var y = random.Next(bm.Height - r * 2) + r;

                rd.SolidElipse(
                    x, y, r, r
                );
            }

            //DisplayWindow.Show(bm);

            bm.Save("out.png");

            new Bitmap32(bm, 0, 0, 128, 128).Save("out2.png");
        }
    }
}
