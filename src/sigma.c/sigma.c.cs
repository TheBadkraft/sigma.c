using static System.Console;

// See https://aka.ms/new-console-template for more information

bool retStatus = SigmaC.Instance.Initialize(args.ToList(), out ITaskQueue taskQueue);
while (retStatus && taskQueue.MoveNext())
{
   SigTask sigTask = taskQueue.Current;
   retStatus = sigTask(SigmaC.Instance);
}