using System;
using SFML.Graphics;
using SFML.Window;
using SFML.System;

class Program
{
    static void Main()
    {
        var window = new RenderWindow(new VideoMode(800, 600), "SFML Lines and Shapes");
        window.SetVerticalSyncEnabled(true);
        window.Closed += (sender, e) => window.Close();

        // Обычная линия
        Vertex[] line = new Vertex[]
        {
            new Vertex(new Vector2f(50, 50), Color.Red),
            new Vertex(new Vector2f(200, 50), Color.Red)
        };

        // Пунктирная линия
        VertexArray dashedLine = new VertexArray(PrimitiveType.Lines);
        for (float x = 50; x < 200; x += 15)
        {
            dashedLine.Append(new Vertex(new Vector2f(x, 100), Color.Green));
            dashedLine.Append(new Vertex(new Vector2f(x + 10, 100), Color.Green));
        }

        // Ломаная линия
        VertexArray polyline = new VertexArray(PrimitiveType.LineStrip);
        polyline.Append(new Vertex(new Vector2f(50, 150), Color.Blue));
        polyline.Append(new Vertex(new Vector2f(100, 200), Color.Blue));
        polyline.Append(new Vertex(new Vector2f(150, 130), Color.Blue));
        polyline.Append(new Vertex(new Vector2f(200, 170), Color.Blue));

        // Толстая линия
        RectangleShape thickLine = new RectangleShape(new Vector2f(150, 5));
        thickLine.Position = new Vector2f(50, 250);
        thickLine.FillColor = Color.Yellow;
        thickLine.Rotation = 30; // Наклон линии

        // Штрих-пунктирная линия 
        VertexArray dotDashLine = new VertexArray(PrimitiveType.Lines);
        for (float x = 50; x < 200; x += 20)
        {
            // Длинный штрих
            dotDashLine.Append(new Vertex(new Vector2f(x, 300), Color.Magenta));
            dotDashLine.Append(new Vertex(new Vector2f(x + 10, 300), Color.Magenta));
            // Точка
            dotDashLine.Append(new Vertex(new Vector2f(x + 15, 300), Color.Magenta));
            dotDashLine.Append(new Vertex(new Vector2f(x + 15, 300), Color.Magenta)); 
        }
        
        var circle = new CircleShape(50f)
        {
            FillColor = Color.Green,
            Position = new Vector2f(100, 100)
        };

        var rectangle = new RectangleShape(new Vector2f(120, 80))
        {
            FillColor = Color.Red,
            Position = new Vector2f(300, 200)
        };

        var triangle = new ConvexShape(3)
        {
            FillColor = Color.Blue,
            Position = new Vector2f(500, 300)
        };
        triangle.SetPoint(0, new Vector2f(0, 0));
        triangle.SetPoint(1, new Vector2f(100, 0));
        triangle.SetPoint(2, new Vector2f(50, 100));

        // Основной цикл
        while (window.IsOpen)
        {
            window.DispatchEvents();
            window.Clear(Color.Black);

            // Рисуем линии
            window.Draw(line, PrimitiveType.Lines);
            window.Draw(dashedLine);
            window.Draw(polyline);
            window.Draw(thickLine);
            window.Draw(dotDashLine);
            window.Draw(triangle);
            window.Draw(rectangle);
            window.Draw(circle);

            window.Display();
        }
    }
}