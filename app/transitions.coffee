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
	@transition(
		@hasClass('sidebar-action'),
		@toValue(true),
		@use('toUp'),
		@reverse('toDown')
	)
	@transition(
		@fromRoute('index'),
		@toRoute('projects'),
		@use('crossFade'),
		@reverse('crossFade')
	)

`export default Transitions`
