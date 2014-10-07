var bagPipe = {
	noiseMargin: 2,
	state: "neutral",
    previousAction: "neutral",
    start: Date.now(),
    basePressure: -1,
    maxPressure: -1,
    minPressure: -1,
    maxSipDuration: 200,
    maxPuffDuration: 300,
    timer: undefined,
    actionTimer: undefined,

    setup: function(pressure) {
    	this.basePressure = pressure; //measure the ambient air pressure in the very beginning
		this.maxPressure = pressure + 5;
		this.minPressure = pressure - 5;
	},

	process: function(pressure) {
		if (pressure > this.maxPressure) this.maxPressure = pressure;
		else if (pressure < this.minPressure) this.minPressure = pressure;

		switch (this.state) {

			case "neutral":
				if (pressure > this.basePressure + this.noiseMargin) {
					this.blowStart();
				} else if (pressure < this.basePressure - this.noiseMargin) {
					this.suckStart()
				}
				$("#value").height(0);
				break;

			///////////////////////////////////////////////////////////////////////////////////////

			case "blow":
				if (pressure < this.basePressure + this.noiseMargin && pressure > this.basePressure - this.noiseMargin) {
					this.blowStop();
					this.neutralStart();
				} else if (pressure < this.basePressure - this.noiseMargin) {
					this.blowStop();
					this.suckStart();
				}
				var intensity = (pressure - this.basePressure) / (this.maxPressure - this.basePressure);
				$("#value").css("margin-top", -100 * intensity);
				$("#value").height(100 * intensity);
				audio.increaseVolume(intensity);
				break;

			///////////////////////////////////////////////////////////////////////////////////////

			case "suck":
				if (pressure > this.basePressure - this.noiseMargin && pressure < this.basePressure + this.noiseMargin) {
					this.suckStop();
					this.neutralStart();
				} else if (pressure > this.basePressure + this.noiseMargin) {
					this.suckStop();
					this.blowStart();
				}
				var intensity = (this.basePressure - pressure) / (this.basePressure - this.minPressure);
				$("#value").height(100 * intensity);
				audio.decreaseVolume(intensity);
				break;
		}
	},

	setPreviousAction: function(action) {
		this.previousAction = action;
		clearTimeout(this.timer);
		clearTimeout(this.actionTimer);
		var self = this;
		this.timer = setTimeout(function() {
			self.previousAction = "neutral";
		}, 400);
	},


	stateChange: function() {
		this.start = Date.now();
	},

	neutralStart: function() {
		this.state = "neutral";
		this.stateChange();
	},

	blowStart: function() {
		this.state = "blow";
		this.stateChange();
		$("#value").css("background", "red");
		printDebug("Started blowing…")
	},

	blowStop: function() {
		var duration = Date.now() - this.start;
		if (duration < this.maxPuffDuration) {
			if (this.previousAction == "puff") {
				this.setPreviousAction("puffpuff");
				printDebug("PuffPuff");
				audio.next();
			} else {
				this.setPreviousAction("puff");
				this.actionTimer = setTimeout(function() {
					printDebug("Puff!");
					audio.playPause();
				}, 400);
			}
		}
		else {
			printDebug("Stopped blowing after " + duration + " milliseconds");
			this.setPreviousAction("blow");
		}
	},

	suckStart: function() {
		$("#value").css("background", "green");
		this.state = "suck";
		this.stateChange();
		printDebug("Started sucking…")
	},

	suckStop: function() {
		var duration = Date.now() - this.start;
		if (duration < this.maxSipDuration) {
			if (this.previousAction == "sip") {
				this.setPreviousAction("sipsip");
				printDebug("SipSip");
				audio.previous();
			} else {
				this.setPreviousAction("sip");
				this.actionTimer = setTimeout(function() {
					printDebug("Sip!");
					audio.playPause();
				}, 400);
			}
		}
		else {
			this.setPreviousAction("suck");
			printDebug("Stopped sucking after " + duration + " milliseconds");
		}
	},
}