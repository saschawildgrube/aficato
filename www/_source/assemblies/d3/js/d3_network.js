
function D3_Network(vSelector, aProps)
{
	var sSvg = d3.select(vSelector);
	if (sSvg.empty() == true)
	{
		console.log('D3_Network: Could not identify element based on selector: '+vSelector);
		return;	
	}
	
	var nSvgWidth = sSvg.node().getBoundingClientRect().width;
	var nSvgHeight = sSvg.node().getBoundingClientRect().height;
	
	var fNodeRadiusMin = 30.0;
	var fNodeRadiusMax = 80.0;

	var aData = aProps['data'];

	var aNodes = aData['nodes'];
	if (typeof aNodes != 'object')
	{
		aNodes = [];
	}
	var aLinks = aData['links'];
	if (typeof aLinks != 'object')
	{
		aLinks = [];
	}
	
	var strCssClassPrefix = GetStringValue(aProps['cssclassprefix']);
	if (strCssClassPrefix == '')
	{
		strCssClassPrefix = 'd3-network-';
	}

	aNodes.forEach(function(node)
	{
		var strId = node['id'];
		if (strId == '')
		{
			console.error('D3_Network: Error: Node has no id: '+RenderValue(node));
		}
		var fRadius = GetNumberValue(node['radius']);
		if (fRadius == 0)
		{
			node['radius'] = fNodeRadiusMin + ((fNodeRadiusMax - fNodeRadiusMin) * 0.5);
		}
		var strText = '';
		var strLabel = GetStringValue(node['label']);
		if (strLabel != '')
		{
			strText += strLabel;
		}
		node['text'] = strText;
	});

	aLinks.forEach(function(link)
	{
		var fDirection = GetNumberValue(link['direction']);
		if (fDirection > 1)
		{
			fDirection = 1.0;
		}
		if (fDirection < 0)
		{
			fDirection = 0.0;
		}
		link['direction'] = fDirection;
		
		var strId = GetStringValue(link['id'])
		if (strId == '')
		{
			link['id'] = link['source']+'-'+link['target'];
		}
	});

	console.log(RenderValue(aData));
	
	var sDefs = sSvg.append('defs');
	
	sDefs.append('marker')
		.attr('id','arrow')
		.attr('viewBox','0 0 10 10')
		.attr('refX','7') 
		.attr('refY','5') 
		.attr('markerWidth','6')
		.attr('markerHeight','6')
		.attr('orient','auto-start-reverse')
		.append('path')
			.attr('d','M 0 0 L 10 5 L 0 10 z')
		;

	function CalcLinkStrokeWidth(link)
	{
		var fStrength = GetNumberValue(link['strength']);
		var nStrokeWidthDefault = 4;
		var nStrokeWidthMin = 1;
		var nStrokeWidthMax = 6;
		var nStrokeWidthSpan = nStrokeWidthMax - nStrokeWidthMin;
		if (fStrength < 0)
		{
			fStrength = 0.0;
		}
		if (fStrength > 1)
		{
			fStrength = 1.0;
		}
		var nStrokeWidth = Math.round(nStrokeWidthMin + nStrokeWidthSpan * fStrength);
		return nStrokeWidth;
	}

  var sLinksArrow1 = sSvg.selectAll('.'+strCssClassPrefix+'link-arrow1')
    .data(aLinks)
    .enter()
    	.append('line')
      	.attr('class', strCssClassPrefix+'link-arrow1')
      	.attr('marker-end','url(#arrow)')
      	.style('stroke-width',function(link){ return CalcLinkStrokeWidth(link); })
     ;

  var sLinksArrow2 = sSvg.selectAll('.'+strCssClassPrefix+'link-arrow2')
    .data(aLinks)
    .enter()
    	.append('line')
      	.attr('class', strCssClassPrefix+'link-arrow2')
      	.attr('marker-end','url(#arrow)')
      	.style('stroke-width',function(link){ return CalcLinkStrokeWidth(link); })
     ;



  var sNodes = sSvg.selectAll('.'+strCssClassPrefix+'node')
		.data(aNodes)
    .enter()
    	.append('g')
      	.attr('class', function(node)
      		{
	     			var strType = GetStringValue(node['type']);
	     			if (strType == '')
	     			{
	     				strType = 'default';
	     			}
	     			var strClass = strCssClassPrefix+'node-'+strType;
      			return strClass;
      		});

  sNodes
  	.append('circle')
      .attr('r', function(node) {	return node.radius;	});

  sNodes
		.append('text')
			.attr('dx', function(node) { return - node.radius + 10; })
			.attr('dy', '.4em')
			.text(function(node) { return node.text; });
 
/*
<svg style="border:1px solid black" >
    <text x="0" y="0" font-size="15" dy="0">
        <tspan x="0" dy=".6em">tspan line 1</tspan>
        <tspan x="0" dy="1.2em">tspan line 2</tspan>
        <tspan x="0" dy="1.2em">tspan line 3</tspan>
    </text>
</svg>

*/      

	const forceX = d3.forceX(nSvgWidth / 2).strength(0.05);
	const forceY = d3.forceY(nSvgHeight / 2).strength(0.05);	

	var simulation = d3.forceSimulation()
		.nodes(aNodes)
		.force('x',forceX)
		.force('y',forceY)
		.force('charge',
			d3.forceManyBody()
				.strength(function(node,index)
					{
						return -200;
					})
			)
		.force('links',
			d3.forceLink(aLinks)
				.distance( function(link,index)
					{
						return 300;
					})
				.id( function(link)
					{
						return link.id;
					})
			)
		.force('collide',
			d3.forceCollide()
				.radius(function(node)
					{
						return node.radius + 20;
					})
			);
	
	sNodes
		.call(
			d3.drag()
				.subject(OnDragGetSubject)
				.on('start', OnDragStart)
				.on('drag', OnDrag)
				.on('end', OnDragEnd)
			);



  simulation.on('tick', function()
  {

  	function CalcLinkArrowEnd(x1,y1,x2,y2,fRadius1,fRadius2,fTransactionDirectionQuotient)
  	{
  		const fHeight = y2-y1;
  		const fWidth = x2-x1;
  		const fSlope = fHeight / fWidth;
  		const fLength = Math.sqrt((fHeight*fHeight) + (fWidth*fWidth));
  		var fNewLength = fRadius1 + ((fLength-fRadius1-fRadius2) * (1-fTransactionDirectionQuotient)) - 10;
  		var fAngle = Math.atan(fSlope);
  		if (fWidth < 0)
  		{
  			fAngle += Math.PI;	
  		}
  		const fNewHeight = Math.sin(fAngle) * fNewLength; 
  		const fNewWidth =  Math.cos(fAngle) * fNewLength; 
    	var pointNew2 = {
  				x: x1 + fNewWidth,
  				y: y1 + fNewHeight
  			};
  		return pointNew2;
  	}
  	
/*  	
    sLinks
			.attr('x1', function(d) { return d.source.x; })
			.attr('y1', function(d) { return d.source.y; })
			.attr('x2', function(d) { return d.target.x; })
			.attr('y2', function(d) { return d.target.y; })
			;
*/

    sLinksArrow1
			.attr('x1', function(d) { return d.source.x; })
			.attr('y1', function(d) { return d.source.y; })
			.attr('x2', function(d) { return CalcLinkArrowEnd(d.source.x,d.source.y,d.target.x,d.target.y,d.source.radius,d.target.radius,d.direction).x; })
			.attr('y2', function(d) { return CalcLinkArrowEnd(d.source.x,d.source.y,d.target.x,d.target.y,d.source.radius,d.target.radius,d.direction).y; })
			;

    sLinksArrow2
			.attr('x1', function(d) { return d.target.x; })
			.attr('y1', function(d) { return d.target.y; })
			.attr('x2', function(d) { return CalcLinkArrowEnd(d.target.x,d.target.y,d.source.x,d.source.y,d.target.radius,d.source.radius,1-d.direction).x; })
			.attr('y2', function(d) { return CalcLinkArrowEnd(d.target.x,d.target.y,d.source.x,d.source.y,d.target.radius,d.source.radius,1-d.direction).y; })
			;

		sNodes
			.attr('transform', function(d)
			{
				return 'translate(' + d.x + ',' + d.y + ')';
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
