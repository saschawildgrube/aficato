'use strict';

function D3_DivergingStackedBarChart(vSelector, aProps)
{
	var sSvg = d3.select(vSelector);
	if (sSvg.empty() == true)
	{
		console.log('D3DivergingStackedBarChart: Could not identify element based on selector: '+vSelector);
		return;	
	}
	
	var aLegends = aProps['legends'];
	var aData = aProps['data'];
	
	if (aLegends.length != 5)
	{
		console.error('aLegends must be an array containing 5 strings!');
		return;	
	}

	var strCssClassPrefix = GetStringValue(aProps['cssclassprefix']);
	if (strCssClassPrefix == '')
	{
		strCssClassPrefix = 'd3-divergingstackedbarchart-';
	}

	
	var nSections = aLegends.length;
	
	var nSvgWidth = sSvg.node().getBoundingClientRect().width;
	var nSvgHeight = sSvg.node().getBoundingClientRect().height;
	
		
	var rMargin = {top: 50, right: 20, bottom: 10, left: 65};
	var nChartWidth = nSvgWidth - rMargin.left - rMargin.right;
	var nChartHeight = nSvgHeight - rMargin.top - rMargin.bottom;

	function GetCssClassFromColumn(nSections, nColumn)
	{
		if (nSections != 5)
		{
			console.error('D3DivergingStackedBarChart() supports 5 sections only!');	
			$nColumn = 3;
		}
		var aClassMapping = ['left2','left1','center','right1','right2'];
		const strPostfix = aClassMapping[nColumn-1];
		const strClassName = strCssClassPrefix + strPostfix;
		return strClassName;	
	}
	
  aData.forEach(function(row)
  {
  	var nColumn;
  	// Make sure data is intepreted as integers
  	for (nColumn = 1; nColumn <= nSections; nColumn++)
  	{
  		row[nColumn] = parseInt(row[nColumn]);
  	}
  	
  	// Calculate the total of each row
  	row['N'] = +row[1]+row[2]+row[3]+row[4]+row[5];
  	
  	// Calculate the percentage values
  	row['P'] = []; 
		for (nColumn = 1; nColumn <= nSections; nColumn++)
		{
			row.P[nColumn] = row[nColumn]*100/row.N;
		}

		// Construct the individual boxes of the bar
    var x01 = -1*(row.P[3]/2+row.P[2]+row.P[1]);
    var idx = 0;
    row['boxes'] = [];
    for (nColumn = 1; nColumn <= nSections; nColumn++)
	  {
	  	var box = {
				cssclass: GetCssClassFromColumn(nSections,nColumn),
  			x0: x01,
  			x1: x01 += +row.P[nColumn],
  			N: +row.N,
  			n: +row[idx += 1]
			};
			row.boxes.push(box);
		}
	});
			
	var nMin = d3.min(aData,function(row)
		{
	  	return row.boxes[0].x0;
	  });
	
	var nMax = d3.max(aData,function(row)
		{
			return row.boxes[nSections-1].x1;
	  });

	var scaleY = d3.scaleBand().
		range([0, nChartHeight])
		.padding(.3)
		.domain(aData.map(function(d) { return d.Question; }))
	
	var scaleX = d3.scaleLinear()
		.rangeRound([0, nChartWidth])
		.domain([nMin, nMax])
		.nice();

	var axisX = d3.axisTop().scale(scaleX);
	var axisY = d3.axisLeft().scale(scaleY);

	var sChart = sSvg.append('g')
    .attr('transform', 'translate(' + rMargin.left + ',' + rMargin.top + ')');

	sChart.append('g')
		.attr('class', 'x d3-divergingstackedbarchart-axis')
		.call(axisX);
	
	sChart.append('g')
		.attr('class', 'y d3-divergingstackedbarchart-axis')
	  .call(axisY)
	
	var sBoxes = sChart.selectAll('.question')
		.data(aData)
		.enter()
			.append('g')
				.attr('class', 'bar')
				.attr('transform', function(row) { return 'translate(0,' + scaleY(row.Question) + ')'; });
	
	var sBars = sBoxes.selectAll('rect')
		.data(function(row) { return row.boxes; })
		.enter()
	  	.append('g')
	  		.attr('class', 'subbar');
	
	sBars.append('rect')
		.attr('height', scaleY.bandwidth())  
		.attr('x', function(box) { return scaleX(box.x0); })
		.attr('width', function(box) { return scaleX(box.x1) - scaleX(box.x0); })
		.attr('class',function(d,i) { return GetCssClassFromColumn(nSections,i+1); });
	
	sBars.append('text')
    .attr('x', function(box) { return scaleX(box.x0); })
    .attr('y', scaleY.bandwidth()/2)
    .attr('dy', '0.5em')
    .attr('dx', '0.5em')
    .attr('class',strCssClassPrefix+'bar-text') 
    .style('text-anchor', 'begin')
    .text(function(box) { return box.n !== 0 && (box.x1-box.x0)>3 ? box.n : '' });
	
	sBoxes.insert('rect',':first-child')
	    .attr('height', scaleY.bandwidth())
	    .attr('x', '1')
	    .attr('width', nChartWidth)
	    .attr('fill-opacity', '0.5')
	    .attr('class', strCssClassPrefix+'bar-background ' + function(row,index)
	    	{
	    		return index%2==0 ? 'even' : 'uneven';
	    	});
	
	sChart.append('g')
	    .attr('class', 'y '+strCssClassPrefix+'axis')
			.append('line')
	    	.attr('x1', scaleX(0))
	    	.attr('x2', scaleX(0))
	    	.attr('y2', nChartHeight);

	var sLegendBox = sChart.append('g').attr('class', 'legendbox').attr('id', 'mylegendbox');
	// this is not nice, we should calculate the bounding box and use that
	var legend_tabs = [0, 120, 200, 375, 450];
	var sLegends = sLegendBox.selectAll('.legend')
	  	.data(aLegends)
	  	.enter()
	  		.append('g')
	    		.attr('class', 'legend')
	    		.attr('transform', function(d, i) { return 'translate(' + legend_tabs[i] + ',-45)'; });
	
	sLegends.append('rect')
	    .attr('x', 0)
	    .attr('width', 18)
	    .attr('height', 18)
	    .attr('class',function(d,i) { return GetCssClassFromColumn(nSections,i+1); });
	
	sLegends.append('text')
	    .attr('x', 22)
	    .attr('y', 9)
	    .attr('dy', '.35em')
	    .attr('class',strCssClassPrefix+'legend-text')
	    .style('text-anchor', 'begin')
	    .text(function(d) { return d; });
	
	
	var movesize = nChartWidth/2 - sLegendBox.node().getBBox().width/2;
	d3.selectAll('.legendbox').attr('transform', 'translate(' + movesize  + ',0)');
	

}
