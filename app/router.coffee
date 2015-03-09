`import Ember from 'ember'`
`import config from './config/environment'`

Router = Ember.Router.extend
    location: config.locationType

Router.map ->
    @route 'overview',
        path: '/'
    @route 'projects'
    @route 'projectinfo',
        path: '/projects/:id'

`export default Router`
