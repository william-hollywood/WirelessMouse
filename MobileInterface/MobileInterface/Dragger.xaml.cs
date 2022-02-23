using Encoding;
using System.Net.Sockets;

namespace MobileInterface
{
	public partial class Dragger : ContentView
	{
		private static UdpClient UDP;
		private bool first;

		public Color Color { get; set; }

		public Dragger()
		{
			InitializeComponent();
			UDP = new UdpClient();

			first = true;
			var panGestureRecognizer = new PanGestureRecognizer();
			panGestureRecognizer.PanUpdated += (sender, eventArgs) => { OnDrag(sender, eventArgs); };
            GestureRecognizers.Add(panGestureRecognizer);

			var singleTapGestureRecognizer = new TapGestureRecognizer();
			singleTapGestureRecognizer.NumberOfTapsRequired = 1;
			singleTapGestureRecognizer.Tapped += (sender, eventArgs) => { SendPkt(short.MaxValue, short.MaxValue); };
			GestureRecognizers.Add(singleTapGestureRecognizer);

			var doubleTapGestureRecognizer = new TapGestureRecognizer();
			doubleTapGestureRecognizer.NumberOfTapsRequired = 2;
			doubleTapGestureRecognizer.Tapped += (sender, eventArgs) => { SendPkt(short.MinValue, short.MinValue); };
			GestureRecognizers.Add(doubleTapGestureRecognizer);
		}

		private void OnDrag(object sender, PanUpdatedEventArgs e)
		{
			if (first)
            {
				SendPkt(0, 0);
				first = false;
			}
			switch (e.StatusType)
            {
                case GestureStatus.Running:
					SendPkt((short) e.TotalX, (short) e.TotalY);
                    break;
                case GestureStatus.Completed:
					first = true;
					break;
				default:
					break;
			}
		}

        public static void SendPkt(short x, short y)
        {
            var data = (x, y).Encode(MainPage.Password);
            UDP.SendAsync(data, data.Length, MainPage.Address, 8192);
        }
    }
}