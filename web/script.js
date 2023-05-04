function playSine() {
	// Create an AudioContext object
	var audioCtx = new (window.AudioContext || window.webkitAudioContext)();

	// Set the sample rate (in samples per second)
	var sampleRate = audioCtx.sampleRate;

	// Set the duration of the audio buffer (in seconds)
	var duration = 1;

	// Create a buffer to hold the audio samples
	var buffer = audioCtx.createBuffer(1, sampleRate * duration, sampleRate);

	// Get the audio data for the buffer
	var channelData = buffer.getChannelData(0);

	// Set the frequency of the sine wave (in Hz)
	var frequency = 440;

	// Generate the audio samples for the sine wave
	for (var i = 0; i < channelData.length; i++) {
		var t = i / sampleRate;
		channelData[i] = Math.sin(2 * Math.PI * frequency * t);
	}

	// Create a buffer source and connect it to the audio context's destination
	var source = audioCtx.createBufferSource();
	source.buffer = buffer;
	source.connect(audioCtx.destination);

	// Start playing the audio
	source.start();
}
