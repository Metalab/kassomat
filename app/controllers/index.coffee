`import Ember from 'ember'`

IndexController = Ember.Controller.extend
	updatedProjects: Ember.observer 'model.[]', ->
		projects = @get 'model'
		if projects.get('length') > 0
			projects.setEach 'active', false
			projects.get('firstObject').set 'active', true

	_currentProjectIndex: 0
	currentProjectIndex: Ember.computed '_currentProjectIndex', (key, value) ->
		if value != undefined
			if value < 0
				value = @get('model.length') - 1
			if (value % @get('model.length')) != @get('_currentProjectIndex')
				@set '_currentProjectIndex', (value % @get('model.length'))
		@get '_currentProjectIndex'
	currentProject: Ember.computed 'model.[]', 'currentProjectIndex', ->
		@get('model').objectAt @get('currentProjectIndex')

	actions:
		next: ->
			@incrementProperty 'currentProjectIndex'
		prev: ->
			@decrementProperty 'currentProjectIndex'
		showActiveProject: ->
			projectId = @get('currentProject.id')
			@transitionToRoute 'projectinfo', projectId
		showProjects: ->
			@transitionToRoute 'projects'
		buyPremium: ->
			socket = @get('socket.socket')
			socket.send JSON.stringify
				eventType: 'buy'
				name: 'premium'
		buyMate: ->
			socket = @get('socket.socket')
			socket.send JSON.stringify
				eventType: 'buy'
				name: 'mate'
		buyPop: ->
			socket = @get('socket.socket')
			socket.send JSON.stringify
				eventType: 'buy'
				name: 'pop'
		buyWater: ->
			socket = @get('socket.socket')
			socket.send JSON.stringify
				eventType: 'buy'
				name: 'water'
		buyCoffee: ->
			socket = @get('socket.socket')
			socket.send JSON.stringify
				eventType: 'buy'
				name: 'coffee'


`export default IndexController`
