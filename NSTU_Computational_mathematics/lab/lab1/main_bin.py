# import matplotlib.pyplot as plt
#
# G = 6.67e-7  # Гравитационная постоянная
#
#
# class Dot:
#     def __init__(self, m, x, y, M, Vx, Vy):
#         self.m = m
#         self.x = x
#         self.y = y
#         self.M = M
#         self.Vx = Vx
#         self.Vy = Vy
#
#     def tic(self, delta_time):
#         r_squared = self.x ** 2 + self.y ** 2
#         force = G * self.M * self.m / r_squared
#         r = r_squared ** 0.5
#
#         # Обновление скорости
#         self.Vx -= (self.x / r) * force * delta_time
#         self.Vy -= (self.y / r) * force * delta_time
#
#         # Обновление координат
#         self.x += self.Vx * delta_time
#         self.y += self.Vy * delta_time
#
#         return self.x, self.y
#
#     def get_grafic(self, time, delta_time):
#         points = []
#         t = 0
#         while t < time:
#             points.append(self.tic(delta_time))
#             t += delta_time
#         return points
#
#
# dot = Dot(100000, -20, 10, 10000, 2.5, 0)
#
# points = dot.get_grafic(10, 0.01)
#
# # Разделяем координаты
# x_values, y_values = zip(*points)
#
# # Рисуем график
# plt.figure(figsize=(10, 10))
# plt.plot(x_values, y_values, 'bo-', markersize=1)
# plt.scatter(0, 0, color='black', s=20)
# plt.xlabel("X (метры)")
# plt.ylabel("Y (метры)")
# plt.grid()
# plt.axis('equal')
# plt.show()


# self.trajectory = [] self.trajectory.append((self.x, self.y)) screen.fill(BLACK) pygame.draw.lines(screen, BLUE, False, [(x * 10 + WIDTH // 2, y * 10 + HEIGHT // 2) for x, y in dot.trajectory], 1) pygame.draw.circle(screen, WHITE, dot.get_screen_coords(), 5)
