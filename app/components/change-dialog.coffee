`import Ember from 'ember'`

ChangeDialogComponent = Ember.Component.extend
	classNames: ['modal', 'fade']

	levels: null
	units: null
	levelsUpdated: (->
		levels = @get('levels')
		units = @get 'units'
		if levels
			@set 'units', levels.filterBy('count').map (level) ->
				if units
					oldUnit = units.findBy('denomination', level.id)
					if oldUnit
						oldUnit.set 'crossedout', level.count <= oldUnit.count
				else
					oldUnit = null

				oldUnit || Ember.Object.create
						denomination: level.id
						url: "images/euros/" + level.id + ".png"
						count: 0
						crossedout: level.count == 0
		else
			@set 'units', []
	).observes('levels')

	generateUnits: (->
		@store.findAll('DenominationLevel').then (levels) =>
			@set 'levels', levels
	).on('init')

	sum: (->
		value = 0
		units = @get('units')
		if units
			units.forEach (unit) ->
				value += unit.get('count') * unit.get('denomination')
		return value
	).property('units.@each.count')

	disabled: (->
		@get('sum') == 0 or @get('sum') > @get('userinfo.credits')
	).property('sum', 'userinfo.credits')

	actions:
		addUnit: (unit) ->
			@store.find('DenominationLevel', unit.denomination).then (level) ->
				if level.get('count') > unit.get('count')
					unit.incrementProperty('count')
				unit.set 'crossedout', level.get('count') <= unit.get('count')
		removeUnit: (unit) ->
			if unit.get('count') > 0
				@store.find('DenominationLevel', unit.denomination).then (level) ->
					unit.decrementProperty('count')
					unit.set 'crossedout', level.get('count') <= unit.get('count')
		accept: ->
			units = @get('units').filterBy('count').map (u) ->
				u.getProperties(['denomination', 'count'])
			socket = this.container.lookup('socket:main').socket
			socket.emit 'action',
				name: 'change'
				options: units


`export default ChangeDialogComponent`
