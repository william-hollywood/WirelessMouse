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

	private async void EnterPassword(object sender, EventArgs e)
    {
		string pass = await DisplayPromptAsync("Password", "Enter a Password.", placeholder: Password);
    }

	private void EnterAddress(object sender, EventArgs e)
    {
		//display text entry
	}
}

