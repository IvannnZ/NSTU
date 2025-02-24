import random
from itertools import permutations


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
'''
переделать на цикл, вместо рекурсии

посмотреть более быструю реализацию в алгосах

указать старт цикла
'''
def deikstra_he_he(distances, num_cities, already_vizited:bin_arr, path):
    if already_vizited.is_full(num_cities):
        return path
    min_len = 0

    for i in range(num_cities):
        if already_vizite[i]

    ...

for num_cities in range(20):
    distances = [[random.randint(1, 20) if i != j else 0 for j in range(num_cities)] for i in range(num_cities)]
    print(broot_force(distances, num_cities))
