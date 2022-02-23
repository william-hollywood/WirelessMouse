using System.Net.Sockets;
using System.Text.RegularExpressions;

namespace MobileInterface;

public partial class MainPage : ContentPage
{
    public static string Password { get; private set; }

    public static string Address { get; private set; }

    //private TextCell keybordCell;
    
    public MainPage()
    {
        Password = Preferences.Get("password", "Default");
        Address = Preferences.Get("address", "192.168.1.6");
        InitializeComponent();
    }

    private async void EnterPassword(object sender, EventArgs e)
    {
        string entered = await DisplayPromptAsync("Password", "Enter a Password.", placeholder: Password);
        if (entered == null)
            return;
        if (string.IsNullOrWhiteSpace(entered))
        {
            await DisplayAlert("Invalid", "Password cannot be empty or whitespace", "Back");
            return;
        }
        Preferences.Set("password", entered);
        Password = entered;
    }

    private void OpenKeyboard(object sender, EventArgs e)
    {
        //open a keyboard to take input from the user and call the function on each key press

    }

    private async void EnterAddress(object sender, EventArgs e)
    {
        string entered = await DisplayPromptAsync("Address", "Enter an Address. (IPv4)", placeholder: Address, maxLength: 15, keyboard: Keyboard.Numeric);
        if (entered != null && !Regex.Matches(entered, @"(?:[0-9]{1,3}\.){3}[0-9]{1,3}").Any())
        {
            await DisplayAlert("Invalid", $"Address does not match IPv4", "Back");
            return;
        }
        Preferences.Set("address", entered);
        Address = entered;
    }
}

