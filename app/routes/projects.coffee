`import Ember from 'ember'`
`import ChildRoute from 'kassomat/routes/childroute'`

ProjectsRoute = ChildRoute.extend
	model: -> @store.find('project')

	actions:
		close: ->
			@transitionTo 'overview'
		showProject: (project) ->
			@transitionTo 'projectinfo', project

`export default ProjectsRoute`
