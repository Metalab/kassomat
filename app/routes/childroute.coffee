`import Ember from 'ember'`

ChildRoute = Ember.Route.extend
	activate: ->
		@_super()
		@controllerFor('application').routeActivated @constructor.toString()
	deactivate: ->
		@_super()
		@controllerFor('application').routeDeactivated @constructor.toString()


`export default ChildRoute`
