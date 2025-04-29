extends Node

var testBusIndex
var faustEffect
var audioPlayer

func _ready():
	print("Ready!")
	testBusIndex = AudioServer.get_bus_index("TestBus")
	faustEffect = Faust2Godot.new()
	AudioServer.add_bus_effect(testBusIndex, faustEffect)
	audioPlayer = AudioStreamPlayer.new()
	add_child(audioPlayer)
	audioPlayer.stream = load("res://assets/file_example_WAV_10MG.wav")
	audioPlayer.bus = "TestBus"
	faustEffect.setDrive(5)
	audioPlayer.play()
