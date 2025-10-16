package edu.ntnu.eliasei.smarttv

import java.io.BufferedReader
import java.io.BufferedWriter
import java.net.InetSocketAddress
import java.net.Socket
import java.util.concurrent.Executors
import java.util.concurrent.LinkedBlockingQueue

class RemoteConnection
{
    private var socket: Socket? = null
    private var reader: BufferedReader? = null
    private var writer: BufferedWriter? = null
    private val executor = Executors.newSingleThreadExecutor()

    @Volatile private var running = false

    val messages = LinkedBlockingQueue<String>()

    val isConnected: Boolean
        get() = socket?.isConnected == true && socket?.isClosed == false

    fun connect(host: String, port: Int)
    {
        if (isConnected) return

        executor.submit {
            try
            {
                val address = InetSocketAddress(host, port)
                socket = Socket()
                socket!!.connect(address, 2000)
                reader = socket!!.getInputStream().bufferedReader()
                writer = socket!!.getOutputStream().bufferedWriter()
                running = true

                while (running)
                {
                    val line = reader?.readLine() ?: break
                    messages.put(line)

                    if (line.startsWith("SERVER_OFFLINE"))
                    {
                        messages.put("[!] Server went offline: disconnecting...")
                        disconnect()
                    }
                }
            }
            catch (e: Exception)
            {
                messages.put("ERROR connect_failed: ${e.message}")
            }
            finally
            {
                disconnectInternal()
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

    fun disconnect()
    {
        running = false
        try
        {
            socket?.shutdownInput()
        } catch (_: Exception) {}
        try
        {
            socket?.shutdownOutput()
        } catch (_: Exception) {}
        disconnectInternal()
    }

    fun disconnectInternal()
    {
        try { reader?.close() } catch (_: Exception) {}
        try { writer?.close() } catch (_: Exception) {}
        try { socket?.close() } catch (_: Exception) {}
        reader = null
        writer = null
        socket = null
    }
}