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

    initCommands()

    if (noGui)
        runCli(host, port)
    else
        runGui(host, port)
}

fun runCli(host: String, port: Int) {
    val conn = RemoteConnection()
    conn.connect(host, port)

    println("Connected to SmartTV at $host:$port")

    while (true) {
        print("> ")
        System.out.flush()
        val input = readlnOrNull() ?: break
        val parts = input.split(" ")
        val cmdName = parts[0]
        val args = parts.drop(1)

        if (cmdName.uppercase() == "EXIT" || cmdName.uppercase() == "QUIT") {
            println("Exiting...")
            break
        }

        if (cmdName.uppercase() == "HELP") {
            println("Available commands:")
            for (cmd in CommandRegistry.all()) {
                println(" - ${cmd.name}: ${cmd.description}")
            }
            println(" - EXIT or QUIT: Exit the client")
            continue
        }

        val cmd = CommandRegistry.get(cmdName)
        if (cmd == null) {
            println("Unknown command: $cmdName")
            continue
        }

        val message = cmd.build(args)
        conn.send(message)

        Thread.sleep(100)
        var msg = conn.pollMessage()
        while (msg != null) {
            println("TV: $msg")
            msg = conn.pollMessage()
        }
    }
}

