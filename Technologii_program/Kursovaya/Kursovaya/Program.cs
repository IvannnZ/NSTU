using OpenTK.Graphics.OpenGL4;
using OpenTK.Mathematics;
using OpenTK.Windowing.Common;
using OpenTK.Windowing.Desktop;
using OpenTK.Windowing.GraphicsLibraryFramework;

public class Program : GameWindow
{
    private Shader _shader;
    private int _vao;
    private Camera _camera;

    private record struct Sphere(Vector3 Center, float Radius, Vector3 Color);
    private List<Sphere> _spheres = new();

    
    public Program() : base(GameWindowSettings.Default, new NativeWindowSettings
    {
        Title = "Ray Tracing OpenGL",
        Size = new Vector2i(1280, 720),
    })
    {
    }

    protected override void OnLoad()
    {
        base.OnLoad();
        GL.ClearColor(0f, 0f, 0f, 1f);
        CursorGrabbed = true;

        _shader = new Shader("raytracing.vert", "raytracing.frag");

        float[] quad =
        {
            -1f, -1f,
            1f, -1f,
            -1f, 1f,
            1f, 1f
        };

        _vao = GL.GenVertexArray();
        int vbo = GL.GenBuffer();
        GL.BindVertexArray(_vao);
        GL.BindBuffer(BufferTarget.ArrayBuffer, vbo);
        GL.BufferData(BufferTarget.ArrayBuffer, quad.Length * sizeof(float), quad, BufferUsageHint.StaticDraw);
        GL.EnableVertexAttribArray(0);
        GL.VertexAttribPointer(0, 2, VertexAttribPointerType.Float, false, 2 * sizeof(float), 0);

        _camera = new Camera(new Vector3(0, 0, 3));

        // Добавим несколько сфер
        _spheres.Add(new Sphere(new Vector3(0, 0, -5), 1.0f, new Vector3(1, 0, 0)));
        _spheres.Add(new Sphere(new Vector3(2, 0, -6), 0.5f, new Vector3(0, 1, 0)));
        _spheres.Add(new Sphere(new Vector3(-2, 0, -4), 0.75f, new Vector3(0, 0, 1)));
    }

    protected override void OnRenderFrame(FrameEventArgs args)
    {
        base.OnRenderFrame(args);
        GL.Clear(ClearBufferMask.ColorBufferBit);

        _shader.Use();

        _shader.Set("camPos", _camera.Position);
        _shader.Set("camForward", _camera.Front);
        _shader.Set("camRight", _camera.Right);
        _shader.Set("camUp", _camera.Up);

        _shader.Set("sphereCount", _spheres.Count);
        for (int i = 0; i < _spheres.Count; i++)
        {
            var s = _spheres[i];
            _shader.Set($"spheres[{i}].center", s.Center);
            _shader.Set($"spheres[{i}].radius", s.Radius);
            _shader.Set($"spheres[{i}].color", s.Color);
        }

        GL.BindVertexArray(_vao);
        GL.DrawArrays(PrimitiveType.TriangleStrip, 0, 4);
        SwapBuffers();
    }

    protected override void OnUpdateFrame(FrameEventArgs args)
    {
        base.OnUpdateFrame(args);
        _camera.Update((float)args.Time, KeyboardState, MouseState);
    }

    public static void Main() => new Program().Run();
}
