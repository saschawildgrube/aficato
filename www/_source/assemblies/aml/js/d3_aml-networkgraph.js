
function D3_AML_NetworkGraph(vSelector, aProps)
{
	var sSvg = d3.select(vSelector);
	if (sSvg.empty() == true)
	{
		console.log('D3AMLNetworkGraph: Could not identify element based on selector: '+vSelector);
		return;	
	}
	
	const strCssClassPrefix = 'aml-networkgraph-';
	
	var nSvgWidth = sSvg.node().getBoundingClientRect().width;
	var nSvgHeight = sSvg.node().getBoundingClientRect().height;
	
	var fNodeRadiusMin = 55.0;
	var fNodeRadiusMax = 100.0;

	var aData = aProps['data'];

	var aNodes = aData['nodes'];
	var aLinks = aData['links'];

	aNodes.forEach(function(node)
	{
		node['radius'] = fNodeRadiusMin + ((fNodeRadiusMax - fNodeRadiusMin) * node.transaction_amount_factor);
	});
	
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
		if (link.transaction_count_out <= 1)
			return '3';
		if (link.transaction_count_out <= 4)
			return '4';
		return '5';
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
	     			var strClass = strCssClassPrefix+'node';
      			if (node.dataquality < 1)
      			{
      				strClass = strClass+' '+strCssClassPrefix+'node-neutral';
      			}
      			else if (node.transaction_netflow > 0)
      			{
      				strClass = strClass+' '+strCssClassPrefix+'node-positive';
      			}
      			else
      			{
      				strClass = strClass+' '+strCssClassPrefix+'node-negative';
      			}
      			return strClass;
      		});


  sNodes
  	.append('circle')
      .attr('r', function(node) {	return node.radius;	});
 
	const bHideText = aProps['hidetext'];

	if (!bHideText)
	{
	  sNodes
			.append('text')
				.attr('dx', function(node) { return 15 - node.radius; })
				.attr('dy', -20)
				.text(function(node) { return node.name; });
					
	  sNodes
			.append('text')
				.attr('dx', function(node) { return 10 - node.radius; })
				.attr('dy', 0)
				.text(function(node) { return '+'+node.text_transaction_amount_in+' -'+node.text_transaction_amount_out; });				
	
	  sNodes 
			.append('text')
				.attr('dx', function(node) { return 15 - node.radius; })
				.attr('dy', 20)
				.text(function(node) { return '\u0394'+node.text_transaction_netflow+' #'+node.transaction_count; });				
	
	  sNodes
			.append('text')
				.attr('dx', function(node) { return 30 - node.radius; })
				.attr('dy', 40)
				.text(function(node) { return node.currency; });
	}		

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
			.attr('x2', function(d) { return CalcLinkArrowEnd(d.source.x,d.source.y,d.target.x,d.target.y,d.source.radius,d.target.radius,d.transaction_direction_quotient).x; })
			.attr('y2', function(d) { return CalcLinkArrowEnd(d.source.x,d.source.y,d.target.x,d.target.y,d.source.radius,d.target.radius,d.transaction_direction_quotient).y; })
			;

    sLinksArrow2
			.attr('x1', function(d) { return d.target.x; })
			.attr('y1', function(d) { return d.target.y; })
			.attr('x2', function(d) { return CalcLinkArrowEnd(d.target.x,d.target.y,d.source.x,d.source.y,d.target.radius,d.source.radius,1-d.transaction_direction_quotient).x; })
			.attr('y2', function(d) { return CalcLinkArrowEnd(d.target.x,d.target.y,d.source.x,d.source.y,d.target.radius,d.source.radius,1-d.transaction_direction_quotient).y; })
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
