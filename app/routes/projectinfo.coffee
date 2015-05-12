`import Ember from 'ember'`
`import ChildRoute from 'kassomat/routes/childroute'`

ProjectinfoRoute = ChildRoute.extend
	model: (params) -> @store.find('project', params.id)

`export default ProjectinfoRoute`
