`import Ember from 'ember'`
`import DS from "ember-data"`

SocketAdapter = DS.Adapter.extend
	needs: ['application']
	queue: []

	doRequest: (request) ->
		console.log '[Adapter] request', request.query
		@socket.emit request.eventType, request.query, (data) ->
			console.log '[Adapter] response?', data
			if data.status
				request.resolve data.content
			else
				request.reject data

	init: ->
		console.log("STARTING SOCKET ADAPTER");

		socket = this.container.lookup('socket:main').socket;
		@socket = socket
		if not socket.connected
			socket.on 'connect', =>
				console.log "[Adapter] App.Socket connected, flushing queue"
				@queue.forEach (q) =>
					@doRequest q
				@queue.length = 0
		@_super();

	makeSocketRequest: (store, eventType, query) ->
		new Ember.RSVP.Promise (resolve, reject) =>
			try
				if not @socket.connected
					@queue.push
						eventType: eventType
						query: query
						resolve: resolve
						reject: reject
				else if @socket.authenticated
					@doRequest
						eventType: eventType
						query: query
						resolve: resolve
						reject: reject
				else
					console.log '[Adapter] App.Socket is not authenticated'
					reject { message: 'App.Socket is not authenticated' }
			catch e
				console.error '[Adapter] App.Socket is not available for requests', e
				reject { message: 'App.Socket is not available for requests' }

	find: (store, type, id) ->
		console.log '[Adapter] FIND: ', type.typeKey
		@makeSocketRequest store, 'find',
			type: type.typeKey
			query:
				id: id

	findAll: (store, type, id) ->
		console.log '[Adapter] FINDALL: ', type.typeKey, id
		@makeSocketRequest store, 'findAll',
			type: type.typeKey
			query:
				since: id

	findQuery: (store, type, query) ->
		@makeSocketRequest store, 'findQuery',
			type: type.typeKey
			query: query
	findMany: (store, type, ids, records) ->
		@makeSocketRequest store, 'findMany',
			type: type.typeKey
			query:
				ids: ids
	findBelongsTo: (store, record, url) ->
		id = @get record, 'id'
		type = record.constructor.typeKey
		console.log '[Adapter] findBelongsTo: ', type, url
		@makeSocketRequest store, 'findBelongsTo',
			type: type
			query:
				id: id
	findHasMany: (store, record, url) ->
		id = @get record, 'id'
		type = record.constructor.typeKey
		console.log '[Adapter] findHasMany: ', type, url
		@makeSocketRequest store, 'findHasMany',
			type: type,
			query:
				id: id

`export default SocketAdapter`
