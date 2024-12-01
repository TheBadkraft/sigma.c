using System.Text.Json.Serialization;

public class Logging
{
    [JsonPropertyName("log")]
    public bool Log { get; set; }
    [JsonPropertyName("log_dir")]
    public string LogDir { get; set; }
    [JsonIgnore]
    internal string LogFile { get; set; }
}
