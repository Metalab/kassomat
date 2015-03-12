`import Ember from 'ember'`

SidebarController = Ember.Controller.extend
	actions:
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


`export default SidebarController`
