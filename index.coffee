extend = require('util')._extend
redis = require "redis"
db = redis.createClient
	detect_buffers: true

db.on 'error', (error) ->
	console.log "Database error:", error

WebSocketServer = require('ws').Server
http = require('http')
express = require('express')
app = express()

app.get '/images/projects/:id', (req, res) ->
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

server = http.createServer app
server.listen 3000

wss = new WebSocketServer
	server: server
	path: "/ws"

wss.on 'connection', (ws) ->
	console.log "Socket connected!"
	if ws.protocol != "kassomat"
		console.error "Invalid protocol:", ws.protocol
		ws.close()
		return

	# A new client needs userinfo pushed
	db.hgetall "userinfo", (err, userinfo) ->
		if err
			console.error err
		else
			ws.send JSON.stringify
				command: 'userinfo'
				username: userinfo.username
				credits: parseInt userinfo.credits

	ws.on 'message', (message) ->
		data = JSON.parse message
		console.log 'received: %s', JSON.stringify data, true, 2

		switch data.eventType
			when 'findAll'
				console.log "findAll " + JSON.stringify data.query, true, 2
				db.keys 'entity:' + data.query.type + ":*", (err, keys) ->
					if err
						console.error "[findAll " + data.query.type + "]:", err
						ws.send JSON.stringify
							command: 'reply'
							id: data.id
							status: 0
							error: err
					else
						console.log "Entity " + data.query.type + ":", JSON.stringify(keys)
						keysRemaining = keys.length
						results = []
						keys.forEach (key) ->
							db.hgetall key, (err, reply) ->
								if keysRemaining > 0
									if err
										keysRemaining = -1
										console.error "[findAll " + key + "]:", err
										ws.send JSON.stringify
											command: 'reply'
											id: data.id
											status: 0
											error: err
									else
										keysRemaining--
										reply.id = key.split(":")[2]
										results.push reply
										if keysRemaining == 0
											content = {}
											content[data.query.type + 's'] = results
											ws.send JSON.stringify
												command: 'reply'
												id: data.id
												status: 1
												content: content
			when 'find'
				db.hgetall "entity:" + data.query.type + ":" + data.query.id, (err, reply) ->
					if err
						console.error "[find " + data.query.type + ":" + data.query.id + "]:", err
						ws.send JSON.stringify
							command: 'reply'
							id: data.id
							status: 0
							error: err
					else
						if not reply
							ws.send JSON.stringify
								command: 'reply'
								id: data.id
								status: 1
						else
							reply.id = data.query.id
							content = {}
							content[data.query.type] = reply
							ws.send JSON.stringify
								command: 'reply'
								id: data.id
								status: 1
								content: content
			when 'findQuery'
				db.hgetall "entity:" + data.query.type + ":" + data.query.id, (err, reply) ->
					if err
						console.error "[find " + data.query.type + ":" + data.query.id + "]:", err
						ws.send JSON.stringify
							command: 'reply'
							id: data.id
							status: 0
							error: err
					else
						if not reply
							ws.send JSON.stringify
								command: 'reply'
								id: data.id
								status: 1
						else
							reply.id = data.query.id
							ws.send JSON.stringify
								command: 'reply'
								id: data.id
								status: 1
								content: [reply]
			when 'action'
				db.publish 'action', JSON.stringify data
			when 'buy'
				if data.name
					db.publish 'buy', data.name
				else
					console.error "Incorrect number of arguments in buy action"
			when 'log'
				timestamp = new Date()
				console.log '[log]', data.message
				db.lpush 'log', JSON.stringify
					source: 'frontend'
					timestamp: timestamp.getTime() / 1000
					message: data.message
			else
				console.error "Unknown command received: ", data.eventType

	ws.on 'disconnect', ->
		subscriptions.end()

subscriptions = redis.createClient()
subscriptions.on "message", (channel, message) ->
	if channel == "__keyevent@0__:hset"
		if message == "userinfo"
			db.hgetall "userinfo", (err, reply) ->
				if err
					console.error "Failed fetching userinfo:", err
				else
					wss.clients.forEach (client) ->
						client.send JSON.stringify
							command: 'userinfo'
							username: reply.username
							credits: parseInt(reply.credits)
		else if message.slice(0, "entity:".length) == "entity:"
			db.hgetall message, (err, reply) ->
				if err
					console.error "Failed fetching entity:" + message + ":", err
				else
					parts = message.split ":"
					payload = extend {}, reply
					payload.id = parts[2]
					wss.clients.forEach (client) ->
						client.send JSON.stringify
							command: 'pushData'
							type: parts[1]
							payload: [ payload ]
	else if channel == "__keyevent@0__:del"
		if message.slice(0, "entity:".length) == "entity:"
			parts = message.split(":")
			wss.clients.forEach (client) ->
				client.send JSON.stringify
				 	command: 'delete'
					type: parts[1]
					ids: [ parts[2] ]
	else if channel == "frontend"
		wss.clients.forEach (client) ->
			client.send JSON.stringify
				command: 'message'
				message: message
subscriptions.subscribe "frontend"
subscriptions.subscribe "__keyevent@0__:hset"
subscriptions.subscribe "__keyevent@0__:del"
