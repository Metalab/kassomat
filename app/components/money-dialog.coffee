`import Ember from 'ember'`

MoneyDialogComponent = Ember.Component.extend
  classNames: ['modal', 'fade']

  debt: Ember.computed 'userinfo', ->
    -@get('userinfo.credits')

  debtChanged: Ember.computed 'userinfo', ->
    if @get('userinfo.credits') >= 0
      @$().modal('hide')

`export default MoneyDialogComponent`
