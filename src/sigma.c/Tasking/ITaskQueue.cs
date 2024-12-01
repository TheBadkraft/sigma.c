

using System.Collections;

public interface ITaskQueue : IEnumerator<SigTask>
{
    void Enqueue(SigTask writeCfg);
}

internal class TaskQueue : ITaskQueue
{

    private Queue<SigTask> Queue { get; } = new();

    public SigTask Current { get; private set; }

    object IEnumerator.Current => Current;

    public void Enqueue(SigTask sigTask)
    {
        Queue.Enqueue(sigTask);
    }
    public bool MoveNext()
    {
        bool retOk = Queue.Any();

        if (retOk)
        {
            Current = Queue.Dequeue();
        }

        return retOk;
    }
    public void Reset()
    {
        //  not supported
    }

    public void Dispose()
    {
        //  ... ???
    }
}