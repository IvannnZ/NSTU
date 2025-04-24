using System;
using SFML.Graphics;
using SFML.System;
using SFML.Window;

namespace BoomerangAnimation
{
    class Program
    {
        private static RenderWindow window;
        private static ConvexShape boomerang;
        private static float angle = 0;
        private static float scale = 1.0f;
        private static bool scalingDown = true;
        private static Vector2f position = new Vector2f(400, 300);
        private static Vector2f direction = new Vector2f(1, 1);
        private static float speed = 2.0f;
        private static Clock clock = new Clock();

        static void Main(string[] args)
        {
             
            window = new RenderWindow(new VideoMode(800, 600), "");
            window.SetFramerateLimit(60);
            
             
            window.Closed += (sender, e) => window.Close();
            window.KeyPressed += OnKeyPressed;

             
            boomerang = new ConvexShape(3);
            boomerang.SetPoint(0, new Vector2f(0, -20));
            boomerang.SetPoint(1, new Vector2f(40, 0));
            boomerang.SetPoint(2, new Vector2f(0, 20));
            boomerang.FillColor = new Color(200, 150, 50);  
            boomerang.OutlineThickness = 2;
            boomerang.OutlineColor = Color.Black;
            boomerang.Origin = new Vector2f(20, 0);  

             
            while (window.IsOpen)
            {
                window.DispatchEvents();
                Update();
                Render();
            }
        }

        private static void OnKeyPressed(object sender, KeyEventArgs e)
        {
            if (e.Code == Keyboard.Key.Escape)
                window.Close();
        }

        private static void Update()
        {
             
            angle += 10f;
            boomerang.Rotation = angle;

             
            if (scalingDown)
            {
                scale -= 0.01f;
                if (scale <= 0.5f) scalingDown = false;
            }
            else
            {
                scale += 0.01f;
                if (scale >= 1.0f) scalingDown = true;
            }
            boomerang.Scale = new Vector2f(scale, scale);

             
            position += direction * speed;
            
             
            if (clock.ElapsedTime.AsSeconds() > 1.0f)
            {
                direction = new Vector2f(-direction.X, -direction.Y);
                clock.Restart();
            }

             
            if (position.X < 0) position.X = 0;
            if (position.X > window.Size.X) position.X = window.Size.X;
            if (position.Y < 0) position.Y = 0;
            if (position.Y > window.Size.Y) position.Y = window.Size.Y;

            boomerang.Position = position;
        }

        private static void Render()
        {
            window.Clear(new Color(240, 240, 240));  
            
             
            window.Draw(boomerang);
            
            window.Display();
        }
    }
}