
public class IntegerLiteralRule : Rule
{
    public IntegerLiteralRule() : base(RuleFactory.GetName(Rule.Types.INTEGER_LITERAL)) { }

    public override bool Matches(Stack<Index> indices)
    {
        throw new NotImplementedException();
    }

    public override ParseResult Parse(Stack<Index> indices)
    {
        throw new NotImplementedException();
    }
}
