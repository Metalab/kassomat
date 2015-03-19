`import Ember from 'ember'`
`import Resolver from 'ember/resolver'`
`import loadInitializers from 'ember/load-initializers'`
`import config from './config/environment'`

Ember.MODEL_FACTORY_INJECTIONS = true

App = Ember.Application.extend
  modulePrefix: config.modulePrefix
  podModulePrefix: config.podModulePrefix
  Resolver: Resolver
  LOG_TRANSITIONS: true
  Socket: EmberSockets.extend
    controllers: ['application']
    autoConnect: true
    port: 3000

Ember.Application.initializer
  name: 'logging'
  initialize: (container, application) ->
    log = console.log.bind(console)
    console.log = ->
      args = Array.prototype.slice.call(arguments)
      socket = container.lookup('socket:main').socket
      socket.emit 'log', encodeURI(args.map( (o) ->
        if typeof o == "object"
          JSON.stringify(o)
        else if typeof o == "undefined"
          "undefined"
        else
          o.toString()
      ).join(" "))
      log.apply console, args
    Ember.onerror = (error) ->
      socket = container.lookup('socket:main').socket
      socket.emit 'log', encodeURI(error.toString())

loadInitializers(App, config.modulePrefix)

Ember.Handlebars.helper 'money', (value, options) ->
  if value == null
    return new Ember.Handlebars.SafeString '$0'
  formatted = parseFloat(value / 100, 10).toFixed 2
  return new Ember.Handlebars.SafeString '€' + formatted

`export default App`
