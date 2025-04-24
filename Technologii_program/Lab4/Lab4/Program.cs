using System;

class Program
{
    static void Main()
    {
        Console.WriteLine("Программа приведения матрицы к нижнетреугольному виду");
        
         
        Console.Write("Введите размер матрицы (число строк/столбцов): ");
        int size = int.Parse(Console.ReadLine());
        
         
        Console.WriteLine("\nВыберите тип данных элементов матрицы:");
        Console.WriteLine("1 -> int");
        Console.WriteLine("2 -> float");
        Console.WriteLine("3 -> double");
        Console.Write("Ваш выбор: ");
        int typeChoice = int.Parse(Console.ReadLine());
        
         
        Console.Write("\nВведите минимальное значение элементов: ");
        double minVal = double.Parse(Console.ReadLine());
        
        Console.Write("Введите максимальное значение элементов: ");
        double maxVal = double.Parse(Console.ReadLine());
        
         
        switch (typeChoice)
        {
            case 1:
                ProcessMatrix<int>(size, minVal, maxVal);
                break;
            case 2:
                ProcessMatrix<float>(size, minVal, maxVal);
                break;
            case 3:
                ProcessMatrix<double>(size, minVal, maxVal);
                break;
            default:
                Console.WriteLine("Неверный выбор. Используется double по умолчанию.");
                ProcessMatrix<double>(size, minVal, maxVal);
                break;
        }
    }

    static void ProcessMatrix<T>(int size, double minVal, double maxVal) where T : struct
    {
         
        T[,] matrix = CreateRandomMatrix<T>(size, minVal, maxVal);
        
        Console.WriteLine("\nИсходная матрица:");
        PrintMatrix(matrix);
        
         
        T[][] lowerTriangular = ConvertToLowerTriangular<T>(matrix);
        
        Console.WriteLine("\nНижнетреугольная матрица (ступенчатый массив):");
        PrintJaggedArray(lowerTriangular);
    }

     
    static T[,] CreateRandomMatrix<T>(int size, double minVal, double maxVal)
    {
        Random rand = new Random();
        T[,] matrix = new T[size, size];
        
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                double val = rand.NextDouble() * (maxVal - minVal) + minVal;
                matrix[i, j] = (T)Convert.ChangeType(
                    typeof(T) == typeof(int) ? Math.Round(val) : val, 
                    typeof(T));
            }
        }
        
        return matrix;
    }

     
    static T[][] ConvertToLowerTriangular<T>(T[,] matrix) where T : struct
    {
        int size = matrix.GetLength(0);
        T[][] result = new T[size][];

        
        
//        T[,] tempMatrix = (T[,])matrix.Clone();
        
        // for (int j = 0; j < size; j++)
        // {
        //      
        //     int pivotRow = j;
        //     for (int i = j; i < size; i++)
        //     {
        //         if (!IsZero(tempMatrix[i, j]))
        //         {
        //             pivotRow = i;
        //             break;
        //         }
        //     }
        //     
        //     if (IsZero(tempMatrix[pivotRow, j])) continue;
        //     
        //      
        //     if (pivotRow != j)
        //     {
        //         for (int k = 0; k < size; k++)
        //         {
        //             T temp = tempMatrix[j, k];
        //             tempMatrix[j, k] = tempMatrix[pivotRow, k];
        //             tempMatrix[pivotRow, k] = temp;
        //         }
        //     }
        //     
        //      
        //     T pivot = tempMatrix[j, j];
        //     for (int k = j; k < size; k++)
        //     {
        //         tempMatrix[j, k] = Divide(tempMatrix[j, k], pivot);
        //     }
        //     
        //      
        //     for (int i = 0; i < j; i++)
        //     {
        //         T factor = tempMatrix[i, j];
        //         for (int k = j; k < size; k++)
        //         {
        //             tempMatrix[i, k] = Subtract(tempMatrix[i, k], Multiply(tempMatrix[j, k], factor));
        //         }
        //     }
        // }

        for (int i = 0; i < size; i++)
        {
            result[i] = new T[i + 1];
            for (int j = 0; j <= i; j++)
            {
                
                result[i][j] = matrix[i, j];
                //result[i][j] = (T)Convert.ChangeType(tempMatrix[i, j], typeof(T));
            }
        }
        
        return result;
    }

     
    static bool IsZero<T>(T value)
    {
        return Math.Abs(Convert.ToDouble(value)) < 1e-10;
    }

    static T Divide<T>(T a, T b)
    {
        return (T)Convert.ChangeType(Convert.ToDouble(a) / Convert.ToDouble(b), typeof(T));
    }

    static T Multiply<T>(T a, T b)
    {
        return (T)Convert.ChangeType(Convert.ToDouble(a) * Convert.ToDouble(b), typeof(T));
    }

    static T Subtract<T>(T a, T b)
    {
        return (T)Convert.ChangeType(Convert.ToDouble(a) - Convert.ToDouble(b), typeof(T));
    }

     
    static void PrintMatrix<T>(T[,] matrix)
    {
        int size = matrix.GetLength(0);
        
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                Console.Write($"{matrix[i, j],10:F2} ");
            }
            Console.WriteLine();
        }
    }

    static void PrintJaggedArray<T>(T[][] jaggedArray)
    {
        for (int i = 0; i < jaggedArray.Length; i++)
        {
            for (int j = 0; j < jaggedArray[i].Length; j++)
            {
                Console.Write($"{jaggedArray[i][j],10:F2} ");
            }
            Console.WriteLine();
        }
    }
}