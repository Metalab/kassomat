`import Ember from 'ember'`
`import Project from 'kassomat/models/project'`

IndexRoute = Ember.Route.extend
	model: ->
		@store.find('project')

	carouselTimer: null
	carouselProgress: ->
		if @get('controller')
			@get('controller').incrementProperty 'currentProjectIndex'
		@set 'carouselTimer', Ember.run.later this, 'carouselProgress', 4000

	activate: ->
		@_super()
		@carouselProgress()
	deactivate: ->
		@_super()
		timer = @get 'carouselTimer'
		if timer
			Ember.run.cancel timer

`export default IndexRoute`
