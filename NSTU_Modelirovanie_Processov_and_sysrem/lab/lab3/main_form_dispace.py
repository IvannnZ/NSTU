import random
import time

FILENAME = "data.txt"
NUM_READERS = 3  # Количество процессов чтения
BUFFER_SIZE = 10  # Размер буфера записи

class Reader:
    def __init__(self):
        self.buffer = []
        self.read_count = 0

def initialize_file():
    with open(FILENAME, "w") as f:
        f.write("abcdefghijklmnopqrstuvwxyz")

def weighted_random_selection(probabilities):
    r = random.uniform(0, 1)
    cumulative = 0
    for i, p in enumerate(probabilities):
        cumulative += p
        if r < cumulative:
            return i
    return len(probabilities) - 1

def main():
    initialize_file()
    
    readers = [Reader() for _ in range(NUM_READERS)]
    write_buffer = []
    writing = False
    
    probabilities = [1.0 / (NUM_READERS + 1)] * (NUM_READERS + 1)
    total_written = 0
    
    with open(FILENAME, "r+") as f:
        while True:
            selected_process = weighted_random_selection(probabilities)
            
            if selected_process < NUM_READERS:  # Читающий процесс
                if not writing:
                    ch = f.read(1)
                    if not ch:  # Если дошли до конца файла, читаем с начала
                        f.seek(0)
                        ch = f.read(1)
                    readers[selected_process].buffer.append(ch)
                    readers[selected_process].read_count += 1
            else:  # Процесс записи
                if not writing:
                    writing = True
                    f.seek(0)
                    f.truncate()
                
                if write_buffer:
                    f.write(write_buffer.pop())
                    total_written += 1
                else:
                    break
            
            time.sleep(0.01)  # Небольшая задержка для имитации процессов
    
    print("Результаты чтения:")
    for i, reader in enumerate(readers):
        print(f"Читатель {i + 1} считал {reader.read_count} символов.")

if __name__ == "__main__":
    main()
