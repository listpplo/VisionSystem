using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace Samples_VisionPro
{
    static class Program
    {
        /// <summary>
        /// 应用程序的主入口点。
		/// Main entry point for application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
        }
    }
}
