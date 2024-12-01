

public class Rule
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

    internal string Name { get; set; }
    internal NodeType Type { get; init; }

    internal static Rule Create(string name)
    {
        Rule rule = default;
        if (name.StartsWith('<') && name.EndsWith('>'))
        {
            switch (name)
            {
                case Types.DECLARATION:
                    rule = new()
                    {
                        Type = NodeType.Program
                    };

                    break;
                case Types.EXPRESSION:
                    rule = new()
                    {
                        Type = NodeType.Expression
                    };

                    break;
                case Types.OPERATOR:
                    rule = new()
                    {
                        Type = NodeType.Operator
                    };

                    break;
                case Types.LITERAL:
                    rule = new()
                    {
                        Type = NodeType.Literal
                    };

                    break;
                case Types.INTEGER_LITERAL:
                    rule = new()
                    {
                        Type = NodeType.IntegerLiteral
                    };

                    break;
                case Types.FLOAT_LITERAL:
                    rule = new()
                    {
                        Type = NodeType.FloatLiteral
                    };

                    break;
                case Types.EXPONENT:
                    rule = new()
                    {
                        Type = NodeType.Exponent
                    };

                    break;
                case Types.TYPE:
                    rule = new()
                    {
                        Type = NodeType.Type
                    };

                    break;
                case Types.IDENTIFIER:
                    rule = new()
                    {
                        Type = NodeType.Identifier
                    };

                    break;
                case Types.LETTER:
                    rule = new()
                    {
                        Type = NodeType.Letter
                    };

                    break;
                case Types.DIGIT:
                    rule = new()
                    {
                        Type = NodeType.Digit
                    };

                    break;
                default:
                    //  error condition
                    break;
            }
        }
        else if (name.StartsWith("##"))
        {
            return rule;
        }

        rule.SetName(name);

        return rule;
    }
}

internal static class RuleI
{
    static Dictionary<string, string> nameLookup = new() {
            { Rule.Types.DECLARATION, "PROGRAM" },
            { Rule.Types.EXPRESSION, "EXPRESSION" },
            { Rule.Types.OPERATOR, "OPERATOR"},
            { Rule.Types.LITERAL, "LITERAL"},
            { Rule.Types.INTEGER_LITERAL, "INT_LITERAL"},
            { Rule.Types.FLOAT_LITERAL, "FLT_LITERAL"},
            { Rule.Types.EXPONENT, "EXPONENT"},
            { Rule.Types.TYPE, "TYPE"},
            { Rule.Types.IDENTIFIER, "IDENTIFIER"},
            { Rule.Types.LETTER, "LETTER"},
            { Rule.Types.DIGIT, "DIGIT"},
    };

    internal static string SetName(this Rule rule, string name)
    {
        //  valid name
        name = name
            .Replace("<", string.Empty)
            .Replace(">", string.Empty);

        return nameLookup[name];
    }

    internal static void Compose(this Rule rule, ArraySegment<Index> nodes)
    {
        var stack = new Stack<Index>(nodes);
        while (stack.TryPop(out Index n))
        {
            var type = n.GetSymbolType();
        }
    }
}
