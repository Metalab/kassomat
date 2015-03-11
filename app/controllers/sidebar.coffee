`import Ember from 'ember'`

SidebarController = Ember.Controller.extend
	actions:
		donate: ->
			socket = this.container.lookup('socket:main').socket
			socket.emit 'action',
				name: 'donate'
		redeem: ->
			socket = this.container.lookup('socket:main').socket
			socket.emit 'action',
				name: 'redeem'
		return: ->
			socket = this.container.lookup('socket:main').socket
			socket.emit 'action',
				name: 'return'


`export default SidebarController`
