`import Ember from 'ember'`
`import Project from 'kassomat/models/project'`

OverviewRoute = Ember.Route.extend
	setupController: (controller, model) ->
		controller.set 'projects', @store.find('project')


`export default OverviewRoute`
