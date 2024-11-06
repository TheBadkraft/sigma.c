using static System.Console;

// See https://aka.ms/new-console-template for more information
WriteLine("Sigma.C Command Line Interpreter!");
WriteLine($"\targs: {args.Length}");

foreach (var arg in args)
{
    WriteLine($"\targ: {arg}");
}