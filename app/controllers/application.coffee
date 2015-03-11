`import Ember from 'ember'`

ApplicationController = Ember.Controller.extend
	overviewTimerActive: false
	timer: null

	userinfo: {
		username: ""
		credits: 0
	}

	routeActivated: (name) ->
		@activateOverviewTimer()
	routeDeactivated: (name) ->
		@set 'overviewTimerActive', false
		Ember.run.cancel @get 'timer'

	transitionToOverview: ->
		@transitionToRoute 'overview'

	debounceOverviewTimer: ->
		if @get('overviewTimerActive')
        	@set 'timer', Ember.run.debounce(this, 'transitionToOverview', 10000)

	activateOverviewTimer: ->
		@set 'overviewTimerActive', true
		@debounceOverviewTimer()

	setupOverviewTimer: (->
		debounce = @debounceOverviewTimer.bind this

		$(document).mousemove debounce
		$(document).mousedown debounce
		$(document).mouseup debounce
		$(document).keydown debounce
		$(document).keyup debounce
	).on 'init'

	sockets:
		userinfo: (record) ->
			@set 'userinfo', record
		pushPayload: (payload) ->
			@store.pushPayload payload

`export default ApplicationController`
