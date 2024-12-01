
internal static class ParserI
{
    internal static Parser New()
    {
        return new();
    }

    internal static bool Initialize(this Parser parser)
    {
        SigmaC sigc = SigmaC.Instance();
        bool retOk = false;
        var resource = parser.Definition;

        retOk = LoadLanguage(sigc, resource, out Codex codex);
        if (retOk)
        {
            parser.Codex = codex;
            var stack = new Stack<Index>(parser.Codex.IndexDefinition());
            parser.Codex.Index = new()
            {
                Indices = stack
            };

            retOk = parser.Codex.Index.Indices.Any();
        }
        if (retOk)
        {
            parser.Codex.LoadInfo();
            sigc.Logger.Log(StdStreams.StdLog, $"Loaded Sigma.C[{parser.Codex.RuleSet.Release}] v.{parser.Codex.RuleSet.Version}");
            retOk = parser.Codex.LoadRuleSet();
        }

        return retOk;
    }

    private static bool LoadLanguage(SigmaC sigc, string resource, out Codex codex)
    {
        bool retOk = false;
        string langDef = string.Empty;
        var resources = sigc.Assembly.GetManifestResourceNames();

        codex = null;
        using (var stream = sigc.Assembly.GetManifestResourceStream(resource))
        using (var reader = new StreamReader(stream))
        {
            langDef = reader.ReadToEnd();
            retOk = !string.IsNullOrEmpty(langDef);
        }

        if (retOk)
        {
            sigc.Logger.Log(StdStreams.StdLog, $"Found Language Definition ({resource})");
            codex = new()
            {
                Definition = langDef
            };

        }
        else
        {
            sigc.Logger.Log(StdStreams.StdErr | StdStreams.StdLog, $"ERROR [NOT FOUND]: Language Definition ({resource})");
        }


        return retOk;
    }
}
