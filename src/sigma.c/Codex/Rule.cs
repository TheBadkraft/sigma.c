

public class Rule
{
    internal static class Types
    {

        internal const string PROGRAM = "PROG";
        internal const string FUNCTION = "FUNC";
        internal const string STATEMENT = "STMT";
        internal const string EXPRESSION = "EXPR";
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
                case Types.PROGRAM:
                    rule = new()
                    {
                        Type = NodeType.Program
                    };

                    break;
                case Types.FUNCTION:
                    rule = new()
                    {
                        Type = NodeType.Function
                    };

                    break;
                case Types.STATEMENT:
                    rule = new()
                    {
                        Type = NodeType.Statement
                    };

                    break;
                case Types.EXPRESSION:
                    rule = new()
                    {
                        Type = NodeType.Expression
                    };

                    break;
                default:
                    //  error condition
                    break;
            }
        }
        rule.SetName(name);

        return rule;
    }
}

internal static class RuleI
{
    static Dictionary<string, string> nameLookup = new() {
            { Rule.Types.PROGRAM, "PROGRAM" },
            { Rule.Types.FUNCTION, "FUNCTION"},
            { Rule.Types.STATEMENT, "STATEMENT"},
            { Rule.Types.EXPRESSION, "EXPRESSION" }
    };

    internal static string SetName(this Rule rule, string name)
    {
        //  valid name
        name = name
            .Replace("<", string.Empty)
            .Replace(">", string.Empty)
            .ToUpper();

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
