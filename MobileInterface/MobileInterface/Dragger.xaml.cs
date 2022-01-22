using Encoding;
using System.Diagnostics;
using System.Net.Sockets;

namespace MobileInterface
{
	public partial class Dragger : ContentView
	{
		private UdpClient udp;
		private bool first;
		private double lastX;
		private double lastY;

		public Color Color { get; set; } 

		public Dragger()
		{
			InitializeComponent();
			udp = new UdpClient();

			lastX = 0;
			lastY = 0;
			first = true;
			var panGestureRecognizer = new PanGestureRecognizer();
			panGestureRecognizer.PanUpdated += (sender, eventArgs) => OnDrag(sender, eventArgs);
            GestureRecognizers.Add(panGestureRecognizer);

			var singleTapGestureRecognizer = new TapGestureRecognizer();
			singleTapGestureRecognizer.NumberOfTapsRequired = 1;
			singleTapGestureRecognizer.Tapped += (sender, eventArgs) => { Debug.WriteLine("single"); };
			GestureRecognizers.Add(singleTapGestureRecognizer);

			var doubleTapGestureRecognizer = new TapGestureRecognizer();
			doubleTapGestureRecognizer.NumberOfTapsRequired = 2;
			doubleTapGestureRecognizer.Tapped += (sender, eventArgs) => { Debug.WriteLine("double"); };
			GestureRecognizers.Add(doubleTapGestureRecognizer);
		}

		private void OnDrag(object sender, PanUpdatedEventArgs e)
		{
			if (first)
            {
				lastX = e.TotalX;
				lastY = e.TotalY;
				first = false;
			}
			if (e.StatusType != GestureStatus.Completed)
            {
				short dx = 0, dy = 0;

				//need to redo this

				if (Math.Abs(e.TotalX - lastX) > 1)
				{
					dx = (short)(e.TotalX - lastX);
					lastX = e.TotalX;
				}
				if (Math.Abs(e.TotalY - lastY) > 1)
				{
					dy = (short)(e.TotalY - lastY);
					lastY = e.TotalY;
				}
				if (dx != 0 && dy != 0)
				{
					var data = (dx, dy).Encode(MainPage.Password);
					udp.SendAsync(data, data.Length, MainPage.Address, 8192);
				}
			} else
            {
				first = true;
            }
		}
	}
}