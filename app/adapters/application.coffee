`import DS from "ember-data"`

KassomatAdapter = DS.RESTAdapter.extend
	namespace: 'api'

`export default KassomatAdapter`
