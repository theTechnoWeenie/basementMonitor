console.log('Basement humidity monitor 0.1');

// var humidity = new Rickshaw.Graph( {
//   element: document.querySelector('#humidity'),
//   series: [
//     {
//       color: 'steelblue',
//       data: [ { x: 0, y: 23}, { x: 1, y: 15 }, { x: 2, y: 79 } ]
//     }, {
//       color: 'lightblue',
//       data: [ { x: 0, y: 30}, { x: 1, y: 20 }, { x: 2, y: 64 } ]
//     }
//   ]
// } );
// humidity.render();
scales = [
  d3.scale.linear().domain([0, 100]).nice(),
  d3.scale.linear().domain([0, 30]).nice(),
  d3.scale.pow().domain([800, 1200]).nice()
]
var ajaxGraph = new Rickshaw.Graph.Ajax( {
	element: document.querySelector("#humidity"),
	renderer: 'line',
	dataURL: 'http://data.sparkfun.com/output/XGY7yw3OM8tznv85g7Oq.json',
	onData: function(d) {
    var i = 0;
    var j = 0;
    var k = 0;

    var humidity = _.flatMap(d, function(item){
      return {y: Number(item.humidity), x: i++};
    });
    var temp = _.flatMap(d, function(item){
      return { y: Number(item.temp), x: j++};
    });
    var pressure = _.flatMap(d, function(item){
      return { y: Number(item.pressure), x: k++};
    });
    return [{ key: 'Humidity', data: humidity }, { key: 'Temperature', data: temp}, { key: 'Pressure', data: pressure}];
  },
	series: [
		{
			name: 'Humidity',
			color: 'blue',
      scale: scales[0]
		},
    {
      name: 'Temperature',
      color: 'green',
      scale: scales[1]
    },
    // {
    //   name: 'Pressure',
    //   color: 'orange',
    //   scale: scales[2]
    // }
	]
} );
