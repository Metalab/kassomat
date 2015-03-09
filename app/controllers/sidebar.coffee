`import Ember from 'ember'`

SidebarController = Ember.Controller.extend
	actions:
		change: ->
			console.log "change"
		donate: ->
			$.post('/api/actions/donate')
		redeem: ->
			$.post('/api/actions/redeem')
		return: ->
			$.post('/api/actions/return')


`export default SidebarController`
