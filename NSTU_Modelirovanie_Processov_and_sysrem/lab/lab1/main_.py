import matplotlib.pyplot as plt
from math import sqrt

G = 6.67e-7
m, L, l, M, Vx, Vy, dt, t = 10000, 10, -20, 10000, -0.8, -1, 0.1, 0
dot = []

fig, ax = plt.subplots(figsize=(10, 10))
ax.set_xlim(-50, 50)
ax.set_ylim(-50, 50)
ax.set_xlabel("X (метры)")
ax.set_ylabel("Y (метры)")
ax.grid()
ax.axis('equal')
ax.scatter(0, 0, color='black', s=20)

while t < 100:
    r_2 = L**2 + l**2
    r = sqrt(r_2)
    F = G * m * M / r_2
    Vx -= (l / r) * F * dt
    Vy -= (L / r) * F * dt
    l += Vx * dt
    L += Vy * dt
    dot.append((l, L))
    if len(dot) % 3 == 0:
        x, y = zip(*dot)
        ax.plot(x, y, 'bo-', markersize=1)
        dot = [(l,L)]

    t += dt
    plt.pause(0.01)
