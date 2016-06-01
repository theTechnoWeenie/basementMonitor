console.log('Basement humidity monitor 0.1');

var graph = new Rickshaw.Graph( {
  element: document.querySelector('#graph'),
  series: [
    {
      color: 'steelblue',
      data: [ { x: 0, y: 23}, { x: 1, y: 15 }, { x: 2, y: 79 } ]
    }, {
      color: 'lightblue',
      data: [ { x: 0, y: 30}, { x: 1, y: 20 }, { x: 2, y: 64 } ]
    }
  ]
} );

graph.render();
