package edu.ntnu.eliasei.smarttv

import imgui.ImGui
import imgui.app.Application
import imgui.app.Configuration

class SmartRemoteApp(private val host: String, private val port: Int) : Application()
{
    override fun configure(config: Configuration)
    {
        config.title = "SmartTV Remote"
        config.width = 640
        config.height = 480
    }

    override fun process()
    {
        ImGui.begin("SmartTV Remote")

        ImGui.text("GUI mode is not yet implemented.")

        ImGui.end()
    }
}

fun runGui(host: String, port: Int)
{
    Application.launch(SmartRemoteApp(host, port))
}
