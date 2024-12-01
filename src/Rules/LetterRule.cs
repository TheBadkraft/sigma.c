
public class LetterRule : Rule
{
    public char[] Letters { get; set; }
    public LetterRule() : base(RuleFactory.GetName(Rule.Types.LETTER))
    {
        Type = TokenType.Letter;
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
