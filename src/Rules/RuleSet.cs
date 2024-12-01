

public class RuleSet
{
    private readonly Dictionary<string, Rule> ruleset = new();

    internal string Release { get; init; }
    internal Version Version { get; init; }

    internal void Add(Index[] indices)
    {
        var i = 0;

        /*
            ASSUMPTIONS:
                - first member is rule name
                - second member is "="
                - remaining members are valid composition
        */
        var rule = RuleFactory.Create(indices[i++].Word());
        ++i;
        // rule.Compose(indices.TakeWhile((n, e) => e > i).ToArray());    

        if (rule != null)
        {
            ruleset.Add(rule.Name, rule);
        }

    }
}
