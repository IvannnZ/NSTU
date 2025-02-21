# import time
# import random
#
#
# def calculate_distance(route, distance_matrix):
#     """Вычисляет суммарное расстояние для заданного маршрута."""
#     distance = 0
#     for i in range(len(route) - 1):
#         distance += distance_matrix[route[i]][route[i + 1]]
#     distance += distance_matrix[route[-1]][route[0]]  # Возвращаемся в начальный город
#     return distance
#
#
# def generate_permutations(arr, l, r, permutations):
#     """Генерирует все возможные перестановки маршрутов."""
#     if l == r:
#         permutations.append(arr[:])  # Копируем текущий маршрут
#     else:
#         for i in range(l, r + 1):
#             arr[l], arr[i] = arr[i], arr[l]  # Меняем местами
#             generate_permutations(arr, l + 1, r, permutations)
#             arr[l], arr[i] = arr[i], arr[l]  # Возвращаем назад
#
#
# def brute_force_tsp(distance_matrix):
#     """Решает задачу коммивояжера полным перебором."""
#     num_cities = len(distance_matrix)
#     cities = list(range(num_cities))
#     all_routes = []
#
#     generate_permutations(cities, 0, num_cities - 1, all_routes)
#
#     best_route = None
#     min_distance = float('inf')
#
#     for route in all_routes:
#         distance = calculate_distance(route, distance_matrix)
#         if distance < min_distance:
#             min_distance = distance
#             best_route = route
#
#     return best_route, min_distance
#
#
# def branch_and_bound_tsp(distance_matrix):
#     """Решает задачу коммивояжера методом ветвей и границ."""
#     from queue import PriorityQueue
#
#     num_cities = len(distance_matrix)
#     pq = PriorityQueue()
#     pq.put((0, [0]))  # Начинаем с первого города
#     best_distance = float('inf')
#     best_route = None
#
#     while not pq.empty():
#         cost, path = pq.get()
#         if len(path) == num_cities:
#             total_cost = cost + distance_matrix[path[-1]][path[0]]
#             if total_cost < best_distance:
#                 best_distance = total_cost
#                 best_route = path
#             continue
#
#         for next_city in range(num_cities):
#             if next_city not in path:
#                 new_cost = cost + distance_matrix[path[-1]][next_city]
#                 pq.put((new_cost, path + [next_city]))
#
#     return best_route, best_distance
#
#
# def generate_distance_matrix(size):
#     """Генерирует случайную матрицу расстояний."""
#     return [[random.randint(10, 100) if i != j else 0 for j in range(size)] for i in range(size)]
#
#
# def main():
#     num_cities = 10  # Количество городов
#     distance_matrix = generate_distance_matrix(num_cities)
#
#     start_time = time.time()
#     route_bnb, dist_bnb = branch_and_bound_tsp(distance_matrix)
#     bnb_time = time.time() - start_time
#
#     print(f"Метод ветвей и границ: {dist_bnb}, маршрут: {route_bnb}, время: {bnb_time:.6f} сек")
#
#
#     start_time = time.time()
#     route_brute, dist_brute = brute_force_tsp(distance_matrix)
#     brute_time = time.time() - start_time
#     print(f"Полный перебор: {dist_brute}, маршрут: {route_brute}, время: {brute_time:.6f} сек")
#
#
#
# if __name__ == "__main__":
#     main()
import pygame
import time
import random
from queue import PriorityQueue

# Константы для визуализации
WIDTH, HEIGHT = 800, 600
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
BLUE = (0, 0, 255)
RED = (255, 0, 0)

# Настройка Pygame
pygame.init()
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("График скорости алгоритмов TSP")
font = pygame.font.SysFont(None, 24)


def calculate_distance(route, distance_matrix):
    """Вычисляет суммарное расстояние маршрута"""
    distance = sum(distance_matrix[route[i]][route[i + 1]] for i in range(len(route) - 1))
    distance += distance_matrix[route[-1]][route[0]]  # Возвращаемся в начальный город
    return distance


def generate_permutations(arr, l, r, permutations):
    """Рекурсивно генерирует все перестановки"""
    if l == r:
        permutations.append(arr[:])
    else:
        for i in range(l, r + 1):
            arr[l], arr[i] = arr[i], arr[l]
            generate_permutations(arr, l + 1, r, permutations)
            arr[l], arr[i] = arr[i], arr[l]


def brute_force_tsp(distance_matrix):
    """Решает задачу коммивояжера полным перебором"""
    num_cities = len(distance_matrix)
    cities = list(range(num_cities))
    all_routes = []

    generate_permutations(cities, 0, num_cities - 1, all_routes)

    min_distance = float('inf')

    for route in all_routes:
        distance = calculate_distance(route, distance_matrix)
        if distance < min_distance:
            min_distance = distance

    return min_distance


def branch_and_bound_tsp(distance_matrix):
    """Решает задачу методом ветвей и границ"""
    num_cities = len(distance_matrix)
    pq = PriorityQueue()
    pq.put((0, [0]))  # Начинаем с первого города
    best_distance = float('inf')

    while not pq.empty():
        cost, path = pq.get()
        if len(path) == num_cities:
            total_cost = cost + distance_matrix[path[-1]][path[0]]
            if total_cost < best_distance:
                best_distance = total_cost
            continue

        for next_city in range(num_cities):
            if next_city not in path:
                new_cost = cost + distance_matrix[path[-1]][next_city]
                pq.put((new_cost, path + [next_city]))

    return best_distance


def generate_distance_matrix(size):
    """Создаёт случайную матрицу расстояний"""
    return [[random.randint(10, 100) if i != j else 0 for j in range(size)] for i in range(size)]


def draw_axes():
    """Рисует координатные оси и подписи"""
    pygame.draw.line(screen, BLACK, (50, HEIGHT - 50), (WIDTH - 50, HEIGHT - 50), 2)  # X-ось
    pygame.draw.line(screen, BLACK, (50, 50), (50, HEIGHT - 50), 2)  # Y-ось

    # Подписи к осям
    screen.blit(font.render("Число городов", True, BLACK), (WIDTH // 2, HEIGHT - 30))
    screen.blit(font.render("Время (мс)", True, BLACK), (10, 20))


def main():
    running = True
    num_cities_range = range(2, 20)  # Меняйте диапазон для большего числа городов
    brute_force_results = []
    branch_and_bound_results = []

    screen.fill(WHITE)
    draw_axes()

    for num_cities in num_cities_range:
        distance_matrix = generate_distance_matrix(num_cities)

        # Измерение времени полного перебора
        start_time = time.time()
        brute_force_tsp(distance_matrix)
        brute_time = (time.time() - start_time) * 1000  # Перевод в миллисекунды
        brute_force_results.append((num_cities, brute_time))

        # Измерение времени метода ветвей и границ
        start_time = time.time()
        branch_and_bound_tsp(distance_matrix)
        bnb_time = (time.time() - start_time) * 1000
        branch_and_bound_results.append((num_cities, bnb_time))
        print(bnb_time)
        # Отображение точек на графике
        x_brute = 50 + (num_cities - 2) * (WIDTH - 100) // (len(num_cities_range) - 1)
        y_brute = HEIGHT - 50 - int(brute_time)  # Масштаб времени
        pygame.draw.circle(screen, RED, (x_brute, max(50, y_brute)), 5)  # Красные точки

        x_bnb = 50 + (num_cities - 2) * (WIDTH - 100) // (len(num_cities_range) - 1)
        y_bnb = HEIGHT - 50 - int(bnb_time)
        pygame.draw.circle(screen, BLUE, (x_bnb, max(50, y_bnb)), 5)  # Синие точки

        # Обновление экрана после каждой точки
        pygame.display.flip()

        time.sleep(0.5)  # Небольшая задержка для визуализации

    # Основной цикл для отображения окна
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

    pygame.quit()


main()