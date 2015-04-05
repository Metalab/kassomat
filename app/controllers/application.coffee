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

  userinfoChanged: Ember.observer 'userinfo', ->
    if @get('userinfo.credits') < 0
      $('#moneydialog').modal
        backdrop: "static"

  sockets:
    userinfo: (record) ->
      @set 'userinfo',
        username: decodeURI(record.username)
        credits: record.credits
    pushData: (data) ->
      payload = Ember.copy data.payload
      payload.forEach (obj) -> Object.keys(obj).forEach (key) -> if obj.hasOwnProperty(key) then obj[key] = decodeURI(obj[key])
      @store.pushMany data.type, data.payload
    delete: (data) ->
      data.ids.forEach (id) =>
        record = @store.getById(data.type, id)
        @store.unloadRecord record if record
    message: (message) ->
      console.log "Received message:", message

`export default ApplicationController`
