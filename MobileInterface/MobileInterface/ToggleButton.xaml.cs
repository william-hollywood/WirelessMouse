using Encoding;
using System.ComponentModel;
using System.Net.Sockets;

namespace MobileInterface
{
	public partial class ToggleButton : ContentView
	{
		private bool on = false;

		public Color CurrentToggleColor { get; set; }
		public Color ToggleColorOn { get; set; }
		public Color ToggleColorOff { get; set; }

		public ToggleButton()
		{
			InitializeComponent();
			
			var ToggleTapGestureRecognizer = new TapGestureRecognizer();
			ToggleTapGestureRecognizer.NumberOfTapsRequired = 1;
			ToggleTapGestureRecognizer.Tapped += (sender, eventArgs) => SendToggle();
			GestureRecognizers.Add(ToggleTapGestureRecognizer);
		}

		private void SendToggle()
        {
			// Note: recieved backwards on the desktop app
			if (on) // MDown
				Dragger.SendPkt(short.MinValue, short.MaxValue);
			else // MUp
				Dragger.SendPkt(short.MaxValue, short.MinValue);
			on = !on; 
			CurrentToggleColor = on ? ToggleColorOn : ToggleColorOff;
		}
	}
} 