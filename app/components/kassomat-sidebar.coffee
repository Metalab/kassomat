`import Ember from 'ember'`

KassomatSidebarComponent = Ember.Component.extend
	classNames: ['sidebar']

	hasMoney: Ember.computed 'model.credits', -> @get('model.credits') != 0

	donate: ->
		socket = this.container.lookup('socket:main').socket
		socket.emit 'action',
			name: 'donate'
	return: ->
		socket = this.container.lookup('socket:main').socket
		socket.emit 'action',
			name: 'return'
	logout: ->
		socket = this.container.lookup('socket:main').socket
		socket.emit 'action',
			name: 'logout'

`export default KassomatSidebarComponent`
