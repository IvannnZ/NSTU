using System;
using System.Xml;
using SFML.Graphics;
using SFML.System;
using SFML.Window;

namespace FunctionPlotter
{
    class Program
    {
         
        const double X_MIN = 0.1;
        const double X_MAX = 15.0;
        const double STEP = 0.01;
        const double SCALE = 50.0;
        const double Y_PARAM = 2.0;
        const double CHI = 1.0;
        const double Z_PARAM = 1.0;

        private static double perem = 10;

        static void Main(string[] args)
        {
            var window = new RenderWindow(new VideoMode(800, 600), "График функции ψ(x)");
            window.SetVerticalSyncEnabled(true);
            window.Closed += (sender, e) => window.Close();

            // Создаем оси и сетку один раз (они не меняются)
            var axes = CreateAxes(window);
            var grid = CreateGrid(window);

            while (window.IsOpen)
            {
                // Обработка ввода
                bool needUpdate = false;
                if (Keyboard.IsKeyPressed(Keyboard.Key.W)) { perem += 0.1; needUpdate = true; }
                if (Keyboard.IsKeyPressed(Keyboard.Key.S)) { perem -= 0.1; needUpdate = true; }

                window.DispatchEvents();
                window.Clear(Color.White);
                
                // Создаем график заново при изменении параметра
                var graph = CreateFunctionGraph(window);
                
                window.Draw(grid);
                window.Draw(axes);
                window.Draw(graph);
                
                window.Display();
            }
        }

        // Функция для вычисления значения y (ψ) по x
        static double CalculateFunction(double x)
        {
            // Первое слагаемое: |x^(y/x) - ∛(y/χ)|
            double term1 = Math.Abs(Math.Pow(x, Y_PARAM/x) - Math.Pow(Y_PARAM/CHI, 1.0/3.0));
            
            // Второе слагаемое: (y - χ)(cos y - Z/(y - χ))/(1 + (y - χ)^2)
            double denominator = (Y_PARAM - CHI);
            if (Math.Abs(denominator) < 0.0001) return double.NaN;  
            
            double term2 = denominator * (Math.Cos(Y_PARAM) - Z_PARAM/denominator) / 
                          (1 + Math.Pow(denominator, 2));
            double a = perem;
            Console.WriteLine(a);
            return ((term1 + term2) * a);
        }

        static VertexArray CreateAxes(RenderWindow window)
        {
            var axes = new VertexArray(PrimitiveType.Lines);
            float centerX = window.Size.X / 2;
            float centerY = window.Size.Y / 2;

            // Ось X
            axes.Append(new Vertex(new Vector2f(0, centerY), Color.Black));
            axes.Append(new Vertex(new Vector2f(window.Size.X, centerY), Color.Black));

            // Ось Y
            axes.Append(new Vertex(new Vector2f(centerX, window.Size.Y), Color.Black));
            axes.Append(new Vertex(new Vector2f(centerX, 0), Color.Black));

            // Стрелки на осях
            axes.Append(new Vertex(new Vector2f(window.Size.X, centerY), Color.Black));
            axes.Append(new Vertex(new Vector2f(window.Size.X - 10, centerY - 5), Color.Black));
            
            axes.Append(new Vertex(new Vector2f(centerX, 0), Color.Black));
            axes.Append(new Vertex(new Vector2f(centerX - 5, 10), Color.Black));

            return axes;
        }

        static VertexArray CreateGrid(RenderWindow window)
        {
            var grid = new VertexArray(PrimitiveType.Lines);

             
            for (float x = (float)SCALE; x < window.Size.X; x += (float)SCALE)
            {
                grid.Append(new Vertex(new Vector2f(x, 0), new Color(220, 220, 220)));
                grid.Append(new Vertex(new Vector2f(x, window.Size.Y), new Color(220, 220, 220)));
            }

             
            for (float y = (float)SCALE; y < window.Size.Y; y += (float)SCALE)
            {
                grid.Append(new Vertex(new Vector2f(0, y), new Color(220, 220, 220)));
                grid.Append(new Vertex(new Vector2f(window.Size.X, y), new Color(220, 220, 220)));
            }

            return grid;
        }

        static VertexArray CreateFunctionGraph(RenderWindow window)
        {
            var graph = new VertexArray(PrimitiveType.LineStrip);
            float centerX = window.Size.X / 2;
            float centerY = window.Size.Y / 2;

            for (double x = X_MIN; x <= X_MAX; x += STEP)
            {
                double y = CalculateFunction(x);
                
                if (double.IsNaN(y)) continue;  
                
                float screenX = centerX + (float)(x * SCALE);
                float screenY = centerY - (float)(y * SCALE);
                
                 
                if (screenY >= 0 && screenY <= window.Size.Y)
                {
                    graph.Append(new Vertex(new Vector2f(screenX, screenY), Color.Blue));
                }
            }

            return graph;
        }
    }
}