`import DS from 'ember-data'`

ProjectSerializer = DS.RESTSerializer.extend
	attrs:
		active: { exclude: true }

`export default ProjectSerializer`
