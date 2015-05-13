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

loadInitializers(App, config.modulePrefix)

Ember.Handlebars.helper 'money', (value, options) ->
  if value == null
    return new Ember.Handlebars.SafeString '$0'
  if value % 100 == 0
    formatted = parseFloat(value / 100, 10).toFixed 0
  else
    formatted = parseFloat(value / 100, 10).toFixed 2
  return new Ember.Handlebars.SafeString '€' + formatted

`export default App`
