var bagPipe = {
	noiseMargin: 2,
	state: "neutral",
    previousAction: "neutral",
    start: Date.now(),
    maxPressure: -1,
    minPressure: -1,
    maxSipDuration: 200,
    maxPuffDuration: 300,
    timer,
    basePressure: -1,

    setup: function(pressure) {
    	basePressure = pressure; //measure the ambient air pressure in the very beginning
		maxPressure = pressure + 5;
		minPressure = pressure - 5;
	},

	checkPressureBoundaries: function(pressure) {
		if (pressure > maxPressure) maxPressure = pressure;
		else if (pressure < minPressure) minPressure = pressure;
	},

	process: function(pressure) {
		switch (_state) {
			case "neutral":
				if (pressure > basePressure + noiseMargin) {
					blowStart();
				} else if (pressure < basePressure - noiseMargin) {
					suckStart()
				}
				$("#value").height(0);
				break;

			case "blow":
				if (pressure < basePressure + noiseMargin && pressure > basePressure - noiseMargin) {
					blowStop();
					neutralStart();
				} else if (pressure < basePressure - noiseMargin) {
					blowStop();
					suckStart();
				}
				$("#value").css("margin-top", -100 * (pressure - basePressure) / (maxPressure - basePressure));
				$("#value").height(100 * (pressure - basePressure) / (maxPressure - basePressure));
				increaseVolume((pressure - basePressure) / (maxPressure - basePressure));
				break;

			case "suck":
				if (pressure > basePressure - noiseMargin && pressure < basePressure + noiseMargin) {
					suckStop();
					neutralStart();
				} else if (pressure > basePressure + noiseMargin) {
					suckStop();
					blowStart();
				}
				$("#value").height(100 * (basePressure - pressure) / (basePressure - minPressure));
				decreaseVolume((basePressure - pressure) / (basePressure - minPressure));
				break;
		}
	},


	stateChange: function() {
		start = Date.now();
	},

	neutralStart: function() {
		state = "neutral";
		stateChange();
	},

	blowStart: function() {
		state = "blow";
		stateChange();
		$("#value").css("background", "red");
		debug("Started blowing…")
	},

	setPreviousAction: function(action) {
		previousAction = action;
		clearTimeout(timer);
		timer = setTimeout(function() {
			previousAction = "neutral";
		}, 400);
	},

	blowStop: function() {
		var duration = Date.now() - start;
		if (duration < maxPuffDuration) {
			if (previousAction == "puff") {
				setPreviousAction("puffpuff");
				debug("PuffPuff");
				next();
			} else {
				debug("Puff!");
				playPause();
				setPreviousAction("puff");
			}
		}
		else {
			debug("Stopped blowing after " + duration + " milliseconds");
			setPreviousAction("blow");
		}
	},

	suckStart: function() {
		$("#value").css("background", "green");
		state = "suck";
		stateChange();
		debug("Started sucking…")
	}

	suckStop: function() {
		var duration = Date.now() - start;
		if (duration < maxSipDuration) {
			if (previousAction == "sip") {
				setPreviousAction("sipsip");
				debug("SipSip");
				previous();
			} else {
				debug("Sip!");
				setPreviousAction("sip");
			}
		}
		else {
			setPreviousAction("suck");
			debug("Stopped sucking after " + duration + " milliseconds");
		}
	}

}