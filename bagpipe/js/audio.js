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

	increaseVolume: function(intensity) {
		this.player.volume = this.player.volume + intensity * 0.15;
	},

	decreaseVolume: function(intensity) {
		this.player.volume = this.player.volume - intensity * 0.15;
	},

	getVolume: function() {
		if (this.player === undefined) return -1;
		return this.player.volume;
	},

	next: function() {
		this.currentTrack = (this.currentTrack + 1) % 3;
		this.player.src = "music/" + this.trackList[this.currentTrack];
		this.player.load();
		this.playPause();
	},

	previous: function() {
		this.currentTrack = (this.trackList.length + this.currentTrack - 1) % 3;
		this.player.src = "music/" + this.trackList[this.currentTrack];
		this.player.load();
		this.playPause();
	},
}