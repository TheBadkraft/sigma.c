using System.Text.Json;
using System.Text.Json.Serialization;

public class SigCConfiguration
{
   [JsonPropertyName("working_dir")]
   public string WorkingDir { get; set; }
   [JsonPropertyName("source")]
   public IReadOnlyCollection<string> Source { get; set; }
   [JsonPropertyName("logging")]
   public Logging Logging { get; set; }
}

public static class SigCConfigurationI
{
   private static readonly JsonSerializerOptions jsOptions = new()
   {
      WriteIndented = true
   };

   /// <summary>
   /// Writes an empty json configuration 
   /// </summary>
   /// <param name="config"></param>
   /// <param name="destFile"></param>
   /// <returns></returns>
   internal static bool WriteConfigTemplate(this SigCConfiguration config, string destFile)
   {
      Console.Write($"Writing output configuration template: {destFile}");

      var json = JsonSerializer.Serialize(config, jsOptions);
      bool retOk = json.Length > 0;

      using (var fStream = new FileStream(destFile, FileMode.Create))
      using (var writer = new StreamWriter(fStream))
      {
         writer.Write(json);
      }

      var fileLen = new FileInfo(destFile).Length;
      retOk = json.Length == fileLen;

      Console.WriteLine($" ({(retOk ? 0 : 1)}) :: {fileLen} bytes");

      return retOk;
   }
   /// <summary>
   /// Loads compiler configuration
   /// </summary>
   /// <param name="config"></param>
   /// <param name="sourceFile"></param>
   /// <returns></returns>
   internal static SigCConfiguration LoadConfiguration(this SigCConfiguration config, string sourceFile)
   {
      var json = string.Empty;

      using (var fStream = new FileStream(sourceFile, FileMode.Open))
      using (var reader = new StreamReader(fStream))
      {
         json = reader.ReadToEnd();
      }

      if (!string.IsNullOrEmpty(json))
      {
         var scCfg = JsonSerializer.Deserialize<SigCConfiguration>(json);
         config.WorkingDir = scCfg.WorkingDir;
         config.Source = scCfg.Source;
         config.Logging = scCfg.Logging;
      }

      return config;
   }
}
