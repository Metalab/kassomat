`import Ember from 'ember'`

MoneyDialogComponent = Ember.Component.extend
  classNames: ['modal', 'ui', 'money-dialog']

  setup: Ember.on 'didInsertElement', ->
    @visible()

  debt: Ember.computed 'userinfo', ->
    -@get('userinfo.credits')

  visible: Ember.observer 'userinfo', ->
    if @get('userinfo.credits') < 0
      @$().modal(
        closable: false
      ).modal 'show'
    else
      @$().modal 'hide'

`export default MoneyDialogComponent`
