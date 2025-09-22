plugins {
    kotlin("jvm") version "2.2.0"
    application
}

group = "edu.ntnu.eliasei.smarttv"
version = "1.0-SNAPSHOT"

repositories {
    mavenCentral()
}

dependencies {
    testImplementation(kotlin("test"))

    implementation("io.github.spair:imgui-java-app:1.90.0")
}

tasks.test {
    useJUnitPlatform()
}
kotlin {
    jvmToolchain(24)
}

application {
    mainClass.set("edu.ntnu.eliasei.smarttv.ClientCliKt")
}