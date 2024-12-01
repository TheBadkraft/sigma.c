

public readonly struct Index
{
    public char this[int i] => Segment[i];
    public ArraySegment<char> Segment { get; internal init; }
}

public static class IndexI
{
    internal static string Word(this Index index)
    {
        return string.Join(string.Empty, index.Segment);
    }

    internal static bool Last(this Index index, Func<char, bool> predicate)
    {
        int i = index.Segment.Count - 1;
        char last = index[i];

        return predicate(last);
    }

    internal static SymbolType GetSymbolType(this Index index)
    {
        SymbolType type = SymbolType.Unknown;
        switch (index[0])
        {
            case '<':
                if (index.Last(n => n == '>'))
                {
                    //   is valid non-terminal
                    type = SymbolType.NonTerm;
                }

                break;
            case '"':
                if (index.Last(n => n == '"'))
                {
                    //  is valid literal
                    type = SymbolType.Literal;
                }

                break;
            default:
                //  nothing else to do here (yet)

                break;
        }

        return type;
    }
}

public enum SymbolType
{
    Unknown = 0,
    NonTerm = 1,
    Literal = 2
}
