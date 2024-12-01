

public static class ByteArrayI
{
    public static void Generate(this byte[] array)
    {
        new Random().NextBytes(array);
    }
    public static void GenerateFrom(this byte[] array, string source)
    {
        var rand = new Random();
        int i = 0;

        while (i < array.Length)
        {
            var r = rand.Next(byte.MinValue, source.Length - 1);
            array[i] = (byte)source[r];

            ++i;
        }
    }

    public static byte[] New(int length)
    {
        return new byte[length];
    }
}