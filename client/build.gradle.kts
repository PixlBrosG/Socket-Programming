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
}

tasks.test {
    useJUnitPlatform()
}
kotlin {
    jvmToolchain(24)
}

application {
    mainClass.set("edu.ntnu.eliasei.smarttv.MainKt")
}