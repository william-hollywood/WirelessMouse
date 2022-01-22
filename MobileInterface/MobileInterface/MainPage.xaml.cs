using System.Net.Sockets;

namespace MobileInterface;

public partial class MainPage : ContentPage
{

	public static string Password = "Default";

	public static string Address = "192.168.1.6";
	public MainPage()
	{
		InitializeComponent();
	}

	private void EnterPassword(object sender, EventArgs e)
    {
		//display text entry
    }

	private void EnterAddress(object sender, EventArgs e)
    {
		//display text entry
	}
}

