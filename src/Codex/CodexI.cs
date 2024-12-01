

internal static class CodexI
{
    internal unsafe static IEnumerable<Index> IndexDefinition(this Codex codex)
    {
        List<Index> list = new();
        char[] def = codex.Definition.ToArray();

        fixed (char* p = codex.Definition)
        {
            char* pos = p;
            char* start = pos;
            char* eos = p + codex.Definition.Length - 1;
            long len = pos - p;
            Index index = default;

            while (pos != eos)
            {
                start = pos;
                switch (pos[0])
                {
                    case ' ':
                        ++pos;
                        continue;

                    case '\n':
                        // Console.WriteLine("CRLF");
                        ++pos;
                        index = new()
                        {
                            Segment = new(def, (int)(start - p), 1)
                        };

                        break;
                    default:
                        // Console.WriteLine("WORD");
                        pos = GetWord(start, eos);
                        len = pos - start;
                        index = new()
                        {
                            Segment = new(def, (int)(start - p), (int)(len))
                        };

                        break;
                }

                list.Add(index);
            }
        }

        list.Reverse();
        return list;
    }
    internal static void LoadInfo(this Codex codex)
    {
        //  read header info - all lines starting with '##'
        //  header info ends with '##\n'
        var stack = codex.Index.Indices;
        var release = string.Empty;
        var version = string.Empty;

        while (stack.Peek().Word() == "##")
        {
            stack.Pop();
            if (stack.Peek().Word() == "\n")
            {
                //  done with header info
                break;
            }

            Index index = stack.Pop();
            switch (index.Word())
            {
                case "Release:":
                    release = stack.Pop().Word().Replace('_', ' ');

                    break;
                case "Version:":
                    version = stack.Pop().Word();

                    break;
            }

            //  clear new line
            stack.Pop();
        }

        codex.RuleSet = new()
        {
            Release = release,
            Version = Version.Parse(version)
        };
    }
    internal static bool LoadRuleSet(this Codex codex)
    {
        bool retOk = true;
        var stack = codex.Index.Indices;
        var rule = new List<Index>();

        while (stack.TryPop(out Index index))
        {
            // var word = index.Word();
            // Console.Write($"{word}{(word == "\n" ? "" : "\n")}");
            while (index.Segment[0] != '\n')
            {
                rule.Add(index);
                if (!stack.TryPop(out index))
                {
                    break;
                }
            }

            if (rule.Any())
            {
                //  creates and adds rule to ruleset
                codex.RuleSet.Add([.. rule]);
                rule.Clear();
            }
        }

        return retOk;
    }

    private static unsafe char* GetWord(char* beg, char* eos)
    {
        char* pos = beg;
        while (pos != eos && pos[0] != ' ' && pos[0] != '\n')
        {
            ++pos;
        }

        return pos;
    }
}