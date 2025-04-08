using System;

class Program
{
    static void Main()
    {
        Console.WriteLine("Программа преобразования матрицы к верхнему треугольному виду");
        
        // Запрос размеров матрицы у пользователя
        Console.Write("Введите количество строк матрицы: ");
        int rows = int.Parse(Console.ReadLine());
        
        Console.Write("Введите количество столбцов матрицы: ");
        int cols = int.Parse(Console.ReadLine());
        
        // Выбор типа данных
        Console.WriteLine("\nВыберите тип данных элементов матрицы:");
        Console.WriteLine("1 -> int");
        Console.WriteLine("2 -> float");
        Console.WriteLine("3 -> double");
        Console.Write("Ваш выбор: ");
        int typeChoice = int.Parse(Console.ReadLine());
        
        // Запрос диапазона значений
        Console.Write("\nВведите минимальное значение элементов: ");
        double minVal = double.Parse(Console.ReadLine());
        
        Console.Write("Введите максимальное значение элементов: ");
        double maxVal = double.Parse(Console.ReadLine());
        
        // Создание и заполнение матрицы в зависимости от выбранного типа
        Array matrix = null;
        Random rand = new Random();
        
        switch (typeChoice)
        {
            case 1: // int
                matrix = CreateAndFillMatrix<int>(rows, cols, rand, minVal, maxVal);
                break;
            case 2: // float
                matrix = CreateAndFillMatrix<float>(rows, cols, rand, minVal, maxVal);
                break;
            case 3: // double
                matrix = CreateAndFillMatrix<double>(rows, cols, rand, minVal, maxVal);
                break;
            default:
                Console.WriteLine("Неверный выбор типа данных. Используется double по умолчанию.");
                matrix = CreateAndFillMatrix<double>(rows, cols, rand, minVal, maxVal);
                break;
        }
        
        // Вывод исходной матрицы
        Console.WriteLine("\nИсходная матрица:");
        PrintMatrix(matrix);
        
        // Приведение матрицы к верхнему треугольному виду
        Array triangularMatrix = ConvertToUpperTriangular(matrix);
        
        // Вывод треугольной матрицы
        Console.WriteLine("\nМатрица в верхнем треугольном виде:");
        PrintMatrix(triangularMatrix);
        
        // Преобразование в ступенчатый массив
        Array jaggedArray = ConvertToJaggedArray(triangularMatrix);
        
        // Вывод ступенчатого массива
        Console.WriteLine("\nСтупенчатый массив:");
        PrintJaggedArray(jaggedArray);
    }
    
    // Создание и заполнение матрицы заданного типа
    static Array CreateAndFillMatrix<T>(int rows, int cols, Random rand, double minVal, double maxVal)
    {
        Array matrix = Array.CreateInstance(typeof(T), rows, cols);
        
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                double val = rand.NextDouble() * (maxVal - minVal) + minVal;
                
                if (typeof(T) == typeof(int))
                    matrix.SetValue((int)Math.Round(val), i, j);
                else if (typeof(T) == typeof(float))
                    matrix.SetValue((float)val, i, j);
                else
                    matrix.SetValue(val, i, j);
            }
        }
        
        return matrix;
    }
    
    // Приведение матрицы к верхнему треугольному виду
    static Array ConvertToUpperTriangular(Array matrix)
    {
        int rows = matrix.GetLength(0);
        int cols = matrix.GetLength(1);
        Type elementType = matrix.GetType().GetElementType();
        Array result = (Array)matrix.Clone();
        
        int rank = 0;
        for (int j = 0; j < cols && rank < rows; j++)
        {
            // Поиск строки с максимальным элементом в текущем столбце
            int maxRow = rank;
            double maxVal = Math.Abs(Convert.ToDouble(result.GetValue(rank, j)));
            
            for (int i = rank + 1; i < rows; i++)
            {
                double currentVal = Math.Abs(Convert.ToDouble(result.GetValue(i, j)));
                if (currentVal > maxVal)
                {
                    maxVal = currentVal;
                    maxRow = i;
                }
            }
            
            // Если все элементы в столбце нулевые, переходим к следующему столбцу
            if (maxVal < 1e-10)
            {
                continue;
            }
            
            // Перестановка строк
            if (maxRow != rank)
            {
                for (int k = 0; k < cols; k++)
                {
                    object temp = result.GetValue(rank, k);
                    result.SetValue(result.GetValue(maxRow, k), rank, k);
                    result.SetValue(temp, maxRow, k);
                }
            }
            
            // Нормализация текущей строки
            double pivot = Convert.ToDouble(result.GetValue(rank, j));
            for (int k = j; k < cols; k++)
            {
                double newValue = Convert.ToDouble(result.GetValue(rank, k)) / pivot;
                result.SetValue(Convert.ChangeType(newValue, elementType), rank, k);
            }
            
            // Обнуление элементов ниже ведущего
            for (int i = rank + 1; i < rows; i++)
            {
                double factor = Convert.ToDouble(result.GetValue(i, j));
                for (int k = j; k < cols; k++)
                {
                    double newValue = Convert.ToDouble(result.GetValue(i, k)) - 
                                     Convert.ToDouble(result.GetValue(rank, k)) * factor;
                    result.SetValue(Convert.ChangeType(newValue, elementType), i, k);
                }
            }
            
            rank++;
        }
        
        return result;
    }
    
    // Преобразование матрицы в ступенчатый массив
    static Array ConvertToJaggedArray(Array matrix)
    {
        int rows = matrix.GetLength(0);
        int cols = matrix.GetLength(1);
        Type elementType = matrix.GetType().GetElementType();
        
        // Создаем массив массивов
        Array jaggedArray = Array.CreateInstance(Array.CreateInstance(elementType, 0).GetType(), rows);
        
        for (int i = 0; i < rows; i++)
        {
            // Определяем длину текущей строки (после первого нулевого элемента)
            int length = cols;
            for (int j = 0; j < cols; j++)
            {
                if (Math.Abs(Convert.ToDouble(matrix.GetValue(i, j))) < 1e-10)
                {
                    bool allZeros = true;
                    for (int k = j; k < cols; k++)
                    {
                        if (Math.Abs(Convert.ToDouble(matrix.GetValue(i, k))) >= 1e-10)
                        {
                            allZeros = false;
                            break;
                        }
                    }
                    if (allZeros)
                    {
                        length = j;
                        break;
                    }
                }
            }
            
            // Создаем строку ступенчатого массива
            Array row = Array.CreateInstance(elementType, length);
            for (int j = 0; j < length; j++)
            {
                row.SetValue(matrix.GetValue(i, j), j);
            }
            
            jaggedArray.SetValue(row, i);
        }
        
        return jaggedArray;
    }
    
    // Вывод матрицы на экран
    static void PrintMatrix(Array matrix)
    {
        int rows = matrix.GetLength(0);
        int cols = matrix.GetLength(1);
        
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                Console.Write(string.Format("{0,10:F2} ", matrix.GetValue(i, j)));
            }
            Console.WriteLine();
        }
    }
    
    // Вывод ступенчатого массива на экран
    static void PrintJaggedArray(Array jaggedArray)
    {
        for (int i = 0; i < jaggedArray.Length; i++)
        {
            Array row = (Array)jaggedArray.GetValue(i);
            foreach (var element in row)
            {
                Console.Write(string.Format("{0,10:F2} ", element));
            }
            Console.WriteLine();
        }
    }
}