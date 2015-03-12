`import Ember from 'ember'`

MoneyDialogComponent = Ember.Component.extend
  classNames: ['modal', 'fade']

  debt: (->
    -@get('userinfo.credits')
  ).property('userinfo')

  debtChanged: (->
    if @get('userinfo.credits') >= 0
      @$().modal('hide')
  ).observes('userinfo')

`export default MoneyDialogComponent`
