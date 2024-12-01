

public class FloatLiteralRule : Rule
{
    public FloatLiteralRule() : base(RuleFactory.GetName(Rule.Types.FLOAT_LITERAL)) { }

    public override bool Matches(Stack<Index> indices)
    {
        throw new NotImplementedException();
    }

    public override ParseResult Parse(Stack<Index> indices)
    {
        throw new NotImplementedException();
    }
}