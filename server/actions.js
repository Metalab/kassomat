module.exports = {
	change: function(data, userinfo, callback) {
		console.log("Change: ", data);
		var total = 0;
		for(var i = 0; i < data.length; ++i) {
			var unit = data[i];
			total += unit.count * unit.value;
		}

		if(userinfo.credits >= total) {
			userinfo.credits -= total;
			if(callback)
				callback({
					status: 1
				});
		} else {
			if(callback)
				callback({
					status: 0
				});
		}
	},
	return: function(data, userinfo, callback) {
		console.log("Return money!");
		userinfo.credits = 0;
		if(callback)
			callback({
				status: 1
			});
	},
	redeem: function(data, userinfo, callback) {
		console.log("Redeem money!");
		if(callback)
			callback({
				status: 1
			});
	},
	donate: function(data, userinfo, callback) {
		console.log("Donate money!");
		userinfo.credits = 0;
		if(callback)
			callback({
				status: 1
			});
	}
};
