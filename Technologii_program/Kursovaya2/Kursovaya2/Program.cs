using OpenTK.Graphics.OpenGL4;
using OpenTK.Windowing.Common;
using OpenTK.Windowing.Desktop;
using OpenTK.Windowing.GraphicsLibraryFramework;
using OpenTK.Mathematics;
using System;
using System.IO;

class RayTracerApp : GameWindow
{
     private Camera _camera;
    private ShaderProgram _rayTracerShader = null!; // Явное указание на nullable
    private int _outputTexture;
    private int _ssbo;
    private int _vao;
    private double _time;
    
    public RayTracerApp() : base(GameWindowSettings.Default, 
        new NativeWindowSettings()
        {
            ClientSize = new Vector2i(1280, 720), // Исправлено с Size на ClientSize
            APIVersion = new Version(4, 3)
        })
    {
        _camera = new Camera(new Vector3(0, 0, 3), ClientSize.X, ClientSize.Y);
    }
    protected override void OnLoad()
    {
        base.OnLoad();
        InitializeShaders();
        InitializeTexture();
        InitializeScene();
        InitializeQuad();
    }

    

    private void InitializeShaders()
    {
        _rayTracerShader = new ShaderProgram(
            LoadShader("Shaders/raytracer.comp", ShaderType.ComputeShader)
        );
    }

    private void InitializeTexture()
    {
        _outputTexture = GL.GenTexture();
        GL.ActiveTexture(TextureUnit.Texture0);
        GL.BindTexture(TextureTarget.Texture2D, _outputTexture);
        GL.TexImage2D(TextureTarget.Texture2D, 0, PixelInternalFormat.Rgba32f, 
            ClientSize.X, ClientSize.Y, 0, PixelFormat.Rgba, PixelType.Float, IntPtr.Zero);
        GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMinFilter, (int)TextureMinFilter.Linear);
        GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMagFilter, (int)TextureMagFilter.Linear);
    }

    private void InitializeQuad()
    {
        _vao = GL.GenVertexArray();
        GL.BindVertexArray(_vao);
        
        float[] quadVertices = {
            -1.0f, -1.0f, 0.0f, 0.0f,
             1.0f, -1.0f, 1.0f, 0.0f,
            -1.0f,  1.0f, 0.0f, 1.0f,
             1.0f,  1.0f, 1.0f, 1.0f
        };

        var vbo = GL.GenBuffer();
        GL.BindBuffer(BufferTarget.ArrayBuffer, vbo);
        GL.BufferData(BufferTarget.ArrayBuffer, quadVertices.Length * sizeof(float), 
            quadVertices, BufferUsageHint.StaticDraw); // Исправлено BufferUsage на BufferUsageHint
        
        GL.VertexAttribPointer(0, 2, VertexAttribPointerType.Float, false, 4 * sizeof(float), 0);
        GL.EnableVertexAttribArray(0);
        GL.VertexAttribPointer(1, 2, VertexAttribPointerType.Float, false, 4 * sizeof(float), 2 * sizeof(float));
        GL.EnableVertexAttribArray(1);
    }

    private void InitializeScene()
    {
        var spheres = new Sphere[]
        {
            new Sphere(new Vector3(0, 0, 0), 0.5f),
            new Sphere(new Vector3(1, 0, 0), 0.3f)
        };

        _ssbo = GL.GenBuffer();
        GL.BindBuffer(BufferTarget.ShaderStorageBuffer, _ssbo);
        GL.BufferData(BufferTarget.ShaderStorageBuffer, spheres.Length * Sphere.SizeInBytes, 
            spheres, BufferUsageHint.StaticDraw); // Исправлено BufferUsage на BufferUsageHint
        GL.BindBufferBase(BufferRangeTarget.ShaderStorageBuffer, 0, _ssbo);
    }

    protected override void OnRenderFrame(FrameEventArgs args)
    {
        base.OnRenderFrame(args);
        _time += args.Time;
        
        GL.BindImageTexture(0, _outputTexture, 0, false, 0, TextureAccess.WriteOnly, SizedInternalFormat.Rgba32f);
        
        _rayTracerShader.Use();
        GL.Uniform3(_rayTracerShader.GetUniformLocation("cameraPos"), _camera.Position);
        
        // Исправление для передачи матрицы
        var viewProjMatrix = _camera.ViewProjectionMatrix;
        GL.UniformMatrix4(_rayTracerShader.GetUniformLocation("viewProjMatrix"), false, ref viewProjMatrix);
        
        GL.Uniform1(_rayTracerShader.GetUniformLocation("time"), (float)_time);
        
        GL.DispatchCompute((ClientSize.X + 15) / 16, (ClientSize.Y + 15) / 16, 1);
        GL.MemoryBarrier(MemoryBarrierFlags.ShaderImageAccessBarrierBit);

        GL.Clear(ClearBufferMask.ColorBufferBit);
        GL.BindVertexArray(_vao);
        GL.ActiveTexture(TextureUnit.Texture0);
        GL.BindTexture(TextureTarget.Texture2D, _outputTexture);
        GL.DrawArrays(PrimitiveType.TriangleStrip, 0, 4);
        
        SwapBuffers();
    }
    
protected override void OnResize(ResizeEventArgs e)
{
    base.OnResize(e);
    GL.Viewport(0, 0, ClientSize.X, ClientSize.Y);
    _camera.UpdateAspectRatio(ClientSize.X, ClientSize.Y);
    
    // Реинициализация текстуры при изменении размера окна
    GL.BindTexture(TextureTarget.Texture2D, _outputTexture);
    GL.TexImage2D(TextureTarget.Texture2D, 0, PixelInternalFormat.Rgba32f, 
        ClientSize.X, ClientSize.Y, 0, PixelFormat.Rgba, PixelType.Float, IntPtr.Zero);
}

    private int LoadShader(string path, ShaderType type)
    {
        var shader = GL.CreateShader(type);
        var source = File.ReadAllText(path);
        GL.ShaderSource(shader, source);
        GL.CompileShader(shader);

        GL.GetShader(shader, ShaderParameter.CompileStatus, out int status);
        if (status == 0)
        {
            var info = GL.GetShaderInfoLog(shader);
            throw new Exception($"Shader compilation failed: {info}");
        }
        return shader;
    }

    protected override void OnUpdateFrame(FrameEventArgs args)
    {
        base.OnUpdateFrame(args);
        var input = KeyboardState;
        var mouse = MouseState;
        
        _camera.Update(input, mouse, (float)args.Time);
        
        if (input.IsKeyDown(Keys.Escape))
            Close();
    }

    static void Main(string[] args)
    {
        using var app = new RayTracerApp();
        app.Run();
    }
}

class ShaderProgram : IDisposable
{
    private readonly int _program;
    
    public ShaderProgram(int ComputeShader)
    {
        _program = GL.CreateProgram();
        GL.AttachShader(_program, ComputeShader);
        GL.LinkProgram(_program);
        
        GL.GetProgram(_program, GetProgramParameterName.LinkStatus, out int status);
        if (status == 0)
        {
            var info = GL.GetProgramInfoLog(_program);
            throw new Exception($"Program linking failed: {info}");
        }
    }

    public void Use() => GL.UseProgram(_program);
    
    public int GetUniformLocation(string name) => GL.GetUniformLocation(_program, name);
    
    public void Dispose()
    {
        GL.DeleteProgram(_program);
        GC.SuppressFinalize(this);
    }
}

class Camera
{
    public Vector3 Position;
    private Vector3 _front = -Vector3.UnitZ;
    private Vector3 _up = Vector3.UnitY;
    private float _fov;
    
    private float _yaw = -90.0f;
    private float _pitch;
    private float _aspectRatio;

    public Matrix4 ViewProjectionMatrix => GetViewMatrix() * GetProjectionMatrix();

    public Camera(Vector3 position, int width, int height)
    {
        Position = position;
        _aspectRatio = (float)width / height;
        _fov = 45.0f;
    }
    
    public void UpdateAspectRatio(int width, int height)
{
    _aspectRatio = (float)width / height;
}

private Matrix4 GetProjectionMatrix() => 
    Matrix4.CreatePerspectiveFieldOfView(
        MathHelper.DegreesToRadians(_fov),
        _aspectRatio, 
        0.1f, 
        100.0f);

    public void Update(KeyboardState input, MouseState mouse, float deltaTime)
    {
        const float cameraSpeed = 2.5f;
        float actualSpeed = cameraSpeed * deltaTime;

        if (input.IsKeyDown(Keys.W)) Position += _front * actualSpeed;
        if (input.IsKeyDown(Keys.S)) Position -= _front * actualSpeed;
        if (input.IsKeyDown(Keys.A)) Position -= Vector3.Normalize(Vector3.Cross(_front, _up)) * actualSpeed;
        if (input.IsKeyDown(Keys.D)) Position += Vector3.Normalize(Vector3.Cross(_front, _up)) * actualSpeed;

        if (mouse.IsButtonDown(MouseButton.Button1))
        {
            _yaw += mouse.Delta.X * 0.1f;
            _pitch -= mouse.Delta.Y * 0.1f;
        }

        _pitch = Math.Clamp(_pitch, -89.0f, 89.0f);

        _front = new Vector3(
            MathF.Cos(MathHelper.DegreesToRadians(_pitch)) *
            MathF.Cos(MathHelper.DegreesToRadians(_yaw)),
            MathF.Sin(MathHelper.DegreesToRadians(_pitch)),
            MathF.Cos(MathHelper.DegreesToRadians(_pitch)) *
            MathF.Sin(MathHelper.DegreesToRadians(_yaw))
        ).Normalized();
    }

    private Matrix4 GetViewMatrix() => 
        Matrix4.LookAt(Position, Position + _front, _up);

    private Matrix4 GetProjectionMatrix() => 
        Matrix4.CreatePerspectiveFieldOfView(
            MathHelper.DegreesToRadians(_fov),
            _aspectRatio, 0.1f, 100.0f);
}

struct Sphere
{
    public Vector3 Position;
    public float Radius;
    
    public const int SizeInBytes = 16;

    public Sphere(Vector3 position, float radius)
    {
        Position = position;
        Radius = radius;
    }
}