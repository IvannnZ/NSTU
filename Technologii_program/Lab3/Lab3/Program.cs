using System;

class Program
{
    static void Main(string[] args)
    {
        MyArrayWrapper Z = new MyArrayWrapper(15);
        MyArrayWrapper D = new MyArrayWrapper(15);


        Console.WriteLine("Z:");
        for (int i = 0; i < Z.Length; i++)
        {
            if (i % 2 != 0)
            {
                Z[i] = Math.Pow(i, 2) + 1;
            }
            else
            {
                Z[i] = 2 * i - 1;
            }

            Console.WriteLine($"Z[{i}] = {Z[i]}");
        }


        Console.WriteLine("\nD:");
        for (int i = 0; i < D.Length; i++)
        {
            if (Z[i] < 2.5)
            {
                D[i] = 2.5 * Z[i];
            }
            else
            {
                D[i] = Z[i] / 2.5;
            }

            Console.WriteLine($"D[{i}] = {D[i]}");
        }
    }
}


class MyArrayWrapper
{
    private double[] array;

    public MyArrayWrapper(int size)
    {
        array = new double[size];
    }


    public double this[int index]
    {
        get
        {
            if (index < 0 || index >= array.Length)
                throw new IndexOutOfRangeException("Index out of range");
            return array[index];
        }
        set
        {
            if (index < 0 || index >= array.Length)
                throw new IndexOutOfRangeException("Index out of range");
            array[index] = value;
        }
    }

    public int Length
    {
        get { return array.Length; }
    }
}