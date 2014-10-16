$(document).ready(function() {
	audio.setup();
});

var audio = {
	player: -1,
	currentTrack: 0,
	trackList: ["track0.mp3","track1.mp3","track2.mp3"],

	setup: function() {
		this.player = document.getElementsByTagName('audio')[0];
		this.player.src = "music/" + this.trackList[this.currentTrack];
		this.player.load();
	},

	playPause: function() {
		if (this.player.paused)
			this.player.play();
		else
			this.player.pause();
	},

	setVolume: function(volume) {
		if (this.player === undefined) return -1;
		this.player.volume = volume;
	}

	getVolume: function() {
		if (this.player === undefined) return -1;
		return this.player.volume;
	},

	next: function() {
		this.currentTrack = (this.currentTrack + 1) % 3;
		this.player.src = "music/" + this.trackList[this.currentTrack];
		this.player.load();
		this.player.play();
	},

	previous: function() {
		this.currentTrack = (this.trackList.length + this.currentTrack - 1) % 3;
		this.player.src = "music/" + this.trackList[this.currentTrack];
		this.player.load();
		this.player.play();	
	},

	rewind: function(duration) {
		this.player.currentTime -= duration; //duration in seconds, depending on intensity
	},

	fastForward: function(duration) {
		this.player.currentTime += duration; //duration in seconds, depending on intensity
	},
}