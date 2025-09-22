package edu.ntnu.eliasei.smarttv

fun main(args: Array<String>)
{
    if (args.contains("--help") || args.size < 2)
    {
        println("Usage: client [--no-gui] <host> <port>")
        return
    }

    val noGui = args.contains("--no-gui")
    val host = args[args.size - 2]
    val port = args.last().toInt()

    if (noGui)
        runCli(host, port)
    else
        runGui(host, port)
}

fun runCli(host: String, port: Int)
{
    val conn = RemoteConnection()
    conn.connect(host, port)

    println("Connected to SmartTV at $host:$port")

    while (true)
    {
        print("> ")
        System.out.flush()

        val input = readlnOrNull() ?: break
        conn.send(input)

        Thread.sleep(100)
        var msg = conn.pollMessage()
        while (msg != null)
        {
            println("TV: $msg")
            msg = conn.pollMessage()
        }
    }
}
