import random
import time
from itertools import permutations


def broot_force(distances, num_cities):
    if num_cities:
        best_path = [distances[0]]
        best_path_len = sum(i for i in distances[0])
        cities = [i for i in range(num_cities)]
        for i in permutations(cities):
            len = sum(distances[i[j]][i[j + 1]] for j in range(num_cities - 1)) + distances[i[-1]][i[0]]
            if len < best_path_len:
                best_path_len = len
                best_path = i
        return best_path
    return []

def reduce_matrix(matrix):
    """Редукция матрицы: вычитает минимальные значения из строк и столбцов."""
    cost = 0
    n = len(matrix)

    # Редукция строк
    for i in range(n):
        min_val = min(matrix[i])
        if min_val != float('inf') and min_val > 0:
            cost += min_val
            for j in range(n):
                matrix[i][j] -= min_val

    # Редукция столбцов
    for j in range(n):
        min_val = min(matrix[i][j] for i in range(n))
        if min_val != float('inf') and min_val > 0:
            cost += min_val
            for i in range(n):
                matrix[i][j] -= min_val

    return cost, matrix

def find_best_edge(matrix):
    """Находит ребро с наибольшей оценкой разрыва."""
    n = len(matrix)
    max_penalty = -1
    best_edge = (-1, -1)

    for i in range(n):
        for j in range(n):
            if matrix[i][j] == 0:
                penalty = 0
                for k in range(n):
                    if matrix[i][k]!=float('inf'):
                        penalty+=matrix[i][k]
                    if matrix[k][j]!=float('inf'):
                        penalty+=matrix[k][j]


                if penalty > max_penalty:
                    max_penalty = penalty
                    best_edge = (i, j)

    return best_edge

def branch_and_bound(matrix, path, lower_bound, best_cost, best_path):
    """Метод ветвей и границ с выбором ребра по оценке разрыва."""
    n = len(matrix)

    if len(path) == n:
        cost = lower_bound + matrix[path[-1]][path[0]]
        if cost < best_cost[0]:
            best_cost[0] = cost
            best_path[0] = path[:]
        return

    best_i, best_j = find_best_edge(matrix)

    if best_i == -1 or best_j == -1:
        return

    # Ветвь 1: включаем ребро (best_i, best_j)
    new_matrix = [row[:] for row in matrix]
    for k in range(n):
        new_matrix[best_i][k] = float('inf')
        new_matrix[k][best_j] = float('inf')
    new_matrix[best_j][best_i] = float('inf')

    cost, new_matrix = reduce_matrix(new_matrix)
    new_lower_bound = lower_bound + cost

    if new_lower_bound < best_cost[0]:
        branch_and_bound(new_matrix, path + [best_j], new_lower_bound, best_cost, best_path)

    # Ветвь 2: запрещаем ребро (best_i, best_j)
    matrix[best_i][best_j] = float('inf')
    cost, matrix = reduce_matrix(matrix)
    new_lower_bound = lower_bound + cost

    if new_lower_bound < best_cost[0]:
        branch_and_bound(matrix, path, new_lower_bound, best_cost, best_path)

def little(matrix):
    """Основная функция для решения TSP методом Литтла."""
    n = len(matrix)

    for i in range(n):
        matrix[i][i] = float('inf')

    cost, reduced_matrix = reduce_matrix([row[:] for row in matrix])
    best_cost = [float('inf')]
    best_path = [[]]

    branch_and_bound(reduced_matrix, [0], cost, best_cost, best_path)

    return best_path[0], best_cost[0]






for num_cities in range(3,20):
    distances = [[random.randint(1, 20) if i > j else 0 for j in range(num_cities)] for i in range(num_cities)]
    for i in range(num_cities):
        for j in range(num_cities):
            if i > j:
                distances[j][i] = distances[i][j]

    start = time.time()
    broot_force(distances, num_cities)
    time_broot = time.time()-start
    start = time.time()
    little(distances)
    time_little = time.time() - start

    print("num_cities: ",num_cities," broot: ",time_broot," little: ",time_little," b/l: ",time_broot/time_little)