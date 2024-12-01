


internal static class RuleFactory
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

    internal static Rule Create(string name)
    {
        Rule rule = default;
        if (name.StartsWith('<') && name.EndsWith('>'))
        {
            name = name.Parse();
            switch (name)
            {
                case Rule.Types.DECLARATION:
                    // rule = new()
                    // {
                    //     Type = TokenType.Program
                    // };

                    break;
                case Rule.Types.EXPRESSION:
                    // rule = new()
                    // {
                    //     Type = TokenType.Expression
                    // };

                    break;
                case Rule.Types.OPERATOR:
                    // rule = new()
                    // {
                    //     Type = TokenType.Operator
                    // };

                    break;
                case Rule.Types.LITERAL:
                    // rule = new()
                    // {
                    //     Type = TokenType.Literal
                    // };

                    break;
                case Rule.Types.INTEGER_LITERAL:
                    // rule = new()
                    // {
                    //     Type = TokenType.IntegerLiteral
                    // };

                    break;
                case Rule.Types.FLOAT_LITERAL:
                    // rule = new()
                    // {
                    //     Type = TokenType.FloatLiteral
                    // };

                    break;
                case Rule.Types.EXPONENT:
                    // rule = new()
                    // {
                    //     Type = TokenType.Exponent
                    // };

                    break;
                case Rule.Types.TYPE:
                    // rule = new()
                    // {
                    //     Type = TokenType.Type
                    // };

                    break;
                case Rule.Types.IDENTIFIER:
                    // rule = new()
                    // {
                    //     Type = TokenType.Identifier
                    // };

                    break;
                case Rule.Types.LETTER:
                    rule = new LetterRule();

                    break;
                case Rule.Types.DIGIT:
                    rule = new DigitRule();

                    break;
                default:
                    //  error condition
                    break;
            }
        }
        if (rule != null)
        {
        }

        return rule;
    }

    internal static string GetName(string tag)
    {
        return nameLookup[tag];
    }

    private static string Parse(this string tag)
    {
        //  valid name
        return tag
            .Replace("<", string.Empty)
            .Replace(">", string.Empty);
    }
}
