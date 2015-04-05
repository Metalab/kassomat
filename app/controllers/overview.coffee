`import Ember from 'ember'`

OverviewController = Ember.Controller.extend
	updatedProjects: Ember.observer 'projects.[]', ->
		projects = @get 'projects'
		if projects.get('length') > 0
			projects.setEach 'active', false
			projects.get('firstObject').set 'active', true

	actions:
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


`export default OverviewController`
