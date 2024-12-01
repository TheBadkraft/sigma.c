

public delegate SigmaC SigC();
public delegate bool SigTask(SigC sigC);

public class SigmaC
{
   private static readonly string[] options = {
        "c", "tmplcfg"
    };

   private static readonly Lazy<SigmaC> _instance = new(() => new());

   public static SigC Instance => () => _instance.Value;

   internal Assembly Assembly { get; set; }
   internal string configFile
   {
      get;
      set;
   } = "./.data/sigc.config";
   internal SigCConfiguration Configuration
   {
      get;
      set;
   } = null;
   internal Logging Logging => Configuration?.Logging ?? new()
   {
      Log = false,
      LogDir = null,
      LogFile = null
   };

   internal string this[Option opt] => options[(int)opt];
   internal Logger Logger => () => Logging;
   internal enum Option
   {
      ConfigSource = 0,
      WriteCfgTmpl = 1,
   }
}

internal static class SigmaCI
{
   private const string DEFAULT_LOG_DIR = ".log";

   public static bool Initialize(this SigC handler, List<string> argList, out ITaskQueue taskQueue)
   {
      bool retOk = true;
      taskQueue = new TaskQueue();
      SigTask start = (h) =>
      {
         LoggingI.SetLogger(StdStreams.StdOut, Console.OpenStandardOutput, Console.SetOut);
         return h.Start();
      };
      taskQueue.Enqueue(start);

      /*
          * -c:<sigc.config>
          Source Sigma.C configuration file (sigc.config) if
          not in default location or default name.
          * --tmplcfg:<sigc.config>
          Create a full Sigma.C configuration template
      */
      if (argList.HasOption(SigmaC.Option.WriteCfgTmpl))
      {
         //  write config template and we're done ...
         SigTask writeCfg = (h) =>
         {
            SigmaC sigc = h();

            return new SigCConfiguration().WriteConfigTemplate(sigc.configFile);
         };
         taskQueue.Enqueue(writeCfg);

         goto done;
      }

      //  no CL args ... then ...
      taskQueue.Enqueue((h) =>
      {
         // load configuration
         SigmaC sigc = h();
         sigc.Configuration = new SigCConfiguration().LoadConfiguration(sigc.configFile);
         return sigc.Configuration != null;
      });
      taskQueue.Enqueue((h) =>
      {
         // configure
         SigmaC sigc = h();
         return sigc.Configure();
      });
      taskQueue.Enqueue((h) =>
      {
         // load codex
         SigmaC sigc = h();
         return sigc.LoadCodex();
      });

   done:
      taskQueue.Enqueue((h) =>
      {
         return h.Exit();
      });
      return retOk;
   }

   private static bool Start(this SigC handler)
   {
      LoggingI.Log(null, StdStreams.StdOut, "Sigma.C Command Line Compiler!");
      handler().Assembly = Assembly.GetExecutingAssembly();

      return true;
   }
   private static bool Exit(this SigC handler)
   {
      LoggingI.Log(null, StdStreams.StdOut, "\nFinished ");
      LoggingI.Dispose(null);
      return true;
   }
   internal static bool Configure(this SigmaC sigC)
   {
      bool retOk = true;

      if (sigC.IsLogging())
      {
         if (string.IsNullOrEmpty(sigC.LogDir()))
         {
            sigC.LogDir(DEFAULT_LOG_DIR);
         }
         sigC.Logging.CreateLogFile();
         Stream openLog()
         {
            var path = Path.Combine(sigC.LogDir(), sigC.LogFile());
            return File.Open(path, FileMode.Append);
         }
         LoggingI.SetLogger(StdStreams.StdLog, openLog, null);
      }

      return retOk;
   }
   internal static bool LoadCodex(this SigmaC sigc)
   {
      bool retOk = true;
      sigc.Logger.Log(StdStreams.StdLog, "Loading Sigma.C Language Codex");

      var parser = ParserI.New();
      retOk = parser.Initialize();

      return retOk;
   }

   internal static string WorkingDir(this SigmaC sigC)
   {
      return sigC.Configuration.WorkingDir;
   }
   internal static bool IsLogging(this SigmaC sigC)
   {
      return sigC.Logging.Log;
   }
   internal static string LogDir(this SigmaC sigC)
   {
      return sigC.Logging.LogDir;
   }
   internal static string LogDir(this SigmaC sigC, string logDir)
   {
      if (!sigC.Logging.CreateLogDir(logDir))
      {
         // TODO: output logging error - cannot create logging directory
      }

      return sigC.LogDir();
   }
   internal static string LogFile(this SigmaC sigC)
   {
      return sigC.Logging.LogFile;
   }
   internal static bool HasOption(this IEnumerable<string> list, SigmaC.Option option)
   {
      var opt = SigmaC.Instance()[option];

      return list.Any(o => o.Replace("-", string.Empty) == opt);
   }
   // TODO - implement & test
   internal static bool RemoveOption(this IList<string> list, SigmaC.Option option)
   {
      var opt = SigmaC.Instance()[option];

      return list.Remove(opt);
   }
}