`import Ember from 'ember'`
`import Resolver from 'ember/resolver'`
`import loadInitializers from 'ember/load-initializers'`
`import config from './config/environment'`

Ember.MODEL_FACTORY_INJECTIONS = true

App = Ember.Application.extend
    modulePrefix: config.modulePrefix
    podModulePrefix: config.podModulePrefix
    Resolver: Resolver
    Socket: EmberSockets.extend
        controllers: ['application', 'overview', 'sidebar']
        autoConnect: true

loadInitializers(App, config.modulePrefix)

Ember.Handlebars.helper 'money', (value, options) ->
    if value == null
        return new Ember.Handlebars.SafeString '$0'
    formatted = parseFloat(value / 100, 10).toFixed 2
    return new Ember.Handlebars.SafeString '€' + formatted

`export default App`
