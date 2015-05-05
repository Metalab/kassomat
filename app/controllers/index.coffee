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
			projectId = $('#carousel-projects .item.active').data('id')
			@transitionToRoute 'projectinfo', projectId
		showProjects: ->
			@transitionToRoute 'projects'
		buyPremium: ->
			socket = this.container.lookup('socket:main').socket
			socket.emit 'buy',
				name: 'premium'
		buyMate: ->
			socket = this.container.lookup('socket:main').socket
			socket.emit 'buy',
				name: 'mate'
		buyPop: ->
			socket = this.container.lookup('socket:main').socket
			socket.emit 'buy',
				name: 'pop'
		buyWater: ->
			socket = this.container.lookup('socket:main').socket
			socket.emit 'buy',
				name: 'water'
		buyCoffee: ->
			socket = this.container.lookup('socket:main').socket
			socket.emit 'buy',
				name: 'coffee'


`export default IndexController`
