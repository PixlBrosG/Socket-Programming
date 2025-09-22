package edu.ntnu.eliasei.smarttv

import java.io.BufferedReader
import java.io.BufferedWriter
import java.net.Socket
import java.util.concurrent.Executors
import java.util.concurrent.LinkedBlockingQueue

class RemoteConnection
{
    private var socket: Socket? = null
    private var reader: BufferedReader? = null
    private var writer: BufferedWriter? = null
    private val executor = Executors.newSingleThreadExecutor()

    val messages = LinkedBlockingQueue<String>()

    val isConnected: Boolean
        get() = socket?.isConnected == true && socket?.isClosed == false

    fun connect(host: String, port: Int)
    {
        if (isConnected) return
        socket = Socket(host, port)
        reader = socket!!.getInputStream().bufferedReader()
        writer = socket!!.getOutputStream().bufferedWriter()

        executor.submit {
            try
            {
                while (true)
                {
                    val line = reader?.readLine() ?: break
                    messages.put(line)
                }
            }
            catch (e: Exception)
            {
                messages.put("Disconnected: ${e.message}")
            }
            finally
            {
                disconnect()
            }
        }
    }

    fun send(command: String)
    {
        if (!isConnected) return
        try
        {
            writer?.write(command + "\n")
            writer?.flush()
            messages.put(">> $command")
        }
        catch (e: Exception)
        {
            messages.put("Failed to send command: ${e.message}")
            disconnect()
        }
    }

    fun pollMessage(): String?
    {
        return messages.poll()
    }

    fun disconnect() {
        try { reader?.close() } catch (_: Exception) {}
        try { writer?.close() } catch (_: Exception) {}
        try { socket?.close() } catch (_: Exception) {}
        reader = null
        writer = null
        socket = null
    }
}