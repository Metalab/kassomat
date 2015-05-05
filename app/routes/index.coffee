`import Ember from 'ember'`
`import Project from 'kassomat/models/project'`

IndexRoute = Ember.Route.extend
	model: ->
		@store.find('project')

`export default IndexRoute`
