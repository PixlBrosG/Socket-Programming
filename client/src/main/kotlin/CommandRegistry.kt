package edu.ntnu.eliasei.smarttv

data class Command(
    val name: String,
    val description: String,
    val build: (List<String>) -> String
)

object CommandRegistry
{
    private val commands = mutableMapOf<String, Command>()

    fun register(cmd: Command)
    {
        commands[cmd.name.uppercase()] = cmd
    }

    fun get(name: String): Command ?
    {
        return commands[name.uppercase()]
    }

    fun all(): Collection<Command> = commands.values
}

fun initCommands()
{
    CommandRegistry.register(Command("ON", "Turn TV on") { _ -> "ON "})
    CommandRegistry.register(Command("OFF", "Turn TV off") { _ -> "OFF "})
    CommandRegistry.register(Command("GET_CHANNEL", "Get current channel") { _ -> "GET_CHANNEL "})
    CommandRegistry.register(Command("GET_CHANNELS", "Get number of available channels") { _ -> "GET_CHANNELS" })
    CommandRegistry.register(Command("SET_CHANNEL", "Set channel by number") { args ->
        if (args.size != 1) throw IllegalArgumentException("SET_CHANNEL requires 1 argument")
        "SET_CHANNEL ${args[0]}"
    })
}