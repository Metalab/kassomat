`import Ember from 'ember'`

InsertCoinComponent = Ember.Component.extend
	classNames: ['insert-coin']
	timer: null

	didInsertElement: ->
		@blink()
	willDestroy: ->
		Ember.run.cancel @get 'timer'

	blink: ->
		@$().toggle()
		@set 'timer', Ember.run.later this, "blink", 800

`export default InsertCoinComponent`
