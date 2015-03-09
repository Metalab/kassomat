`import Ember from 'ember'`

ChangeDialogComponent = Ember.Component.extend
	classNames: ['modal', 'fade']

	denominations: [ 10, 20, 100, 200, 500, 1000, 2000, 5000, 10000 ]

	generateUnits: (->
		@set 'units', @get('denominations').map (u) ->
			Ember.Object.create
				value: u
				url: "images/euros/" + u + ".png"
				count: 0
	).on('init')

	summary: (->
		summary = []
		value = 0
		@get('units').forEach (unit) ->
			if unit.get('count') > 0
				summary.push "%@x €%@".fmt(unit.get('count'), parseFloat(unit.get('value') / 100, 10).toFixed(2))
				value += unit.get('count') * unit.get('value')
		if summary.length > 0
			(summary.join " + ") + " = €" + parseFloat(value / 100, 10).toFixed(2)
		else
			""
	).property('units.@each.count')

	actions:
		addUnit: (value) ->
			@get('units').findBy('value', value).incrementProperty('count');

`export default ChangeDialogComponent`
