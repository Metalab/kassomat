`import Ember from 'ember'`

OverviewController = Ember.Controller.extend
	updatedProjects: (->
		projects = @get 'projects'
		if projects.get('length') > 0
			projects.setEach 'active', false
			projects.get('firstObject').set 'active', true
	).observes('projects.[]')

	actions:
		showActiveProject: ->
			projectId = $('#carousel-projects .item.active').data('id')
			@transitionToRoute 'projectinfo', projectId
		showProjects: ->
			@transitionToRoute 'projects'



`export default OverviewController`
