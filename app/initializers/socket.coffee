initialize = (container, app) ->
  socket = {}

  app.register 'socket:main', socket,
    instantiate: false
  app.inject 'controller', 'socket', 'socket:main'
  app.inject 'adapter', 'socket', 'socket:main'
  app.inject 'route', 'socket', 'socket:main'
  app.inject 'application', 'socket', 'socket:main'
  app.inject 'component', 'socket', 'socket:main'

SocketInitializer =
  name: 'socket'
  initialize: initialize

`export {initialize}`
`export default SocketInitializer`
