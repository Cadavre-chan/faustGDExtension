extends Node


var sine_generator: GDExample
var stream_player: AudioStreamPlayer
var stream: AudioStreamGenerator

func _ready():
	# Create AudioStreamPlayer and set it up
	stream_player = AudioStreamPlayer.new()
	add_child(stream_player)
	
	# Create AudioStreamGenerator (used for real-time audio)
	stream = AudioStreamGenerator.new()
	stream.mix_rate = 44100  # Set sample rate
	stream.buffer_length = 0.1  # Small buffer for real-time processing
	stream_player.stream = stream
	stream_player.play()
	
	# Create and initialize the sine generator
	sine_generator = GDExample.new()
	sine_generator.init_ext(stream_player.get_stream_playback(), 44100, 440)

	# Start generating audio
	set_process(true)

func _process(delta):
	sine_generator.process()
