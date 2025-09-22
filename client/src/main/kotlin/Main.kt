package edu.ntnu.eliasei.smarttv

import java.net.Socket

fun main(args: Array<String>)
{
    if (args.size < 2)
    {
        println("Usage: client <host> <port>");
        return;
    }

    val host = args[0];
    val port = args[1].toInt();

    Socket(host, port).use { socket ->
        val reader = socket.getInputStream().bufferedReader();
        val writer = socket.getOutputStream().bufferedWriter();

        println("Connected to SmartTV at $host:$port");

        while (true)
        {
            print("> ");
            System.out.flush();

            val input = readlnOrNull() ?: run {
                println("No input, exiting loop")
                break
            }
            writer.write(input + "\n")
            writer.flush()

            val response = reader.readLine()
            if (response == null) {
                println("Server closed connection")
                break
            }
            println("TV: $response")

        }
    }
}
