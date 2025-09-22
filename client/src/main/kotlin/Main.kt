package edu.ntnu.eliasei.smarttv

import java.net.Socket

fun main(args: Array<String>)
{
    if (args.size < 2)
    {
        println("Usage: client <host> <port>");
        return;
    }

    val connection = RemoteConnection()
    connection.connect(args[0], args[1].toInt())

    println("Connected to ${args[0]}:${args[1]}")

    while (true)
    {
        print("> ")
        System.out.flush();

        val input = readlnOrNull() ?: break
        connection.send(input)

        Thread.sleep(100) // For demo purposes
        var msg = connection.pollMessage()
        while (msg != null)
        {
            println("TV: $msg")
            msg = connection.pollMessage()
        }
    }
}
