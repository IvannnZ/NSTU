using OpenTK.Graphics.OpenGL4;

public class Shader
{
    public int Handle;

    public Shader(string vertexPath, string fragmentPath)
    {
        var vertexSource = File.ReadAllText(vertexPath);
        var fragmentSource = File.ReadAllText(fragmentPath);

        int vertexShader = GL.CreateShader(ShaderType.VertexShader);
        GL.ShaderSource(vertexShader, vertexSource);
        GL.CompileShader(vertexShader);
        CheckCompile(vertexShader);

        int fragmentShader = GL.CreateShader(ShaderType.FragmentShader);
        GL.ShaderSource(fragmentShader, fragmentSource);
        GL.CompileShader(fragmentShader);
        CheckCompile(fragmentShader);

        Handle = GL.CreateProgram();
        GL.AttachShader(Handle, vertexShader);
        GL.AttachShader(Handle, fragmentShader);
        GL.LinkProgram(Handle);

        GL.DetachShader(Handle, vertexShader);
        GL.DetachShader(Handle, fragmentShader);
        GL.DeleteShader(vertexShader);
        GL.DeleteShader(fragmentShader);
    }

    private void CheckCompile(int shader)
    {
        GL.GetShader(shader, ShaderParameter.CompileStatus, out int success);
        if (success == 0)
        {
            string infoLog = GL.GetShaderInfoLog(shader);
            throw new Exception($"Shader compile error: {infoLog}");
        }
    }

    public void Use() => GL.UseProgram(Handle);

    public void Set(string name, int value) => GL.Uniform1(GL.GetUniformLocation(Handle, name), value);
    public void Set(string name, float value) => GL.Uniform1(GL.GetUniformLocation(Handle, name), value);
    public void Set(string name, OpenTK.Mathematics.Vector3 value) =>
        GL.Uniform3(GL.GetUniformLocation(Handle, name), value);
}