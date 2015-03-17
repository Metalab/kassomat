extend = require('util')._extend
redis = require "redis"
db = redis.createClient
	detect_buffers: true

db.on 'error', (error) ->
	console.log "Database error:", error

require('zappajs') ->
	@get '/': 'hi'
	@io.sockets.on 'connection', (socket) ->
		console.log "Socket connected!"

		db.hgetall "userinfo", (err, reply) ->
			if err
				console.error err
			else
				reply.credits = parseInt reply.credits
				socket.emit 'userinfo', reply
		db.keys 'entity:DenominationLevel:*', (err, keys) ->
			if err
				console.error "Failed getting DenominationLevel keys:", err
			else
				keys.forEach (key) ->
					db.hgetall key, (err, reply) ->
						if err
							console.error "Failed fetching " + key + ":", err
						else
							payload = {}
							payload.id = parseInt(key.split(":")[2])
							payload.count = reply.count
							socket.emit 'pushData',
								type: 'DenominationLevel'
								payload: [ payload ]

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

		socket.on 'disconnect', ->
			subscriptions.end()
