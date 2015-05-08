Transitions = ->
	@transition(
		@hasClass('carousel-item'),
		@use('toLeft')
	)
	@transition(
		@hasClass('user'),
		@toValue(true),
		@use('toDown')
	)
	@transition(
		@hasClass('user'),
		@toValue(false),
		@use('toUp')
	)
	@transition(
		@hasClass('creditsValue'),
		@use('toDown')
	)
	@transition(
		@hasClass('money'),
		@use('toDown')
	)

`export default Transitions`
