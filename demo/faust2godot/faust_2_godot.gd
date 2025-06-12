extends Node

var dsp_manager
var faust_effect
var audio_player
var bus_name := "TestBus"
var bus_name2 := "TestBus2"

func _ready():
	print("Initializing Faust DSP test...")

	dsp_manager = FaustDSPManager.new()
	add_child(dsp_manager)
	faust_effect = dsp_manager.createNewDSPInstance()

	var bus_index = AudioServer.get_bus_index(bus_name)

	AudioServer.add_bus_effect(bus_index, faust_effect)

	print("Faust DSP added to bus '%s'" % bus_name)

	dsp_manager.loadDSP("/home/kad/grame_internship/faustGDExtension/demo/dsp/libfoo.so", true, faust_effect)
	# Step 5: Create a test AudioStreamPlayer
	audio_player = AudioStreamPlayer.new()
	add_child(audio_player)
	audio_player.stream = load("res://assets/file_example_WAV_10MG.wav")
	audio_player.bus = bus_name
	audio_player.play()
