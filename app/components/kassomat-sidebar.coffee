`import Ember from 'ember'`

KassomatSidebarComponent = Ember.Component.extend
	classNames: ['sidebar']

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
