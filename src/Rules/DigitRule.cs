
public class DigitRule : Rule
{
    public DigitRule() : base(RuleFactory.GetName(Rule.Types.DIGIT))
    {
        Type = TokenType.Digit;
    }

    public override bool Matches(Stack<Index> indices)
    {
        throw new NotImplementedException();
    }

    public override ParseResult Parse(Stack<Index> indices)
    {
        throw new NotImplementedException();
    }
}