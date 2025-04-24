using System;
using System.Collections.Generic;
using SFML.Graphics;
using SFML.Window;
using SFML.System;

class Program
{
    private static RenderWindow window;
    private static Texture texture;
    private static Sprite sprite;
    private static Image image;
    private static Color brushColor = Color.Black;
    private static int brushSize = 5;
    private static bool isDrawing = false;
    private static Vector2i previousPosition;
    private static Font font;
    private static List<Button> buttons = new List<Button>();
    private static RectangleShape toolbarBackground;

    class Button
    {
        public RectangleShape Shape { get; set; }
        public Text Label { get; set; }
        public Action Action { get; set; }
        public bool IsMouseOver { get; set; }

        public Button(Vector2f position, Vector2f size, string text, Font font, Action action)
        {
            Shape = new RectangleShape(size)
            {
                Position = position,
                FillColor = new Color(240, 240, 240),
                OutlineColor = Color.Black,
                OutlineThickness = 1
            };

            Label = new Text(text, font, 14)
            {
                Position = new Vector2f(position.X + 5, position.Y + 5),
                FillColor = Color.Black
            };

            Action = action;
        }

        public bool Contains(Vector2f point)
        {
            return Shape.GetGlobalBounds().Contains(point.X, point.Y);
        }

        public void Draw(RenderWindow target)
        {
            Shape.FillColor = IsMouseOver ? new Color(220, 220, 220) : new Color(240, 240, 240);
            target.Draw(Shape);
            target.Draw(Label);
        }
    }

    static void Main(string[] args)
    {
         
        window = new RenderWindow(new VideoMode(1000, 700), "Графический редактор с кнопками");
        window.SetVerticalSyncEnabled(true);
        
         
        font = new Font("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf");
         
        // font = new Font("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
        
         
        image = new Image(800, 600, Color.White);
        texture = new Texture(image);
        sprite = new Sprite(texture);
        sprite.Position = new Vector2f(200, 50);

         
        toolbarBackground = new RectangleShape(new Vector2f(180, window.Size.Y))
        {
            Position = new Vector2f(0, 0),
            FillColor = new Color(230, 230, 230)
        };

         
        CreateButtons();

         
        window.Closed += (sender, e) => window.Close();
        window.KeyPressed += HandleKeyPress;
        window.MouseButtonPressed += HandleMousePress;
        window.MouseButtonReleased += HandleMouseRelease;
        window.MouseMoved += HandleMouseMove;
        window.MouseLeft += (sender, e) => { foreach (var btn in buttons) btn.IsMouseOver = false; };

         
        while (window.IsOpen)
        {
            window.DispatchEvents();
            window.Clear(Color.White);
            
            // Рисуем интерфейс
            window.Draw(toolbarBackground);
            foreach (var btn in buttons)
            {
                btn.Draw(window);
            }
            
            // Рисуем изображение
            window.Draw(sprite);
            
            // Отображаем информацию о кисти
            var brushInfo = new Text($"Кисть: {brushSize}px", font, 14)
            {
                Position = new Vector2f(10, window.Size.Y - 50),
                FillColor = Color.Black
            };
            window.Draw(brushInfo);
            
            // Отображаем текущий цвет
            var colorRect = new RectangleShape(new Vector2f(30, 30))
            {
                Position = new Vector2f(10, window.Size.Y - 80),
                FillColor = brushColor
            };
            window.Draw(colorRect);
            
            window.Display();
        }
    }

    private static void CreateButtons()
    {
        buttons.Add(new Button(new Vector2f(10, 10), new Vector2f(160, 30), "Открыть", font, OpenImage));
        buttons.Add(new Button(new Vector2f(10, 50), new Vector2f(160, 30), "Сохранить", font, SaveImage));
        // buttons.Add(new Button(new Vector2f(10, 90), new Vector2f(160, 30), "Серый", font, ConvertToGrayscale));
        
         
        buttons.Add(new Button(new Vector2f(10, 140), new Vector2f(160, 30), "Черный", font, () => brushColor = Color.Black));
        buttons.Add(new Button(new Vector2f(10, 180), new Vector2f(160, 30), "Красный", font, () => brushColor = Color.Red));
        buttons.Add(new Button(new Vector2f(10, 220), new Vector2f(160, 30), "Зеленый", font, () => brushColor = Color.Green));
        buttons.Add(new Button(new Vector2f(10, 260), new Vector2f(160, 30), "Синий", font, () => brushColor = Color.Blue));
        buttons.Add(new Button(new Vector2f(10, 300), new Vector2f(160, 30), "Желтый", font, () => brushColor = Color.Yellow));
        
         
        buttons.Add(new Button(new Vector2f(10, 350), new Vector2f(75, 30), "+ Размер", font, () => brushSize = Math.Min(brushSize + 1, 20)));
        buttons.Add(new Button(new Vector2f(95, 350), new Vector2f(75, 30), "- Размер", font, () => brushSize = Math.Max(brushSize - 1, 1)));
        
         
        buttons.Add(new Button(new Vector2f(10, 400), new Vector2f(160, 30), "Очистить", font, ClearImage));
    }

    private static void HandleKeyPress(object sender, KeyEventArgs e)
    {
        var window = (RenderWindow)sender;
        
        if (e.Code == Keyboard.Key.Escape)
            window.Close();
        else if (e.Code == Keyboard.Key.O)
            OpenImage();
        else if (e.Code == Keyboard.Key.S)
            SaveImage();
        else if (e.Code == Keyboard.Key.G)
            ConvertToGrayscale();
        else if (e.Code == Keyboard.Key.Add)
            brushSize = Math.Min(brushSize + 1, 20);
        else if (e.Code == Keyboard.Key.Subtract)
            brushSize = Math.Max(brushSize - 1, 1);
    }

    private static void HandleMousePress(object sender, MouseButtonEventArgs e)
    {
        var mousePos = window.MapPixelToCoords(new Vector2i(e.X, e.Y));
        
         
        foreach (var btn in buttons)
        {
            if (btn.Contains(mousePos))
            {
                btn.Action();
                return;
            }
        }
        
         
        if (e.Button == Mouse.Button.Left && mousePos.X >= 200)
        {
            isDrawing = true;
            previousPosition = new Vector2i((int)(mousePos.X - 200), (int)(mousePos.Y - 50));
            DrawCircle(previousPosition.X, previousPosition.Y);
        }
    }

    private static void HandleMouseRelease(object sender, MouseButtonEventArgs e)
    {
        if (e.Button == Mouse.Button.Left)
        {
            isDrawing = false;
        }
    }

    private static void HandleMouseMove(object sender, MouseMoveEventArgs e)
    {
        var mousePos = window.MapPixelToCoords(new Vector2i(e.X, e.Y));
        
         
        foreach (var btn in buttons)
        {
            btn.IsMouseOver = btn.Contains(mousePos);
        }
        
         
        if (isDrawing && mousePos.X >= 200)
        {
            var currentPos = new Vector2i((int)(mousePos.X - 200), (int)(mousePos.Y - 50));
            DrawLine(previousPosition.X, previousPosition.Y, currentPos.X, currentPos.Y);
            previousPosition = currentPos;
        }
    }

    private static void DrawCircle(int x, int y)
    {
        for (int i = -brushSize; i <= brushSize; i++)
        {
            for (int j = -brushSize; j <= brushSize; j++)
            {
                if (i * i + j * j <= brushSize * brushSize)
                {
                    int px = x + i;
                    int py = y + j;
                    
                    if (px >= 0 && px < (int)image.Size.X && py >= 0 && py < (int)image.Size.Y)
                    {
                        image.SetPixel((uint)px, (uint)py, brushColor);
                    }
                }
            }
        }
        texture.Update(image);
    }

    private static void DrawLine(int x1, int y1, int x2, int y2)
    {
        // Алгоритм Брезенхэма для рисования линии
        int dx = Math.Abs(x2 - x1);
        int dy = Math.Abs(y2 - y1);
        int sx = x1 < x2 ? 1 : -1;
        int sy = y1 < y2 ? 1 : -1;
        int err = dx - dy;

        while (true)
        {
            DrawCircle(x1, y1);
            
            if (x1 == x2 && y1 == y2) break;
            
            int e2 = 2 * err;
            if (e2 > -dy)
            {
                err -= dy;
                x1 += sx;
            }
            if (e2 < dx)
            {
                err += dx;
                y1 += sy;
            }
        }
    }

    private static void OpenImage()
    {
        string path = "/home/ivannz/Programing/NSTU/Technologii_program/Lab10/Lab10/1.png";

        try
        {
            image = new Image(path);
            texture = new Texture(image);
            sprite = new Sprite(texture);
            sprite.Position = new Vector2f(200, 50);
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Ошибка при открытии файла: {ex.Message}");
        }
    }

    private static void SaveImage()
    {
        string path = "/home/ivannz/Programing/NSTU/Technologii_program/Lab10/Lab10/1.png";
        
        try
        {
            image.SaveToFile(path);
            Console.WriteLine("Изображение успешно сохранено.");
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Ошибка при сохранении файла: {ex.Message}");
        }
    }

    private static void ConvertToGrayscale()
    {
        for (uint x = 0; x < image.Size.X; x++)
        {
            for (uint y = 0; y < image.Size.Y; y++)
            {
                Color pixel = image.GetPixel(x, y);
                byte gray = (byte)((pixel.R + pixel.G + pixel.B) / 3);
                image.SetPixel(x, y, new Color(gray, gray, gray));
            }
        }
        texture.Update(image);
    }

    private static void ClearImage()
    {
        image = new Image(800, 600, Color.White);
        texture = new Texture(image);
        sprite = new Sprite(texture);
        sprite.Position = new Vector2f(200, 50);
    }
}