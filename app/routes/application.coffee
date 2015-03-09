`import Ember from 'ember'`

ApplicationRoute = Ember.Route.extend
	setupController: (controller, model) ->
		controller.set 'userinfo', @store.find('userinfo', 0)

`export default ApplicationRoute`
