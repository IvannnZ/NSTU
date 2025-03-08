import random
import threading
import time


class FileAccess:
    def __init__(self):
        self.lock = threading.Lock()
        self.read_lock = threading.Lock()
        self.readers = 0
        self.file_content = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"  # Исходное содержимое файла
        self.file_pointer = 0

    def start_read(self, reader_id, buffer):
        with self.read_lock:
            self.readers += 1
            if self.readers == 1:
                self.lock.acquire()

        if self.file_pointer < len(self.file_content):
            buffer.append(self.file_content[self.file_pointer])
            self.file_pointer += 1
            print(f"Reader {reader_id} read: {buffer[-1]}")

        with self.read_lock:
            self.readers -= 1
            if self.readers == 0:
                self.lock.release()

    def start_write(self, writer_id):
        with self.lock:
            print(f"Writer {writer_id} is writing...")
            time.sleep(0.1)  # Симуляция записи
            print(f"Writer {writer_id} finished writing.")


class Process(threading.Thread):
    def __init__(self, process_id, process_type, file_access, priority, buffer=None):
        super().__init__()
        self.process_id = process_id
        self.process_type = process_type
        self.file_access = file_access
        self.priority = priority
        self.buffer = buffer if buffer is not None else []

    def run(self):
        if self.process_type == 'reader':
            self.file_access.start_read(self.process_id, self.buffer)
        elif self.process_type == 'writer':
            self.file_access.start_write(self.process_id)


if __name__ == "__main__":
    file_access = FileAccess()
    readers = [Process(i, 'reader', file_access, random.randint(1, 5), []) for i in range(5)]
    writers = [Process(i, 'writer', file_access, random.randint(1, 5)) for i in range(2)]

    processes = readers + writers
    random.shuffle(processes)  # Случайное распределение порядка процессов

    for _ in range(10):
        process = random.choices(processes, weights=[p.priority for p in processes])[0]
        process.start()

    for process in processes:
        process.join()

    for i, reader in enumerate(readers):
        print(f"Reader {i} buffer: {''.join(reader.buffer)}")
