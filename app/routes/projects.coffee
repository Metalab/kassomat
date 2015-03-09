`import Ember from 'ember'`

ProjectsRoute = Ember.Route.extend
	model: -> @store.find('project')

	actions:
		close: ->
			@transitionTo 'overview'
		showProject: (project) ->
			@transitionTo 'projectinfo', project

`export default ProjectsRoute`
