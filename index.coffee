extend = require('util')._extend
redis = require "redis"
db = redis.createClient
	detect_buffers: true

db.on 'error', (error) ->
	console.log "Database error:", error

require('zappajs') ->
	@get '/': 'hi'

	@get '/images/projects/:id', (req, res) ->
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
		db.hgetall "userinfo", (err, userinfo) ->
			if err
				console.error err
			else
				socket.emit 'userinfo',
					username: encodeURI(userinfo.username)
					credits: parseInt userinfo.credits

		subscriptions = redis.createClient()
		subscriptions.on "message", (channel, message) ->
			if channel == "__keyevent@0__:hset"
				if message == "userinfo"
					db.hgetall "userinfo", (err, reply) ->
						if err
							console.error "Failed fetching userinfo:", err
						else
							socket.emit 'userinfo',
								username: encodeURI(reply.username)
								credits: parseInt(reply.credits)
				else if message.slice(0, "entity:".length) == "entity:"
					db.hgetall message, (err, reply) ->
						if err
							console.error "Failed fetching entity:" + message + ":", err
						else
							parts = message.split(":")
							payload = extend {}, reply
							payload.id = parts[2]
							socket.emit 'pushData',
								type: parts[1]
								payload: [ Object.keys(payload).reduce(((prev,cur) -> prev[cur] = encodeURI(payload[cur]); prev), {}) ]
			else if channel == "__keyevent@0__:del"
				if message.slice(0, "entity:".length) == "entity:"
					parts = message.split(":")
					socket.emit 'delete',
						type: parts[1]
						ids: [ encodeURI(parts[2]) ]
			else if channel == "frontend"
				socket.emit 'message', encodeURI(message)
		subscriptions.subscribe "frontend"
		subscriptions.subscribe "__keyevent@0__:hset"
		subscriptions.subscribe "__keyevent@0__:del"

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
									results.push Object.keys(reply).reduce(((prev,cur) -> prev[cur] = encodeURI(reply[cur]); prev), {})
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
						content: Object.keys(reply).reduce(((prev,cur) -> prev[cur] = encodeURI(reply[cur]); prev), {})

		socket.on 'action', (data) ->
			db.publish 'action', JSON.stringify(data)

		socket.on 'buy', (data) ->
			db.publish 'buy', data.name

		socket.on 'log', (data) ->
			timestamp = new Date()
			db.lpush 'log', JSON.stringify
				source: 'frontend'
				timestamp: timestamp.getTime() / 1000
				message: decodeURI(data)

		socket.on 'disconnect', ->
			subscriptions.end()
