$(document).ready(function() {
	audio.setup();
});

var audio = {
	player: -1,
	currentTrack: 0,
	players: [],

	setup: function() {
		this.player1 = document.getElementById('audio1');
		this.player2 = document.getElementById('audio2');
		this.player3 = document.getElementById('audio3');
		this.players.push(this.player1);
		this.players.push(this.player2);
		this.players.push(this.player3);
		this.player1.load();
		this.player2.load();
		this.player3.load();
	},

	playPause: function() {
		if (this.players[this.currentTrack].paused)
			this.players[this.currentTrack].play();
		else
			this.players[this.currentTrack].pause();
	},

	pauseAll: function() {
		for (var i = 0; i<=3; i++) {
			if (this.players[i] === undefined) return -1;
			this.players[i].pause();
		}
	},

	setVolume: function(volume) {
		for (var i = 0; i<=3; i++) {
			if (this.players[i] === undefined) return -1;
			this.players[i].volume = volume;
		}
	},

	next: function() {
		this.currentTrack = (this.currentTrack + 1) % 3;
		this.pauseAll();
		this.players[this.currentTrack].currentTime = 0;
		this.players[this.currentTrack].play();
	},

	previous: function() {
		this.currentTrack = (3 + this.currentTrack - 1) % 3;
		this.pauseAll();
		this.players[this.currentTrack].currentTime = 0;
		this.players[this.currentTrack].play();
	},

	rewind: function(duration) {
		this.player.currentTime -= duration; //duration in seconds, depending on intensity
	},

	fastForward: function(duration) {
		this.player.currentTime += duration; //duration in seconds, depending on intensity
	},
}