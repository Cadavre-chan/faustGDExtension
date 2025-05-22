extends Node

var dsp_manager
var faust_effect
var audio_player
var bus_name := "TestBus"

func _ready():
	print("Initializing Faust DSP test...")

	dsp_manager = FaustDSPManager.new()

	faust_effect = dsp_manager.getDSPInstance()
	if faust_effect == null:
		print("Failed to get Faust audio effect.")
		return

	var bus_index = AudioServer.get_bus_index(bus_name)
	if bus_index == -1:
		print("Audio bus not found: %s" % bus_name)
		return
	AudioServer.add_bus_effect(bus_index, faust_effect)

	print("Faust DSP added to bus '%s'" % bus_name)

	dsp_manager.loadDSP("/home/kad/grame_internship/faustGDExtension/demo/dsp/libfoo.so")
	# Step 5: Create a test AudioStreamPlayer
	audio_player = AudioStreamPlayer.new()
	add_child(audio_player)
	audio_player.stream = load("res://assets/file_example_WAV_10MG.wav")
	audio_player.bus = bus_name
	audio_player.play()
