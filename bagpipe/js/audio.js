$(document).ready(function() {
	audio.player = document.getElementsByTagName('audio')[0];
});

var audio = {
	player: -1,
	currentTrack: 0,

	playPause: function() {
		if (player.paused)
			player.play();
		else
			player.pause();
	},

	increaseVolume: function(intensity) {
		player.volume = player.volume + intensity * 0.15;
	},

	decreaseVolume: function(intensity) {
		player.volume = player.volume - intensity * 0.15;
	},

	getVolume: function() {
		return player.volume;
	},

	next: function() {
		currentTrack = (currentTrack + 1) % 3;
		player.src = "music/track" + currentTrack + ".mp3";
	},

	previous: function() {
		currentTrack = (currentTrack - 1) % 3;
		player.src = "music/track" + currentTrack + ".mp3";
	},
}