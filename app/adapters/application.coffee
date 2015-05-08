`import Ember from 'ember'`
`import DS from "ember-data"`
`import uuid from 'kassomat/utils/uuid'`

SocketAdapter = DS.Adapter.extend
  queue: []

  requests: {}

  doRequest: (request) ->
    r = Ember.copy request
    if request.resolve or request.reject
      r.id = uuid()
      @get('requests')[r.id] = (data) ->
        if data.status
          request.resolve data.content
        else
          request.reject data
    @get('socket.socket').send JSON.stringify r

  init: ->
    console.log "STARTING SOCKET ADAPTER"

    socket = @get 'socket'
    socket.socket = new WebSocket("ws://%@:3000/ws".fmt(document.location.hostname), "kassomat")

    console.log "[Adapter] Waiting for connection"
    socket.socket.onopen = =>
      console.log "[Adapter] App.Socket connected, flushing queue"
      @queue.forEach (q) =>
        @doRequest q
      @queue.length = 0
    socket.socket.onmessage = (event) =>
      data = JSON.parse event.data
      if data.command == 'reply' and data.id
        handler = @get('requests')[data.id]
        if handler
          handler data
          delete handler[data.id]
      else
        applicationController = @container.lookup("controller:application")
        handlers = applicationController.get('sockets')
        if handlers[data.command]
          handlers[data.command].call(applicationController, data)
        else
          console.log "Unhandled command", data.command, data
    Ember.onerror = (error) =>
      req =
        eventType: 'log'
        message: error.toString()
      if socket.socket.readyState != 1
        @queue.push req
      else
        @doRequest req

      console.error.apply console, arguments
    log = console.log
    console.log = =>
      req =
        eventType: 'log'
        message: Array.prototype.slice.call arguments
      if socket.socket.readyState != 1
        @queue.push req
      else
        @doRequest req

      log.apply console, arguments
    @_super()

  makeSocketRequest: (store, eventType, query) ->
    new Ember.RSVP.Promise (resolve, reject) =>
      req =
        eventType: eventType
        query: query
        resolve: resolve
        reject: reject
      if @get('socket.socket').readyState != 1
        @queue.push req
      else
        @doRequest req

  find: (store, type, id) ->
    console.log '[Adapter] FIND: ', type.typeKey
    @makeSocketRequest store, 'find',
      type: type.typeKey.charAt(0).toUpperCase() + type.typeKey.slice(1)
      query:
        id: id

  findAll: (store, type, id) ->
    console.log '[Adapter] FINDALL: ', type.typeKey, id
    @makeSocketRequest store, 'findAll',
      type: type.typeKey.charAt(0).toUpperCase() + type.typeKey.slice(1)
      query:
        since: id

  findQuery: (store, type, query) ->
    @makeSocketRequest store, 'findQuery',
      type: type.typeKey.charAt(0).toUpperCase() + type.typeKey.slice(1)
      query: query
  findMany: (store, type, ids, records) ->
    @makeSocketRequest store, 'findMany',
      type: type.typeKey.charAt(0).toUpperCase() + type.typeKey.slice(1)
      query:
        ids: ids
  findBelongsTo: (store, record, url) ->
    id = @get record, 'id'
    type = record.constructor.typeKey
    console.log '[Adapter] findBelongsTo: ', type, url
    @makeSocketRequest store, 'findBelongsTo',
      type: type.charAt(0).toUpperCase() + type.slice(1)
      query:
        id: id
  findHasMany: (store, record, url) ->
    id = @get record, 'id'
    type = record.constructor.typeKey
    console.log '[Adapter] findHasMany: ', type, url
    @makeSocketRequest store, 'findHasMany',
      type: type.charAt(0).toUpperCase() + type.slice(1)
      query:
        id: id

`export default SocketAdapter`
