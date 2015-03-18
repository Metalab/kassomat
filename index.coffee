extend = require('util')._extend
redis = require "redis"
db = redis.createClient
	detect_buffers: true

db.on 'error', (error) ->
	console.log "Database error:", error

require('zappajs') ->
	@get '/': 'hi'

	@get '/projectimages/:id', (req, res) ->
		path = "projectimages:" + req.params.id
		db.hget path, "type", (err, type) ->
			if err
				console.error err
				res.sendStatus(500)
			else
				if not type
					res.sendStatus(404)
				else
					db.hget path, new Buffer("data"), (err, imageData) ->
						if err
							console.error "[Project Images " + req.params.id + "]:", err
							res.sendStatus(500)
						else
							if not imageData
								res.sendStatus(404)
							else
								res.type type
								res.send imageData

	@io.sockets.on 'connection', (socket) ->
		console.log "Socket connected!"

		# A new client needs userinfo pushed
		db.hgetall "userinfo", (err, reply) ->
			if err
				console.error err
			else
				reply.credits = parseInt reply.credits
				socket.emit 'userinfo', reply

		subscriptions = redis.createClient()
		subscriptions.on "message", (channel, message) ->
			if channel == "frontend"
				socket.emit 'message', message
			else if channel == "updates"
				parts = message.split(":")
				switch parts[0]
					when "userinfo"
						db.hgetall "userinfo", (err, reply) ->
							if err
								console.error "Failed fetching userinfo:", err
							else
								reply.credits = parseInt(reply.credits)
								socket.emit 'userinfo', reply
					when "update"
						db.hgetall "entity:" + message, (err, reply) ->
							if err
								console.error "Failed fetching entity:" + message + ":", err
							else
								payload = extend {}, reply
								payload.id = parts[2]
								socket.emit 'pushData',
									type: parts[1]
									payload: [ payload ]
					when "delete"
						socket.emit 'delete',
							type: parts[1]
							ids: [ parts[2] ]
		subscriptions.subscribe "frontend"
		subscriptions.subscribe "updates"

		socket.on 'findAll', (data, callback) ->
			console.log "findAll " + JSON.stringify(data)
			db.keys 'entity:' + data.type + ":*", (err, keys) ->
				if err
					console.error "[findAll " + data.type + "]:", err
					callback
						status: 0
						error: err
				else
					console.log "Entity " + data.type + ":", JSON.stringify(keys)
					keysRemaining = keys.length
					results = []
					keys.forEach (key) ->
						db.hgetall key, (err, reply) ->
							if keysRemaining > 0
								if err
									keysRemaining = -1
									console.error "[findAll " + key + "]:", err
									callback
										status: 0
										error: err
								else
									keysRemaining--
									reply.id = key.split(":")[2]
									results.push reply
									if keysRemaining == 0
										callback
											status: 1
											content: results

		socket.on 'find', (data, callback) ->
			db.hgetall "entity:" + data.type + ":" + data.query.id, (err, reply) ->
				if err
					console.error "[find " + data.type + ":" + data.query.id + "]:", err
					callback
						status: 0
						error: err
				else
					reply.id = data.query.id
					callback
						status: 1
						content: reply

		socket.on 'disconnect', ->
			subscriptions.end()
