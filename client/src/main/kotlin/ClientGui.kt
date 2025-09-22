package edu.ntnu.eliasei.smarttv

import imgui.ImGui
import imgui.app.Application
import imgui.app.Configuration
import java.util.concurrent.CopyOnWriteArrayList

class SmartRemoteApp(private val host: String, private val port: Int) : Application()
{
    private var connected = false
    private val log = CopyOnWriteArrayList<String>()

    override fun configure(config: Configuration)
    {
        config.title = "SmartTV Remote"
        config.width = 640
        config.height = 480
    }

    override fun process()
    {
        ImGui.begin("SmartTV Remote")

        ImGui.text("Host: $host")
        ImGui.sameLine()
        ImGui.text("Port: $port")

        if (!connected)
        {
            if (ImGui.button("Connect"))
            {
                // TODO: Implement connection logic
                connected = true
                log.add(">> Connected to $host:$port")
            }
        }
        else
        {
            if (ImGui.button("Disconnect"))
            {
                // TODO: Implement disconnection logic
                connected = false
                log.add(">> Disconnected from $host:$port")
            }
        }

        ImGui.separator()

        if (connected)
        {
            if (ImGui.button("ON"))
            {
                val cmd = CommandRegistry.get("ON")
                // TODO: Send ON command using RemoteConnection
                log.add(">> Sent: ON")
            }
            ImGui.sameLine()
            if (ImGui.button("OFF"))
            {
                val cmd = CommandRegistry.get("OFF")
                // TODO: Send OFF command using RemoteConnection
                log.add(">> Sent: OFF")
            }

            if (ImGui.button("Channel Up"))
            {
                val cmd = CommandRegistry.get("SET_CHANNEL")
                // TODO: Send Channel Up command
                log.add(">> Sent: SET_CHANNEL current+1")
            }
            ImGui.sameLine()
            if (ImGui.button("Channel Down"))
            {
                val cmd = CommandRegistry.get("SET_CHANNEL")
                // TODO: Send Channel Down command
                log.add(">> Sent: SET_CHANNEL current-1")
            }
        }
        else
        {
            ImGui.text("Connect to a TV to enable controls.")
        }

        ImGui.end()

        ImGui.begin("Log")

        for (entry in log)
        {
            when
            {
                entry.startsWith("OK") || entry.startsWith("CHANNEL") ->
                    ImGui.textColored(0f, 1f, 0f, 1f, entry) // Green for success
                entry.startsWith("ERROR") ->
                    ImGui.textColored(1f, 0f, 0f, 1f, entry) // Red for errors
                entry.startsWith(">>") ->
                    ImGui.textColored(0.5f, 0.5f, 0f, 1f, entry) // Yellow for sent commands
                else ->
                    ImGui.text(entry) // Default color for other messages
            }
        }

        ImGui.end()
    }
}

fun runGui(host: String, port: Int)
{
    Application.launch(SmartRemoteApp(host, port))
}
