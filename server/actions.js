module.exports = {
	change: function(data, callback) {
		console.log("Change: ", data);
		if(callback)
			callback({
				status: 1
			});
	},
	return: function(data, callback) {
		console.log("Return money!");
		if(callback)
			callback({
				status: 1
			});
	},
	redeem: function(data, callback) {
		console.log("Redeem money!");
		if(callback)
			callback({
				status: 1
			});
	},
	donate: function(data, callback) {
		console.log("Donate money!");
		if(callback)
			callback({
				status: 1
			});
	}
};
