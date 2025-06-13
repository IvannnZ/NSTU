using System;
using SFML.Graphics;
using SFML.System;
using SFML.Window;

class Program
{
    static void Main()
    {
        int w = 1920;
        int h = 1080;
        int mouseX = w / 2;
        int mouseY = h / 2;
        float mouseSensitivity = 3.0f;
        float speed = 0.1f;
        bool mouseHidden = true;
        bool[] wasdUD = new bool[6];
        Vector3f pos = new Vector3f(-5.0f, 0.0f, 0.0f);
        Clock clock = new Clock();
        int framesStill = 1;

        var window = new RenderWindow(new VideoMode((uint)w, (uint)h), "Ray tracing", Styles.Titlebar | Styles.Close);
        window.SetFramerateLimit(60);
        window.SetMouseCursorVisible(false);
        window.Closed += (sender, e) => window.Close();

        RenderTexture firstTexture = new RenderTexture((uint)w, (uint)h);
        Sprite firstTextureSprite = new Sprite(firstTexture.Texture);
        Sprite firstTextureSpriteFlipped = new Sprite(firstTexture.Texture)
        {
            Scale = new Vector2f(1, -1),
            Position = new Vector2f(0, h)
        };

        RenderTexture outputTexture = new RenderTexture((uint)w, (uint)h);
        Sprite outputTextureSprite = new Sprite(outputTexture.Texture);
        Sprite outputTextureSpriteFlipped = new Sprite(outputTexture.Texture)
        {
            Scale = new Vector2f(1, -1),
            Position = new Vector2f(0, h)
        };
        Shader shader;
        try
        {
            shader = Shader.FromFile(
                fragmentShaderFilename: "OutputShader.frag",
                shaderType: ShaderType.Fragment
            );
        }
        catch (LoadingFailedException ex)
        {
            Console.WriteLine($"Shader loading failed: {ex.Message}");
            return;
        }

        shader.SetUniform("u_resolution", new Vector2f(w, h));
        shader.SetUniform("u_pos", pos);
        Random rand = new Random();

        while (window.IsOpen)
        {
            window.DispatchEvents();
            
            // Обработка ввода
            if (mouseHidden)
            {
                Vector2i mousePos = Mouse.GetPosition(window);
                int mx = mousePos.X - w / 2;
                int my = mousePos.Y - h / 2;
                mouseX += mx;
                mouseY += my;
                Mouse.SetPosition(new Vector2i(w / 2, h / 2), window);
                if (mx != 0 || my != 0) framesStill = 1;
            }

            // Обновление позиции камеры
            if (mouseHidden)
            {
                float mx = ((float)mouseX / w - 0.5f) * mouseSensitivity;
                float my = ((float)mouseY / h - 0.5f) * mouseSensitivity;
                
                Vector3f dir = new Vector3f();
                if (wasdUD[0]) dir.X = 1.0f;
                else if (wasdUD[2]) dir.X = -1.0f;
                
                if (wasdUD[1]) dir.Y = -1.0f;
                else if (wasdUD[3]) dir.Y = 1.0f;

                // Вращение направления
                Vector3f dirTemp = new Vector3f();
                dirTemp.Z = dir.Z * (float)Math.Cos(-my) - dir.X * (float)Math.Sin(-my);
                dirTemp.X = dir.Z * (float)Math.Sin(-my) + dir.X * (float)Math.Cos(-my);
                dirTemp.Y = dir.Y;
                
                dir.X = dirTemp.X * (float)Math.Cos(mx) - dirTemp.Y * (float)Math.Sin(mx);
                dir.Y = dirTemp.X * (float)Math.Sin(mx) + dirTemp.Y * (float)Math.Cos(mx);
                dir.Z = dirTemp.Z;

                pos += dir * speed;
                
                if (wasdUD[4]) pos.Z -= speed;
                else if (wasdUD[5]) pos.Z += speed;

                // Обновление uniform-переменных
                shader.SetUniform("u_pos", pos);
                shader.SetUniform("u_mouse", new Vector2f(mx, my));
                shader.SetUniform("u_time", clock.ElapsedTime.AsSeconds());
                shader.SetUniform("u_sample_part", 1.0f / framesStill);
                shader.SetUniform("u_seed1", new Vector2f((float)rand.NextDouble(), (float)rand.NextDouble()) * 999.0f);
                shader.SetUniform("u_seed2", new Vector2f((float)rand.NextDouble(), (float)rand.NextDouble()) * 999.0f);
            }

            // Рендеринг
            window.Clear();
            if (framesStill % 2 == 1)
            {
                shader.SetUniform("u_sample", firstTexture.Texture);
                outputTexture.Draw(outputTextureSpriteFlipped, new RenderStates(shader));
                window.Draw(outputTextureSprite);
            }
            else
            {
                shader.SetUniform("u_sample", outputTexture.Texture);
                firstTexture.Draw(firstTextureSpriteFlipped, new RenderStates(shader));
                window.Draw(firstTextureSprite);
            }
            
            window.Display();
            framesStill++;
        }
    }
}