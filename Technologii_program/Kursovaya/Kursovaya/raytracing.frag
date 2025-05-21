#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

uniform vec3 camPos;
uniform vec3 camForward;
uniform vec3 camRight;
uniform vec3 camUp;

struct Sphere {
    vec3 center;
    float radius;
    vec3 color;
};

#define MAX_SPHERES 10
uniform int sphereCount;
uniform Sphere spheres[MAX_SPHERES];

bool IntersectSphere(vec3 ro, vec3 rd, Sphere s, out float t)
{
    vec3 oc = ro - s.center;
    float b = dot(oc, rd);
    float c = dot(oc, oc) - s.radius * s.radius;
    float h = b * b - c;

    if (h < 0.0) return false;
    h = sqrt(h);
    t = -b - h;
    if (t < 0.001) t = -b + h;
    return t > 0.001;
}

void main()
{
    vec2 uv = TexCoord * 2.0 - 1.0;
    vec3 rayDir = normalize(uv.x * camRight + uv.y * camUp + camForward);
    vec3 rayOrigin = camPos;

    float closestT = 1e20;
    vec3 color = vec3(0.5, 0.7, 1.0); // background

    for (int i = 0; i < sphereCount; ++i)
    {
        float t;
        if (IntersectSphere(rayOrigin, rayDir, spheres[i], t))
        {
            if (t < closestT)
            {
                closestT = t;
                color = spheres[i].color;
            }
        }
    }

    FragColor = vec4(color, 1.0);
}
