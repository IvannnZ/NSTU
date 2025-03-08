import random

def weighted_random_index(priorities):
    if not priorities:
        return -1

    total_weight = sum(priorities)
    if total_weight == 0:
        return -1

    random_value = random.randint(0, total_weight - 1)
    cumulative_weight = 0
    for i, weight in enumerate(priorities):
        cumulative_weight += weight
        if random_value < cumulative_weight:
            return i
    return -1


def fill_priorities(size, read_flow_size):
    if size <= 0 or read_flow_size <= 0:
        return []
    return [random.randint(1, read_flow_size) for _ in range(size)]


def main():
    size = 100
    read_flow_size = size
    size_file = 10

    priorities = fill_priorities(size, read_flow_size)
    save_priorities = priorities[:]
    values = [0] * size
    absolute_value = 0

    while absolute_value < size:
        temp = weighted_random_index(priorities)
        if temp == -1:
            break
        values[temp] += 1
        if values[temp] == size_file:
            absolute_value += 1
            priorities[temp] = 0

    for i in range(size):
        print(f"Process {i} with priority {save_priorities[i]} read {values[i]} characters.")


if __name__ == "__main__":
    main()
