using System;
using System.Text.RegularExpressions;

class Program
{
    static void Main(string[] args)
    {
        while (true)
        {
            Console.Write("> ");
            string input = Console.ReadLine();
            
            if (input.ToLower() == "e")
                break;
                
            try
            {
                double result = CalculateExpression(input);
                Console.WriteLine($"Результат: {result}");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Ошибка: {ex.Message}");
            }
        }
    }

    static double CalculateExpression(string expression)
    {
        
        expression = Regex.Replace(expression, @"\s+", "");
        
        while (true)
        {
            Match match = Regex.Match(expression, @"\(([^()]+)\)");
            if (!match.Success)
                break;
            double value = CalculateSimpleExpression(match.Groups[1].Value);
            expression = expression.Replace(match.Value, value.ToString());
        }
        
        return CalculateSimpleExpression(expression);
    }

    static double CalculateSimpleExpression(string expression)
    {
        //умножение и деление
        expression = ProcessOperations(expression, @"(\d+\.?\d*)([*/])(-?\d+\.?\d*)");
        //сложение и вычитание
        expression = ProcessOperations(expression, @"(\d+\.?\d*)([+-])(-?\d+\.?\d*)");
        return double.Parse(expression);
    }

    static string ProcessOperations(string expression, string pattern)
    {
        Regex regex = new Regex(pattern);
        
        while (true)
        {
            Match match = regex.Match(expression);
            if (!match.Success)
                break;
            double left = double.Parse(match.Groups[1].Value);
            string op = match.Groups[2].Value;
            double right = double.Parse(match.Groups[3].Value);
            double result = 0;
            
            switch (op)
            {
                case "*":
                    result = left * right;
                    break;
                case "/":
                    if (right == 0)
                        throw new DivideByZeroException("Деление на ноль");
                    result = left / right;
                    break;
                case "+":
                    result = left + right;
                    break;
                case "-":
                    result = left - right;
                    break;
            }
            expression = expression.Replace(match.Value, result.ToString());
        }
        
        return expression;
    }
}