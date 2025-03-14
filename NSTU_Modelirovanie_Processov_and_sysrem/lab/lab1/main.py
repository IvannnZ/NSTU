import pygame

G = 6.67e-7  # Гравитационная постоянная
WIDTH, HEIGHT = 800, 800
CENTER = (WIDTH // 2, HEIGHT // 2)

BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
BLUE = (0, 0, 255)
YELLOW = (255, 255, 0)

pygame.init()
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Лаб 1")

# Параметры точки
m = 10000
x, y = -40, 12
M = 1000000
Vx, Vy = 10, 0
delta_time = 0.01
speed_multiplier = 1


def update_position(x, y, Vx, Vy, m, M, delta_time):
    r_squared = x ** 2 + y ** 2
    force = G * M * m / r_squared
    r = r_squared ** 0.5

    Vx -= (x / r) * force * delta_time
    Vy -= (y / r) * force * delta_time

    x += Vx * delta_time
    y += Vy * delta_time

    return x, y, Vx, Vy


def get_screen_coords(x, y):
    return int(x * 10 + WIDTH // 2), int(-y * 10 + HEIGHT // 2)


running = True
clock = pygame.time.Clock()
screen.fill(BLACK)
pygame.draw.circle(screen, YELLOW, CENTER, 10)

t = -10
x, y, Vx, Vy = update_position(x, y, Vx, Vy, m, M, delta_time * 20)

while running:
    t += delta_time
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    x, y, Vx, Vy = update_position(x, y, Vx, Vy, m, M, delta_time)
    pygame.draw.circle(screen, BLUE, get_screen_coords(x, y), 5)

    pygame.display.flip()
    clock.tick(60)

pygame.quit()
