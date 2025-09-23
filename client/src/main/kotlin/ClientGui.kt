package edu.ntnu.eliasei.smarttv

import imgui.ImGui
import imgui.app.Application
import imgui.app.Configuration
import imgui.type.ImInt
import imgui.type.ImString
import java.util.concurrent.CopyOnWriteArrayList

class SmartRemoteApp(private val host: String, private val port: Int) : Application() {
    private var connection = RemoteConnection()
    private val log = CopyOnWriteArrayList<String>()

    private val hostInput = ImString(host, 64)
    private val portInput = ImInt(port)
    private val channelInput = ImInt(1)

    override fun configure(config: Configuration) {
        config.title = "SmartTV Remote"
        config.width = 640
        config.height = 480
    }

    override fun process() {
        ImGui.begin("SmartTV Remote")

        val disabled = connection.isConnected
        if (disabled) ImGui.beginDisabled()
        ImGui.inputText("Host", hostInput)
        ImGui.sameLine()
        ImGui.inputInt("Port", portInput)
        if (disabled) ImGui.endDisabled()

        if (connection.isConnected) {
            if (ImGui.button("Disconnect")) {
                connection.disconnect()
                log.add(">> Disconnected from $host:$port")
            }
        } else {
            if (ImGui.button("Connect")) {
                connection.connect(hostInput.get(), portInput.get())
                log.add(">> Connected to $host:$port")
            }
        }

        ImGui.separator()

        if (connection.isConnected) {
            if (ImGui.button("ON")) {
                val cmd = CommandRegistry.get("ON")
                connection.send(cmd!!.build.invoke(emptyList()))
                log.add(">> Sent: ON")
            }
            ImGui.sameLine()
            if (ImGui.button("OFF")) {
                val cmd = CommandRegistry.get("OFF")
                connection.send(cmd!!.build.invoke(emptyList()))
                log.add(">> Sent: OFF")
            }

            if (ImGui.button("Channel Up")) {
                val cmd = CommandRegistry.get("SET_CHANNEL")
                connection.send(cmd!!.build.invoke(listOf("+1")))
                log.add(">> Sent: SET_CHANNEL +1")
            }
            ImGui.sameLine()
            if (ImGui.button("Channel Down")) {
                val cmd = CommandRegistry.get("SET_CHANNEL")
                connection.send(cmd!!.build.invoke(listOf("-1")))
                log.add(">> Sent: SET_CHANNEL -1")
            }

            ImGui.inputInt("Channel", channelInput)
            ImGui.sameLine()
            if (ImGui.button("Set Channel")) {
                val cmd = CommandRegistry.get("SET_CHANNEL")
                connection.send(cmd!!.build.invoke(listOf(channelInput.get().toString())))
                log.add(">> Sent: SET_CHANNEL ${channelInput.get()}")
            }

            ImGui.separator()

            if (ImGui.button("Get Channel")) {
                val cmd = CommandRegistry.get("GET_CHANNEL")
                connection.send(cmd!!.build.invoke(emptyList()))
                log.add(">> Sent: GET_CHANNEL")
            }

            if (ImGui.button("Get Channel Count")) {
                val cmd = CommandRegistry.get("GET_CHANNELS")
                connection.send(cmd!!.build.invoke(emptyList()))
                log.add(">> Sent: GET_CHANNELS")
            }
        } else {
            ImGui.text("Connect to a TV to enable controls.")
        }

        ImGui.end()

        // Drain all new messages from the connection
        var msg = connection.pollMessage()
        while (msg != null)
        {
            log.add(msg)
            msg = connection.pollMessage()
        }

        ImGui.begin("Log")

        for (entry in log) {
            when {
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

fun runGui(host: String, port: Int) {
    Application.launch(SmartRemoteApp(host, port))
}
