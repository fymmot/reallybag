///////////////////////////////////////////////////////////////////////////////
// GLOBAL SETTINGS
///////////////////////////////////////////////////////////////////////////////

var rFleaMAC = 47740, // enter your rFlea MAC address here (e.g. 47740)
    frequency = 32, // use higher frequency for better response. Standard is 4, higher frequencies use more batteries
    debug = true, // outputs debug messages on the phone. Set to true if something doesn't work out
    verbose = false; // outputs even more debug messages (use with care!)

///////////////////////////////////////////////////////////////////////////////
// HERE'S WHERE THE MAGIC HAPPENS
///////////////////////////////////////////////////////////////////////////////

function loop(data) {
	var pressure = data.analogIn[1];
	if (packagesReceived == 1) bagpipe.setup(pressure);

	//output current value
	$("#debug").html(state + " at value " + pressure + "<br/>Volume: " + audio.getVolume());

	bagpipe.process(pressure);
}

function debug(message) {
	$("#output").html($("#output").html() + message + "<br/>");
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