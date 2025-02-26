import random
from itertools import permutations
import FibonacciHeap
from lab.lab2.FibonacciHeap import Bin_heap




class bin_arr:
    def __init__(self):
        self.arr = 0

    def __getitem__(self, item):
        return (self.arr >> item) & 1
    def __setitem__(self, key, value):
        assert value < 0 or value > 1
        self.arr |= value<<key
    def is_full(self, num_args):
        return self.arr == ((2**num_args) - 1)
'''
переделать на цикл, вместо рекурсии

посмотреть более быструю реализацию в алгосах

указать старт цикла
'''
def deikstra_he_he(distances, num_cities, already_vizited:bin_arr, path):
    min_way_to = Bin_heap()
    min_way_to.insert(0,0)

    while (True):
        now = min_way_to.peek_min()
        if now.value == 0:
            return now.priority


    ...

# def broot_force(distances, num_cities):
#     if num_cities:
#         best_path = [distances[0]]
#         best_path_len = sum(i for i in distances[0])
#         cities = [i for i in range(num_cities)]
#         for i in permutations(cities):
#             len = sum(distances[i[j]][i[j + 1]] for j in range(num_cities - 1)) + distances[i[-1]][i[0]]
#             if len < best_path_len:
#                 best_path_len = len
#                 best_path = i
#         return best_path
#     return []
#
#
# def reduce_matrix(matrix):
#     """Редукция матрицы: вычитает минимальные значения из строк и столбцов."""
#     cost = 0
#     n = len(matrix)
#
#     # Редукция строк
#     for i in range(n):
#         min_val = min(matrix[i])
#         if min_val != float('inf') and min_val > 0:
#             cost += min_val
#             for j in range(n):
#                 matrix[i][j] -= min_val
#
#     # Редукция столбцов
#     for j in range(n):
#         min_val = min(matrix[i][j] for i in range(n))
#         if min_val != float('inf') and min_val > 0:
#             cost += min_val
#             for i in range(n):
#                 matrix[i][j] -= min_val
#
#     return cost, matrix
#
# def branch_and_bound(matrix, path, lower_bound, level, best_cost, best_path):
#     """Метод ветвей и границ для решения задачи коммивояжера."""
#     n = len(matrix)
#
#     if level == n:
#         cost = lower_bound + matrix[path[-1]][path[0]]
#         if cost < best_cost[0]:
#             best_cost[0] = cost
#             best_path[0] = path[:]
#         return
#
#     for next_city in range(n):
#         if next_city not in path:
#             new_matrix = [row[:] for row in matrix]
#
#             # Запрещаем переходы из текущего города и в следующий город
#             for i in range(n):
#                 new_matrix[path[-1]][i] = float('inf')
#                 new_matrix[i][next_city] = float('inf')
#             new_matrix[next_city][path[0]] = float('inf')
#
#             cost, new_matrix = reduce_matrix(new_matrix)
#             new_lower_bound = lower_bound + matrix[path[-1]][next_city] + cost
#
#             # Отладочный вывод текущего пути и оценки
#             print(f"Шаг {level}: Путь {path + [next_city]}, lower_bound={new_lower_bound}, best_cost={best_cost[0]}")
#
#             if new_lower_bound < best_cost[0]:
#                 branch_and_bound(new_matrix, path + [next_city], new_lower_bound, level + 1, best_cost, best_path)
#
# def solve_tsp(matrix):
#     """Основная функция для решения TSP методом Литтла."""
#     n = len(matrix)
#
#     # Запрещаем переход в сам себя
#     for i in range(n):
#         matrix[i][i] = float('inf')
#
#     cost, reduced_matrix = reduce_matrix([row[:] for row in matrix])
#     best_cost = [float('inf')]
#     best_path = [[]]  # Используем пустой список вместо None
#
#     for start in range(n):
#         print(f"=== Начинаем с города {start} ===")
#         branch_and_bound(reduced_matrix, [start], cost, 1, best_cost, best_path)
#
#     return best_path[0], best_cost[0]

def reduce_matrix(matrix):
    """Редукция матрицы: вычитает минимальные значения из строк и столбцов."""
    cost = 0
    n = len(matrix)

    # Редукция строк
    for i in range(n):
        min_val = min([val for val in matrix[i] if val != float('inf')], default=float('inf'))
        if min_val != float('inf'):
            cost += min_val
            for j in range(n):
                if matrix[i][j] != float('inf'):
                    matrix[i][j] -= min_val

    # Редукция столбцов
    for j in range(n):
        min_val = min([matrix[i][j] for i in range(n) if matrix[i][j] != float('inf')], default=float('inf'))
        if min_val != float('inf'):
            cost += min_val
            for i in range(n):
                if matrix[i][j] != float('inf'):
                    matrix[i][j] -= min_val

    return cost, matrix

def branch_and_bound(matrix, path, lower_bound, level, best_cost, best_path):
    """Метод ветвей и границ для решения задачи коммивояжера."""
    n = len(matrix)

    if level == n:
        final_cost = lower_bound + (matrix[path[-1]][path[0]] if matrix[path[-1]][path[0]] != float('inf') else 0)
        if final_cost < best_cost[0]:
            best_cost[0] = final_cost
            best_path[0] = path[:]
        return

    for next_city in range(n):
        if next_city not in path:
            new_matrix = [row[:] for row in matrix]

            # Запрещаем переходы из текущего города и в следующий город
            for i in range(n):
                new_matrix[path[-1]][i] = float('inf')
                new_matrix[i][next_city] = float('inf')
            new_matrix[next_city][path[0]] = float('inf')

            cost, new_matrix = reduce_matrix(new_matrix)
            new_lower_bound = lower_bound + (matrix[path[-1]][next_city] if matrix[path[-1]][next_city] != float('inf') else 0) + cost

            # Отладочный вывод текущего пути и оценки
            print(f"Шаг {level}: Путь {path + [next_city]}, lower_bound={new_lower_bound}, best_cost={best_cost[0]}")

            if new_lower_bound < best_cost[0]:
                branch_and_bound(new_matrix, path + [next_city], new_lower_bound, level + 1, best_cost, best_path)

def solve_tsp(matrix):
    """Основная функция для решения TSP методом Литтла."""
    n = len(matrix)

    # Запрещаем переход в сам себя
    for i in range(n):
        matrix[i][i] = float('inf')

    cost, reduced_matrix = reduce_matrix([row[:] for row in matrix])
    best_cost = [float('inf')]
    best_path = [[]]  # Используем пустой список вместо None

    for start in range(n):
        print(f"=== Начинаем с города {start} ===")
        branch_and_bound(reduced_matrix, [start], cost, 1, best_cost, best_path)

    return best_path[0], best_cost[0]

# Пример для 3 городов:
distance_matrix_3 = [
    [0, 10, 15],
    [10, 0, 35],
    [15, 35, 0]
]

best_path, best_cost = solve_tsp(distance_matrix_3)
print("Оптимальный маршрут:", best_path)
print("Минимальная стоимость:", best_cost)



# for num_cities in range(10):
# num_cities = 3
# distances = [[random.randint(1, 20) if i > j else 0 for j in range(num_cities)] for i in range(num_cities)]
# for i in range(num_cities):
#     for j in range(num_cities):
#         if i > j:
#             distances[j][i] = distances[i][j]
#
# # print(broot_force(distances, num_cities))
# print(distances)
# print(solve_tsp(distances))
