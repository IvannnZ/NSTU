using OpenTK.Mathematics;
using OpenTK.Windowing.GraphicsLibraryFramework;

public class Camera
{
    public Vector3 Position;
    private float Pitch = 0;
    private float Yaw = -90;

    private float speed = 3.0f;
    private float sensitivity = 0.2f;
    private Vector2 lastMouse;
    private bool firstMouse = true;

    public Camera(Vector3 startPosition)
    {
        Position = startPosition;
    }

    public Vector3 Front => Vector3.Normalize(new Vector3(
        MathF.Cos(MathHelper.DegreesToRadians(Yaw)) * MathF.Cos(MathHelper.DegreesToRadians(Pitch)),
        MathF.Sin(MathHelper.DegreesToRadians(Pitch)),
        MathF.Sin(MathHelper.DegreesToRadians(Yaw)) * MathF.Cos(MathHelper.DegreesToRadians(Pitch))
    ));

    public Vector3 Right => Vector3.Normalize(Vector3.Cross(Front, Vector3.UnitY));
    public Vector3 Up => Vector3.Normalize(Vector3.Cross(Right, Front));

    public void Update(float deltaTime, KeyboardState keyboard, MouseState mouse)
    {
        float velocity = speed * deltaTime;

        if (keyboard.IsKeyDown(Keys.W)) Position += Front * velocity;
        if (keyboard.IsKeyDown(Keys.S)) Position -= Front * velocity;
        if (keyboard.IsKeyDown(Keys.A)) Position -= Right * velocity;
        if (keyboard.IsKeyDown(Keys.D)) Position += Right * velocity;
        if (keyboard.IsKeyDown(Keys.Q)) Position -= Vector3.UnitY * velocity;
        if (keyboard.IsKeyDown(Keys.E)) Position += Vector3.UnitY * velocity;

        if (firstMouse)
        {
            lastMouse = mouse.Position;
            firstMouse = false;
        }

        var offset = mouse.Position - lastMouse;
        lastMouse = mouse.Position;

        Yaw += offset.X * sensitivity;
        Pitch -= offset.Y * sensitivity;
        Pitch = MathHelper.Clamp(Pitch, -89f, 89f);
    }
}