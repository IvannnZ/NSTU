using Gtk;
using System;

class Program
{
    static void Main()
    {
        Application.Init();
        
        var win = new Window("Обработка строк");
        win.SetDefaultSize(400, 300);
        win.DeleteEvent += (sender, e) => Application.Quit();
        
         
        var listStore = new ListStore(typeof(string));
        listStore.AppendValues("hello world");
        listStore.AppendValues("test string");
        
         
        var treeView = new TreeView(listStore);
        treeView.AppendColumn("Текст", new CellRendererText(), "text", 0);
        
         
        var btnProcess = new Button("Обработать");
        var btnAdd = new Button("Добавить строку");
        var label = new Label("Результат:");
        var entry = new Entry();
        
         
        btnProcess.Clicked += (sender, e) => 
        {
            if (treeView.Selection.GetSelected(out TreeIter iter))
            {
                var text = (string)listStore.GetValue(iter, 0);
                label.Text = CapitalizeFirstLetters(text);
            }
        };
        
         
        btnAdd.Clicked += (sender, e) => 
        {
            if (!string.IsNullOrWhiteSpace(entry.Text))
            {
                listStore.AppendValues(entry.Text);
                entry.Text = "";
            }
        };
        
         
        var inputBox = new HBox();
        inputBox.PackStart(entry, true, true, 0);
        inputBox.PackStart(btnAdd, false, true, 5);
        
        var buttonBox = new HBox();
        buttonBox.PackStart(btnProcess, false, true, 0);
        
        var vbox = new VBox();
        vbox.PackStart(treeView, true, true, 0);
        vbox.PackStart(inputBox, false, true, 5);
        vbox.PackStart(buttonBox, false, true, 5);
        vbox.PackStart(label, false, true, 5);
        
        win.Add(vbox);
        win.ShowAll();
        Application.Run();
    }
    
    static string CapitalizeFirstLetters(string input)
    {
        if (string.IsNullOrEmpty(input))
            return input;

        char[] chars = input.ToCharArray();
        bool newWord = true;

        for (int i = 0; i < chars.Length; i++)
        {
            if (newWord && char.IsLetter(chars[i]))
            {
                chars[i] = char.ToUpper(chars[i]);
                newWord = false;
            }
            else if (chars[i] == ' ')
            {
                newWord = true;
            }
        }

        return new string(chars);
    }
}
// using Gtk;
// using System;
//
// class Program
// {
//     static void Main()
//     {
//         Application.Init();
//         
//         var win = new Window("Обработка строк");
//         win.SetDefaultSize(400, 300);
//         
//         var listStore = new ListStore(typeof(string));
//         listStore.AppendValues("hello world");
//         listStore.AppendValues("test string");
//         
//         var treeView = new TreeView(listStore);
//         treeView.AppendColumn("Текст", new CellRendererText(), "text", 0);
//         
//         var btn = new Button("Обработать");
//         var label = new Label("Результат:");
//         
//         btn.Clicked += (sender, e) => 
//         {
//             if (treeView.Selection.GetSelected(out TreeIter iter))
//             {
//                 var text = (string)listStore.GetValue(iter, 0);
//                 label.Text = text.ToUpper(); // Ваш метод обработки
//             }
//         };
//         
//         var vbox = new VBox();
//         vbox.PackStart(treeView, true, true, 0);
//         vbox.PackStart(btn, false, true, 0);
//         vbox.PackStart(label, false, true, 0);
//         
//         win.Add(vbox);
//         win.ShowAll();
//         Application.Run();
//     }
// }