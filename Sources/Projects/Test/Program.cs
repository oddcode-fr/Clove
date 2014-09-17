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
            var b = new Bitmap32(640, 480);

            for (uint x = 0; x < 5; x++)
            {
                for (uint y = 0; y < 5; y++)
                {
                    var b2 = new Bitmap32(b, x * 30, y * 30, 20, 20);
                    var r = new BasicRenderer32(b2);
                }
            }

            b.Save("out.png");

            var b3 = new Bitmap32(b, 0, 0, 128, 128);

            b3.Save("out2.png");
        }
    }
}
