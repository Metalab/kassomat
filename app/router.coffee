`import Ember from 'ember'`
`import config from './config/environment'`

Router = Ember.Router.extend
    location: config.locationType

Router.map ->
    @route 'projects'
    @route 'projectinfo',
        path: '/project/:id'

`export default Router`
