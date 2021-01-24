

function D3_LineChart(vSelector, aProps)
{
	var sSvg = d3.select(vSelector);
	if (sSvg.empty() == true)
	{
		console.log('D3_LineChart: Could not identify element based on selector: '+vSelector);
		return;	
	}

	var aData = aProps['data'];
	if (aData == undefined)
	{
		console.log('D3_LineChart: No data to display.');
		return;	
	}

	var config = aProps['config'];
	if (config == undefined)
	{
		config = {};
		aFirstRow = aData[0];
		if (aFirstRow.length == 0)
		{
			console.log('D3_LineChart: No config provided and the first row is empty.');
			return;	
		}
		
		var aColumns = Object.keys(aFirstRow);
		if (aColumns.length == 0)
		{
			console.log('D3_LineChart: First row is not an associative array or object.');
			return;	
		}
		config['legend'] = aColumns[0];
		config['lines'] = [];
		for (var nColumn = 1; nColumn < aColumns.length; nColumn++)
		{
			config['lines'].push(
			{
				'key': aColumns[nColumn],
				'label': aColumns[nColumn],
				'color': 'black',
				'weight': 2
			});
		}
	}

	if (config.legend == undefined)
	{
		console.log('D3_LineChart: Legend key is not defined.');
		return;
	}
	
	if (config.lines == undefined)
	{
		console.log('D3_LineChart: lines are not defined.');
		return;
	}
	
	var aLines = config.lines;	
	for (var nLine = 0; nLine < aLines.length; nLine++)
	{
		if (aLines[nLine].weight == undefined || aLines[nLine].weight == 0)
		{
			aLines[nLine].weight = 2;
		}
		if (aLines[nLine].color == undefined || aLines[nLine].color == '')
		{
			aLines[nLine].color = 'black';
		}
	}


	
	var strMetric = '';
	if (config.metric != undefined)
	{
		strMetric = config.metric;
	}
	
	var strLegendPriority = null;
	if (config.metric != undefined)
	{
		strLegendPriority = config.legendpriority;
	}
	
	const strCssClassPrefix = 'linechart-';
	
	var nSvgWidth = sSvg.node().getBoundingClientRect().width;
	var nSvgHeight = sSvg.node().getBoundingClientRect().height;

	var nBorderTop = 40;	
	var nBorderLeft = 80;
	var nBorderRight = 5;
	var nBorderBottom = 40;
	
	var nChartHeight = Math.max(0,nSvgHeight-nBorderTop-nBorderBottom);
	var nChartWidth = Math.max(0,nSvgWidth-nBorderLeft-nBorderRight);


	var fMax = d3.max(aData,function(row)
		{
			var aValues = [];
			aLines.forEach(function(line)
			{
				aValues.push(row[line.key]); 
			});
			return Math.max(...aValues); 
	  });

	var fMin = d3.min(aData,function(row) 
		{
			var aValues = [];
			aLines.forEach(function(line)
			{
				aValues.push(row[line.key]); 
			});
			return Math.min(...aValues);
	  });

	const fMaxAbs = Math.max(fMax,-fMin);

	var scaleY = d3.scaleLinear()
		.rangeRound([0, nChartHeight])
		.domain([fMax*1.1, fMin*1.1])
		.nice();

	var nIndex = -1;
	var scaleX = d3.scaleBand()
		.rangeRound([0, nChartWidth])
		.padding(.3)
		.domain(aData.map(function(row)
			{
				nIndex++;
				return nIndex;
			}))

	var axisY = d3.axisLeft()
		.scale(scaleY)
    .tickFormat(function(fScale)
    {
    	var strScale = '';
    	strScale = FormatNumber(fScale,strMetric,fMaxAbs > 100000);
    	return strScale;
    });		

	var axisX = d3.axisBottom()
		.scale(scaleX)
		.tickFormat(function(nScale)
		{
			return aData[nScale][config.legend];
		});

		
	var sChart = sSvg.append('g')
		.attr('transform', 'translate(' + nBorderLeft + ',' + nBorderTop + ')');


	var sAxisY = sChart.append('g')
		.attr('class', strCssClassPrefix+'axis-y')
	  .call(axisY);

	var sAxisX = sChart.append("g")
		.attr("class", strCssClassPrefix+'axis-x')
		.attr('transform', 'translate(0,' + nChartHeight + ')')
		.call(axisX);		
		
	var nIndex = -1;
	var sColumns = sChart.selectAll('.'+strCssClassPrefix+'column')
		.data(aData)
		.enter()
			.append('g')
				.attr('class', strCssClassPrefix+'column')
				.attr('transform', function(row)
					{
						nIndex++;
						return 'translate('+scaleX(nIndex)+',0)';
					});

	
	previousValues = null;
	for (var nDataIndex = 0; nDataIndex < aData.length; nDataIndex++)
	{
		values = aData[nDataIndex];
		for (var nLinesIndex = 0; nLinesIndex < aLines.length; nLinesIndex++)
		{
			// Draw dots
			sChart
				.append('circle')
					.attr('cx',scaleX(nDataIndex) + (scaleX.bandwidth()/2) )
					.attr('cy',scaleY(values[aLines[nLinesIndex].key]))
					.attr('r',aLines[nLinesIndex].weight+1)
					.attr('stroke',aLines[nLinesIndex].color)
					.attr('fill',aLines[nLinesIndex].color)
					.attr('class',strCssClassPrefix+'dot');
			
			// Draw lines
			if (previousValues != null)
			{
				sChart
					.append('line')
						.attr('x1',scaleX(nDataIndex-1) + (scaleX.bandwidth()/2))
						.attr('x2',scaleX(nDataIndex) + (scaleX.bandwidth()/2))
						.attr('y1',scaleY(previousValues[aLines[nLinesIndex].key]))
						.attr('y2',scaleY(values[aLines[nLinesIndex].key]))
						.attr('class',strCssClassPrefix+'line')
						.style('stroke',aLines[nLinesIndex].color)
						.style('stroke-width',aLines[nLinesIndex].weight);
			}
		
		}
		previousValues = values;		
	}
		

	sColumns
		.insert('rect',':first-child')
			.attr('height', nChartHeight)
			.attr('width', scaleX.bandwidth())
			.attr('y', '1')
			.attr('fill-opacity', '0.5')
			.attr('class',strCssClassPrefix+'column-background'); 

	function FormatNumber(fNumber,strMetric,bShortHand = false)
	{
		var strNumber;
		if (bShortHand)
		{
			strNumber = (Math.sign(fNumber)*((Math.abs(fNumber)/1000))).toLocaleString("en-GB") + 'k';
		}
		else
		{
			strNumber = d3.format(",.0f")(fNumber);
		}
		return '' + strNumber + ' ' + strMetric;
	}

}
