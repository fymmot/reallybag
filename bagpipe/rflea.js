///////////////////////////////////////////////////////////////////////////////
// GLOBAL SETTINGS
///////////////////////////////////////////////////////////////////////////////

var rFleaMAC = 47740, // enter your rFlea MAC address here (e.g. 47740)
    frequency = 32, // use higher frequency for better response. Standard is 4, higher frequencies use more batteries
    debug = true, // outputs debug messages on the phone. Set to true if something doesn't work out
    verbose = false, // outputs even more debug messages (use with care!)
    noiseMargin = 2,
    state = "neutral",
    previousAction = "neutral",
    start = Date.now(),
    maxPressure = -1,
    minPressure = -1,
    maxSipDuration = 200,
    maxPuffDuration = 300,
    currentTrack = 0,
    audio,
    timer;

///////////////////////////////////////////////////////////////////////////////
// HERE'S WHERE THE MAGIC HAPPENS
///////////////////////////////////////////////////////////////////////////////

$(document).ready(function() {
	audio = document.getElementsByTagName('audio')[0];
});

function theMagic(data) {
	var pressure = data.analogIn[1];
	if (packagesReceived == 1) {
		basePressure = pressure; //measure the ambient air pressure in the very beginning
		maxPressure = pressure + 5;
		minPressure = pressure - 5;
	}

	if (pressure > maxPressure) maxPressure = pressure;
	else if (pressure < minPressure) minPressure = pressure;

	//output current value
	$("#debug").html(state + " at value " + pressure + "<br/>Volume: " + audio.volume);

	switch (state) {
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
}

function playPause() {
	if (audio.paused)
		audio.play();
	else
		audio.pause();
}

function increaseVolume(intensity) {
	audio.volume = audio.volume + intensity * 0.15;
}

function decreaseVolume(intensity) {
	audio.volume = audio.volume - intensity * 0.15;
}

function next() {
	currentTrack = (currentTrack + 1) % 3;
	audio.src = "music/track" + currentTrack + ".mp3";
}

function previous() {
	currentTrack = (currentTrack - 1) % 3;
	audio.src = "music/track" + currentTrack + ".mp3";
}

function stateChange() {
	start = Date.now();
}

function neutralStart() {
	state = "neutral";
	stateChange();
}

function blowStart() {
	state = "blow";
	stateChange();
	$("#value").css("background", "red");
	$("#output").html($("#output").html() + "Started blowing…<br/>")
}

function setPreviousAction(action) {
	previousAction = action;
	clearTimeout(timer);
	timer = setTimeout(function() {
		previousAction = "neutral";
	}, 400);
}

function blowStop() {
	var duration = Date.now() - start;
	if (duration < maxPuffDuration) {
		if (previousAction == "puff") {
			setPreviousAction("puffpuff");
			$("#output").html($("#output").html() + "PuffPuff<br/>");
			next();
		} else {
			$("#output").html($("#output").html() + "Puff!<br/>");
			playPause();
			setPreviousAction("puff");
		}
	}
	else {
		$("#output").html($("#output").html() + "Stopped blowing after " + duration + " milliseconds<br/>");
		setPreviousAction("blow");
	}
}

function suckStart() {
	$("#value").css("background", "green");
	state = "suck";
	stateChange();
	$("#output").html($("#output").html() + "Started sucking…<br/>")
}

function suckStop() {
	var duration = Date.now() - start;
	if (duration < maxSipDuration) {
		if (previousAction == "sip") {
			setPreviousAction("sipsip");
			$("#output").html($("#output").html() + "SipSip<br/>");
			previous();
		} else {
			$("#output").html($("#output").html() + "Sip!<br/>");
			setPreviousAction("sip");
		}
	}
	else {
		setPreviousAction("suck");
		$("#output").html($("#output").html() + "Stopped sucking after " + duration + " milliseconds<br/>");
	}
}

///////////////////////////////////////////////////////////////////////////////
// SETUP
///////////////////////////////////////////////////////////////////////////////

var antConnected = AntInterface.addNewChannel(false, rFleaMAC, frequency); //false means we are opening a Slave master
var packagesReceived = 0; //counter
var basePressure = -1;

if (debug) AndroidInterface.showToast("Ant connected " + antConnected);

///////////////////////////////////////////////////////////////////////////////
// ANT MESSAGE HANDLING
///////////////////////////////////////////////////////////////////////////////

function onMessage(data) {
	packagesReceived++;
	var rFleaData = parseRflea(data);

	//user code
	theMagic(rFleaData);

	var tx_message = toString(LED_value,1,2,3,4,5,6,7);
	var tx_successful = AntInterface.send(tx_message); //returns true if format is correct
	if (debug && verbose) AndroidInterface.showToast(tx_message, tx_successful);
}

function onPaired() {
	if (debug) AndroidInterface.showToast("Paired with rFlea");
}

function onSearching() {
	if (debug) AndroidInterface.showToast("Searching for rFlea…");
}

///////////////////////////////////////////////////////////////////////////////
// HELPER METHODS
///////////////////////////////////////////////////////////////////////////////

function parseRflea(data) {
	var data = data.split(",");
	return {
		analogIn: [
			parseInt(data[0]),
			parseInt(data[1])
		],
		digitalIn: [
			parseInt(data[2]),
			parseInt(data[3]),
			parseInt(data[4]),
			parseInt(data[5])
		],
		MAC: "rFlea " + toHex(data[8]) + ":" +toHex(data[9]),
	};
}

function toHex(val) {
	var result = parseInt(val).toString(16);
	if (result.length == 1) {
		result = "0" + result;
	}
	return result;
}

function toString(a,b,c,d,e,f,g,h){
	return (a+":"+b+":"+c+":"+d+":"+e+":"+f+":"+g+":"+h);
}