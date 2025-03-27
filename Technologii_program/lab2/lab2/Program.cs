using System;
using System.Collections.Generic;

class Program
{
    static void Main(string[] args)
    {
        List<string> strings = new List<string> { };

        while (true)
        {
            Console.WriteLine("\nlist str:");
            for (int i = 0; i < strings.Count; i++)
            {
                Console.WriteLine($"{i + 1}. {strings[i]}");
            }

            Console.WriteLine("\nChoise action:");
            Console.WriteLine("1. remake str from lis");
            Console.WriteLine("2. add new_str");
            Console.WriteLine("3. exit");

            string choice = Console.ReadLine();

            switch (choice)
            {
                case "1":
                    ProcessStringFromList(strings);
                    break;
                case "2":
                    AddNewString(strings);
                    break;
                case "3":
                    return;
                default:
                    Console.WriteLine("Incorrect, try again");
                    break;
            }
        }
    }

    static void ProcessStringFromList(List<string> strings)
    {
        if (strings.Count == 0)
        {
            Console.WriteLine("list is empty, add str");
            return;
        }

        Console.Write("enter nam str for remake: ");
        if (int.TryParse(Console.ReadLine(), out int index) && index > 0 && index <= strings.Count)
        {
            string original = strings[index - 1];
            string processed = CapitalizeFirstLetters(original);

            Console.WriteLine("\nresault:");
            Console.WriteLine($"original: {original}");
            Console.WriteLine($"remake: {processed}");
        }
        else
        {
            Console.WriteLine("incorrect num_str");
        }
    }

    static void AddNewString(List<string> strings)
    {
        Console.Write("enter new_str: ");
        string newString = Console.ReadLine();

        if (!string.IsNullOrWhiteSpace(newString))
        {
            strings.Add(newString);
        }
    }

    static string CapitalizeFirstLetters(string input)
    {
        if (string.IsNullOrEmpty(input))
            return input;

        char[] chars = input.ToCharArray();
        bool newWord = true;

        for (int i = 0; i < chars.Length; i++)
        {
            if (newWord && char.IsLetter(chars[i]))
            {
                chars[i] = char.ToUpper(chars[i]);
                newWord = false;
            }
            else if (chars[i] == ' ')
            {
                newWord = true;
            }
        }

        return new string(chars);
    }
}