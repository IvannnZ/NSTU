using System;

// Базовый абстрактный класс Товар
public abstract class Product
{
    public string Name { get; set; }
    public float Price { get; set; }
    public string Barcode { get; set; }
    
    // Конструктор
    public Product(string name, float price, string barcode)
    {
        Name = name;
        Price = price;
        Barcode = barcode;
    }
    
    // Абстрактный метод для получения информации о продукте
    public abstract void DisplayInfo();
    
    // Виртуальный метод для применения скидки
    public virtual void ApplyDiscount(float percent)
    {
        if (percent > 0 && percent <= 100)
        {
            Price *= (1 - percent / 100);
            Console.WriteLine($"Применена скидка {percent}%. Новая цена: {Price:C}");
        }
    }
}

// Класс Игрушка (наследуется от Товара)
public class Toy : Product
{
    public string Material { get; set; }
    public int AgeLimit { get; set; }
    
    public Toy(string name, float price, string barcode, string material, int ageLimit) 
        : base(name, price, barcode)
    {
        Material = material;
        AgeLimit = ageLimit;
    }
    
    // Переопределение абстрактного метода
    public override void DisplayInfo()
    {
        Console.WriteLine($"Игрушка: {Name}");
        Console.WriteLine($"Материал: {Material}");
        Console.WriteLine($"Возрастное ограничение: {AgeLimit}+");
        Console.WriteLine($"Цена: {Price:C}");
        Console.WriteLine($"Штрих-код: {Barcode}");
    }
    
    // Переопределение виртуального метода (игрушки могут иметь максимальную скидку 30%)
    public override void ApplyDiscount(float percent)
    {
        if (percent > 30)
        {
            Console.WriteLine("Максимальная скидка на игрушки - 30%");
            percent = 30;
        }
        base.ApplyDiscount(percent);
    }
}

// Класс Продукт (наследуется от Товара)
public class FoodProduct : Product
{
    public DateTime ExpirationDate { get; set; }
    public int Calories { get; set; }
    
    public FoodProduct(string name, float price, string barcode, DateTime expirationDate, int calories) 
        : base(name, price, barcode)
    {
        ExpirationDate = expirationDate;
        Calories = calories;
    }
    
    public override void DisplayInfo()
    {
        Console.WriteLine($"Продукт: {Name}");
        Console.WriteLine($"Калории: {Calories} ккал/100г");
        Console.WriteLine($"Срок годности: {ExpirationDate:dd.MM.yyyy}");
        Console.WriteLine($"Цена: {Price:C}");
        Console.WriteLine($"Штрих-код: {Barcode}");
    }
    
    // Новый метод для проверки срока годности
    public bool IsExpired()
    {
        return DateTime.Now > ExpirationDate;
    }
}

// Класс Молочный продукт (наследуется от Продукта)
public class DairyProduct : FoodProduct
{
    public double FatContent { get; set; } // процент жирности
    public string Origin { get; set; } // происхождение (коровье, козье и т.д.)
    
    public DairyProduct(string name, float price, string barcode, 
                       DateTime expirationDate, int calories, 
                       double fatContent, string origin) 
        : base(name, price, barcode, expirationDate, calories)
    {
        FatContent = fatContent;
        Origin = origin;
    }
    
    public override void DisplayInfo()
    {
        Console.WriteLine($"Молочный продукт: {Name}");
        Console.WriteLine($"Тип молока: {Origin}");
        Console.WriteLine($"Жирность: {FatContent}%");
        Console.WriteLine($"Калории: {Calories} ккал/100г");
        Console.WriteLine($"Срок годности: {ExpirationDate:dd.MM.yyyy}");
        Console.WriteLine($"Цена: {Price:C}");
        Console.WriteLine($"Штрих-код: {Barcode}");
    }
    
    // Переопределение виртуального метода (на молочные продукты скидка больше, если срок годности близок)
    public override void ApplyDiscount(float percent)
    {
        // Если до конца срока годности осталось меньше 3 дней - увеличиваем скидку на 20%
        if ((ExpirationDate - DateTime.Now).TotalDays < 3)
        {
            percent += 20;
            Console.WriteLine("Скидка увеличена, так как срок годности заканчивается!");
        }
        
        base.ApplyDiscount(percent);
    }
}

 
public interface ITestable
{
    void Test();
}

 
public class ConstructionToy : Toy, ITestable
{
    public int PartsCount { get; set; }
    
    public ConstructionToy(string name, float price, string barcode, 
                          string material, int ageLimit, int partsCount) 
        : base(name, price, barcode, material, ageLimit)
    {
        PartsCount = partsCount;
    }
    
    public override void DisplayInfo()
    {
        base.DisplayInfo();
        Console.WriteLine($"Количество деталей: {PartsCount}");
    }
    
    // Реализация метода интерфейса
    public void Test()
    {
        Console.WriteLine($"Тестируем конструктор '{Name}': проверяем все {PartsCount} деталей...");
        Console.WriteLine("Тестирование завершено!");
    }
}

class Program
{
    static void Main(string[] args)
    {
        // Создаем объекты разных классов
        Toy doll = new Toy("Кукла Барби", 1999.99f, "123456789012", "пластик", 3);
        FoodProduct bread = new FoodProduct("Хлеб Бородинский", 89.50f, "234567890123", 
                                          DateTime.Now.AddDays(2), 250);
        DairyProduct milk = new DairyProduct("Молоко Простоквашино", 99.90f, "345678901234", 
                                           DateTime.Now.AddDays(5), 52, 3.2, "коровье");
        ConstructionToy lego = new ConstructionToy("LEGO Creator", 3499.99f, "456789012345", 
                                                 "пластик", 6, 450);
        
        // Демонстрация полиморфизма
        Product[] products = { doll, bread, milk, lego };
        
        Console.WriteLine("=== Информация о товарах ===");
        foreach (var product in products)
        {
            product.DisplayInfo();
            Console.WriteLine();
        }
        
        // Применение скидок
        Console.WriteLine("\n=== Применение скидок ===");
        doll.ApplyDiscount(25); // обычная скидка
        doll.ApplyDiscount(40); // попытка скидки больше 30%
        
        milk.ApplyDiscount(10); // скидка будет увеличена, так как срок годности близок
        
        // Тестирование игрушки через интерфейс
        Console.WriteLine("\n=== Тестирование игрушки ===");
        if (lego is ITestable testableToy)
        {
            testableToy.Test();
        }
        
        // Проверка срока годности
        Console.WriteLine("\n=== Проверка срока годности ===");
        Console.WriteLine($"Хлеб просрочен? {bread.IsExpired()}");
    }
}