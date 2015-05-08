`import Ember from 'ember'`

KassomatSidebarComponent = Ember.Component.extend
	classNames: ['sidebar']

	hasMoney: Ember.computed 'model.credits', -> @get('model.credits') != 0

	showChange: false

	actions:
		donate: ->
			socket = @get('socket.socket')
			socket.send JSON.stringify
				eventType: 'action'
				name: 'donate'
		change: ->
			@set 'showChange', true
		return: ->
			socket = @get('socket.socket')
			socket.send JSON.stringify
				eventType: 'action'
				name: 'return'
		logout: ->
			socket = @get('socket.socket')
			socket.send JSON.stringify
				eventType: 'action'
				name: 'logout'

`export default KassomatSidebarComponent`
