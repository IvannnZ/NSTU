using SkiaSharp;
using SkiaSharp.Views.Desktop;

var window = new SKGLControl();
window.PaintSurface += (sender, e) => {
    e.Surface.Canvas.Clear(SKColors.White);
    e.Surface.Canvas.DrawLine(0, 0, 100, 100, 
        new SKPaint { Color = SKColors.Red, StrokeWidth = 2 });
};
window.Show();
Application.Run();