using System.Text;

public delegate Logging Logger();

public static class LoggingI
{
    private const string VALID_FILE_CHARS = "a1b2c3d4e5f6g7h8i9j0kmnopqrstuvwxyz_A1B2C3D4E5F6G7H8I9J0KLMNPQRSTUVWXYZ_";
    private static readonly Encoding ENCODER = Encoding.UTF8;
    private static readonly Dictionary<StdStreams, StreamWriter> LOG_WRITERS = new() {
        { StdStreams.StdOut, null },
        { StdStreams.StdErr, null },
        { StdStreams.StdLog, null },
    };

    private static SigmaC SigmaC => SigmaC.Instance();

    internal static string GetLogDir(this Logging logging)
    {
        return logging.LogDir;
    }
    internal static string GetLogPath(this Logging logging)
    {
        return Path.Combine(SigmaC.WorkingDir(), logging.LogDir);
    }
    internal static bool CreateLogDir(this Logging logging, string logDir)
    {
        bool retOk = !string.IsNullOrEmpty(logDir);
        string path = string.Empty;

        if (retOk)
        {
            path = Path.Combine(SigmaC.WorkingDir(), logDir);
        }
        if (retOk && !Directory.Exists(path))
        {
            var dirInfo = Directory.CreateDirectory(path);
            retOk = dirInfo.Exists;
        }
        if (retOk)
        {
            logging.LogDir = logDir;
        }

        return retOk;
    }
    internal static bool CreateLogFile(this Logging logging)
    {
        return CreateLogFile(logging, GenerateLogName());
    }
    internal static bool CreateLogFile(this Logging logging, string logFile)
    {
        bool retOk = !string.IsNullOrEmpty(logFile);
        string path = logging.GetLogPath();

        if (retOk)
        {
            path = Path.Combine(path, logFile);
        }
        if (retOk && !File.Exists(path))
        {
            using (var fStream = File.Create(path, 0, FileOptions.None))
            {
                fStream.Write(ENCODER.GetBytes($"{DateTime.UtcNow}  *** Begin Sigma.C Compiler Logging ***\n"));
            }
            retOk = File.Exists(path);
        }
        if (retOk)
        {
            logging.LogFile = logFile;
        }

        return retOk;
    }
    internal static string GenerateLogName(this Logging logging)
    {
        return GenerateLogName();
    }

    internal static void SetLogger(StdStreams stdStream, Func<Stream> getStream, Action<TextWriter> setStream)
    {
        StreamWriter sw = new StreamWriter(getStream());
        sw.AutoFlush = true;

        if (setStream != null)
        {
            setStream(sw);
        }

        LOG_WRITERS[stdStream] = sw;
    }

    internal static void Log(this Logger logger, StdStreams stdType, string text)
    {
        if ((stdType & StdStreams.StdOut) == StdStreams.StdOut)
        {
            LOG_WRITERS[StdStreams.StdOut].Write(text);
            LOG_WRITERS[StdStreams.StdOut].Write('\n');
        }
        if ((stdType & StdStreams.StdErr) == StdStreams.StdErr)
        {
            LOG_WRITERS[StdStreams.StdErr]?.Write(text);
            LOG_WRITERS[StdStreams.StdErr]?.Write('\n');
        }
        if ((stdType & StdStreams.StdLog) == StdStreams.StdLog)
        {
            LOG_WRITERS[StdStreams.StdLog]?.WriteLine($"{DateTime.UtcNow}      {text}");
        }
    }
    internal static void Dispose(this Logger logger)
    {
        StreamWriter sw = LOG_WRITERS[StdStreams.StdOut];
        if (sw != null)
        {
            sw.Flush();
            sw.Close();
            sw.Dispose();
        }

        sw = LOG_WRITERS[StdStreams.StdErr];
        if (sw != null)
        {
            sw.Flush();
            sw.Close();
            sw.Dispose();
        }

        sw = LOG_WRITERS[StdStreams.StdLog];
        if (sw != null)
        {
            sw.Flush();
            sw.Close();
            sw.Dispose();
        }
    }
    private static string GenerateLogName()
    {
        var fid = ByteArrayI.New(16);
        fid.GenerateFrom(VALID_FILE_CHARS);

        return $"{ENCODER.GetString(fid)}.log";
    }
}
