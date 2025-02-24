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


def get_coordinates_no_loop(m, x0, y0, M, Vx0, Vy0, t):
    r = (x0 ** 2 + y0 ** 2) ** 0.5
    ax = -G * M * m * x0 / r ** 3
    ay = -G * M * m * y0 / r ** 3

    x = x0 + Vx0 * t + 0.5 * ax * t ** 2
    y = y0 + Vy0 * t + 0.5 * ay * t ** 2
    print(x,y)

    return int(x * 10 + WIDTH // 2), int(-y * 10 + HEIGHT // 2)


class Dot:
    def __init__(self, m, x, y, M, Vx, Vy):
        self.m = m
        self.x = x
        self.y = y
        self.M = M
        self.Vx = Vx
        self.Vy = Vy

    def tic(self, delta_time):
        r_squared = self.x ** 2 + self.y ** 2
        force = G * self.M * self.m / r_squared
        r = r_squared ** 0.5

        self.Vx -= (self.x / r) * force * delta_time
        self.Vy -= (self.y / r) * force * delta_time

        self.x += self.Vx * delta_time
        self.y += self.Vy * delta_time
        return self.x, self.y

    def get_screen_coords(self):
        return int(self.x * 10 + WIDTH // 2), int(-self.y * 10 + HEIGHT // 2)


dot = Dot(10000, -40, 12, 1000000, 10, 0)

dot2 = Dot(1000, 0, 12, 1000000, 2, 2)

running = True
clock = pygame.time.Clock()
delta_time = 0.01
speed_multiplier = 1
screen.fill(BLACK)
pygame.draw.circle(screen, YELLOW, CENTER, 10)
t = -10
dot.tic(delta_time * 20)
while running:
    t += delta_time
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_RIGHT:
                speed_multiplier = min(speed_multiplier + 10, 50)
            elif event.key == pygame.K_LEFT:
                speed_multiplier = max(speed_multiplier - 5, 1)
            pygame.display.set_caption(f"Лаб 1 - Скорость x{speed_multiplier}")

    for _ in range(int(speed_multiplier)):
        dot.tic(delta_time)
        pygame.draw.circle(screen, BLUE, dot.get_screen_coords(), 5)
        dot2.tic(delta_time)
        # pygame.draw.circle(screen, WHITE, dot2.get_screen_coords(), 1)

        # pygame.draw.circle(screen, WHITE, get_coordinates_no_loop(1000, 0, 12, 1000000, 2, 2, t), 1)

        # screen.set_at(dot.get_screen_coords(), BLUE)

    pygame.display.flip()
    clock.tick(60)

pygame.quit()
