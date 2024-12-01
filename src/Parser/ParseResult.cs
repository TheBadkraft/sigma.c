
public class ParseResult
{
    public bool Success { get; }
    public object Result { get; }
    public int ReadCount { get; }

    public ParseResult(bool success, object result, int tokensRead)
    {
        Success = success;
        Result = result;
        ReadCount = tokensRead;
    }
}
