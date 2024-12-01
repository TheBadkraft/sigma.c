

public abstract class Rule
{
    internal static class Types
    {
        internal const string DECLARATION = "declaration";
        internal const string EXPRESSION = "expression";
        internal const string OPERATOR = "operator";
        internal const string LITERAL = "literal";
        internal const string INTEGER_LITERAL = "integer_literal";
        internal const string FLOAT_LITERAL = "float_literal";
        internal const string EXPONENT = "exponent";
        internal const string TYPE = "type";
        internal const string IDENTIFIER = "identifier";
        internal const string LETTER = "letter";
        internal const string DIGIT = "digit";

    }

    protected Rule(string name) => Name = name;

    internal string Name { get; init; }
    internal TokenType Type { get; init; }

    public abstract bool Matches(Stack<Index> indices);
    public abstract ParseResult Parse(Stack<Index> indices);
}
