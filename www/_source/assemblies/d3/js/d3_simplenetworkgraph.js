'use strict';

function D3_SimpleNetworkGraph(vSelector, aProps)
{
	console.log('D3SimpleNetworkGraph');
	var sSvg = d3.select(vSelector);
	if (sSvg.empty() == true)
	{
		console.log('D3SimpleNetworkGraph: Could not identify element based on selector: '+vSelector);
		return;	
	}
	
	var nSvgWidth = sSvg.node().getBoundingClientRect().width;
	var nSvgHeight = sSvg.node().getBoundingClientRect().height;

	var aData = aProps['data'];

	var aNodes = aData['nodes'];
	var aLinks = aData['links'];

	var strCssClassPrefix = GetStringValue(aProps['cssclassprefix']);
	if (strCssClassPrefix == '')
	{
		strCssClassPrefix = 'd3-simplenetworkgraph-';
	}

  var sLinks = sSvg.selectAll('.'+strCssClassPrefix+'link')
    .data(aLinks)
    .enter()
    	.append("line")
      	.attr("class", strCssClassPrefix+'link')
    		.style("stroke-width", function(d) { return Math.sqrt(d.weight); });

  var sNodes = sSvg.selectAll('.'+strCssClassPrefix+'node')
		.data(aNodes)
    .enter()
    	.append("g")
      	.attr("class", strCssClassPrefix+'node');

  sNodes
  	.append("circle") 
      .attr("r","5");
  
  sNodes
		.append("text")
			.attr("dx", 12)
			.attr("dy", ".35em")
			.text(function(d) { return d.name });

	const forceX = d3.forceX(nSvgWidth / 2).strength(0.05);
	const forceY = d3.forceY(nSvgHeight / 2).strength(0.05);	

	var simulation = d3.forceSimulation()
		.nodes(aNodes)
		.force('x',forceX)
		.force('y',forceY)
		.force("charge",d3.forceManyBody())
		.force("links",d3.forceLink(aLinks)
				.distance( function() { return 40; } )
			);
		
	sNodes
		.call(
			d3.drag()
				.subject(OnDragGetSubject)
				.on("start", OnDragStart)
				.on("drag", OnDrag)
				.on("end", OnDragEnd)
			);

  simulation.on("tick", function()
  {
    sLinks
			.attr("x1", function(d) { return d.source.x; })
			.attr("y1", function(d) { return d.source.y; })
			.attr("x2", function(d) { return d.target.x; })
			.attr("y2", function(d) { return d.target.y; });

		sNodes
			.attr("transform", function(d)
			{
				return "translate(" + d.x + "," + d.y + ")";
			});
  });
  
  
	function OnDragGetSubject()
	{
		return simulation.find(d3.event.x, d3.event.y);
  } 
	function OnDragStart()
	{
	  if (!d3.event.active)
	  {
	  	simulation.alphaTarget(0.3).restart();
	  }
	  d3.event.subject.fx = d3.event.subject.x;
	  d3.event.subject.fy = d3.event.subject.y;
	}
	
	function OnDrag()
	{
	  d3.event.subject.fx = d3.event.x;
	  d3.event.subject.fy = d3.event.y;
	}
	
	function OnDragEnd()
	{
	  if (!d3.event.active)
	  {
	  	simulation.alphaTarget(0);
	  }
	  d3.event.subject.fx = null;
	  d3.event.subject.fy = null;
	}

}
