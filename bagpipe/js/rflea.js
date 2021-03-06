///////////////////////////////////////////////////////////////////////////////
// GLOBAL SETTINGS
///////////////////////////////////////////////////////////////////////////////

var rFleaMAC = 47740, // enter your rFlea MAC address here (e.g. 47740)
    frequency = 32, // use higher frequency for better response. Standard is 4, higher frequencies use more batteries
    debug = true, // outputs debug messages on the phone. Set to true if something doesn't work out
    verbose = false; // outputs even more debug messages (use with care!)

var PUFF = 1,
    PUFFPUFF = 2,
    SIP = 3,
    SIPSIP = 4;

    var waitAfterEvent = false;

///////////////////////////////////////////////////////////////////////////////
// HERE'S WHERE THE MAGIC HAPPENS
///////////////////////////////////////////////////////////////////////////////

function loop(data) {
	audio.setVolume(data[0] / 100.0);
	$("#volume").width(320 * (data[0] / 100.0));
	if(!waitAfterEvent){
		var receiveEvent = false;
		if (data[1] == PUFF || data[1] == SIP) {
			audio.playPause();
			receiveEvent=true;
		}
		else if (data[1] == PUFFPUFF) {
			audio.next();
			receiveEvent = true;
		}
		else if (data[1] == SIPSIP) {
			audio.previous();
			receiveEvent=true;
		}

		if(receiveEvent){
			waitAfterEvent = true;
			setTimeout(function() {waitAfterEvent=false}, 400);
		}
	}	

	//output current value
	$("#debug").html(data[0] + " " + data[1] + " " + data[7]);
	var tx_message = toString(waitAfterEvent ? 1 : 0,0,0,0,0,0,0,0);
	var tx_successful = AntInterface.send(tx_message); //returns true if format is correct
}

function printDebug(message) {
	if (debug) $("#output").html($("#output").html() + message + "<br/>");
}

///////////////////////////////////////////////////////////////////////////////
// SETUP
///////////////////////////////////////////////////////////////////////////////

var antConnected = AntInterface.addNewChannel(false, rFleaMAC, frequency); //false means we are opening a Slave master
var packagesReceived = 0; //counter

if (debug) AndroidInterface.showToast("Ant connected " + antConnected);


///////////////////////////////////////////////////////////////////////////////
// ANT MESSAGE HANDLING
///////////////////////////////////////////////////////////////////////////////

function onMessage(data) {
	packagesReceived++;
	var rFleaData = parseRflea(data);

	//user code
	loop(rFleaData);

	// var tx_message = toString(0,1,2,3,4,5,6,7);
	// var tx_successful = Antvarerface.send(tx_message); //returns true if format is correct

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
	return data;
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