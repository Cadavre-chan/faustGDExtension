extends Node

var faust2godot: Faust2Godot
var stream_player: AudioStreamPlayer
var stream: AudioStreamGenerator

func _ready():
	print("Ready!")
	faust2godot = Faust2Godot.new()
	add_child(faust2godot)
	set_process(true)
	print("Begin initialization!")
	faust2godot.init_ext()



func _process(_delta):
	faust2godot.process()
