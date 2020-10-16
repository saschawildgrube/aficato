


function D3_AML_AccountChart(vSelector, aProps)
{
	var sSvg = d3.select(vSelector);
	if (sSvg.empty() == true)
	{
		console.log('D3_AML_AccountChart: Could not identify element based on selector: '+vSelector);
		return;	
	}

	var aData = aProps['data'];

	if (aData.length == 0)
	{
		console.log('D3_AML_AccountChart: No data to display.');
		return;	
	}

	var aBars = aData['bars'];
	if (aBars == null)
	{
		console.log('D3_AML_AccountChart: No data to display.');
		return;	
	}
	if (aBars.length == 0)
	{
		console.log('D3_AML_AccountChart: No data to display.');
		return;	
	}

	
	const strCssClassPrefix = 'aml-accountchart-';
	
	var nSvgWidth = sSvg.node().getBoundingClientRect().width;
	var nSvgHeight = sSvg.node().getBoundingClientRect().height;

	var nBorderTop = 40;	
	var nBorderLeft = 80;
	var nBorderRight = 5;
	var nBorderBottom = 40;
	
	var nChartHeight = Math.max(0,nSvgHeight-nBorderTop-nBorderBottom);
	var nChartWidth = Math.max(0,nSvgWidth-nBorderLeft-nBorderRight);


	var fMax = d3.max(aBars,function(row)
		{
			return Math.max(
				row.bar_positive_amount,
				row.line_value_left,
				row.line_value_right);
	  });

	var fMin = d3.min(aBars,function(row)
		{
			return Math.min(
				-row.bar_negative_amount,
				row.line_value_left,
				row.line_value_right);
	  });
	  
	//const fDomain = fMax + (-fMin);
	const fMaxAbs = Math.max(fMax,-fMin);
	//console.log(fMaxAbs);
	
	aBars.map(function(row)
		{
/*			if ((row.bar_positive_amount / fMax) > 0.2)
			{
				row['bar_positive_count_text_position'] = 'inside';	
			}
			else
			{*/
				row['bar_positive_count_text_position'] = 'outside';	
/*			}*/

/*			if ((row.bar_negative_amount / -fMin) > 0.1) 
			{
				row['bar_negative_count_text_position'] = 'inside';	
			}
			else
			{*/
				row['bar_negative_count_text_position'] = 'outside';	
/*			}*/
			
			
		});


	aFirstRow = aBars[0];
	aFractions = aFirstRow['bar_positive_fractions'];
	aFractionNames = aFractions.map( function(aFraction)
		{
			return aFraction.name;
		});
		

	var scaleY = d3.scaleLinear()
		.rangeRound([0, nChartHeight])
		.domain([fMax*1.1, fMin*1.1])
		.nice();

	var scaleX = d3.scaleBand()
		.rangeRound([0, nChartWidth])
		.padding(.3)
		.domain(aBars.map(function(row)
			{
				return row.axis_text;
			}))

	var axisY = d3.axisLeft()
		.scale(scaleY)
    .tickFormat(function(fScale)
    {
    	var strScale = '';
    	if (fMaxAbs > 500000)
    	{
    		strScale = FormatNumber(fScale);
    	}
    	else
    	{
      	strScale = d3.format(",.0f")(fScale);
			}
      return '' + strScale + ' ' + aData['metric'];
    });		

	var axisX = d3.axisBottom()
		.scale(scaleX);
		
	var sChart = sSvg.append('g')
		.attr('transform', 'translate(' + nBorderLeft + ',' + nBorderTop + ')');


	var sAxisY = sChart.append('g')
		.attr('class', strCssClassPrefix+'axis-y')
	  .call(axisY);

	var sAxisX = sChart.append("g")
		.attr("class", strCssClassPrefix+'axis-x')
		.attr('transform', 'translate(0,' + nChartHeight + ')')
		.call(axisX);		
		
		
	var sColumns = sChart.selectAll('.'+strCssClassPrefix+'column')
		.data(aBars)
		.enter()
			.append('g')
				.attr('class', strCssClassPrefix+'column')
				.attr('transform', function(row)
					{
						return 'translate('+scaleX(row.axis_text)+',0)';
					});

	sColumns
		.insert('rect',':first-child')
			.attr('height', nChartHeight)
			.attr('width', scaleX.bandwidth())
			.attr('y', '1')
			.attr('fill-opacity', '0.5')
			.attr('class',strCssClassPrefix+'column-background'); 

	var sBarPositive = sColumns
		.append('rect')
			.attr('height', function(row)
				{
					return scaleY(0) - scaleY(row.bar_positive_amount) + ((row.bar_positive_amount != 0)?(1):(0));
				})
			.attr('width', scaleX.bandwidth())
			.attr('y', function(row)
				{
					return scaleY(row.bar_positive_amount);
				})
			.attr('class',strCssClassPrefix+'bar-positive');


	
	var sBarNegative = sColumns
		.append('rect')
			.attr('height', function(row)
				{
					return scaleY(-row.bar_negative_amount) - scaleY(0) + ((row.bar_negative_amount != 0)?(1):(0));
				})
			.attr('width', scaleX.bandwidth())
			.attr('y', function(row)
				{
					return scaleY(0);
				})
			.attr('class',strCssClassPrefix+'bar-negative');






	if (aFractionNames.length > 0)
	{
		var scaleFractionsX = d3.scaleBand()
			.rangeRound([0,scaleX.bandwidth()])
			.domain(aFractionNames);

		var sPositiveFractions = sColumns.selectAll('.'+strCssClassPrefix+'positive-fraction')
			.data(function(row)	
				{
					return row.bar_positive_fractions;
				})
			.enter()
	  		.append('g')
	  			.attr('class', strCssClassPrefix+'positive-fraction');
	  			
		sPositiveFractions
			.append('rect')
				.attr('height', function(fraction)
					{
						return scaleY(0) - scaleY(fraction.amount) + ((fraction.amount != 0)?(1):(0));
					})
				.attr('width', scaleFractionsX.bandwidth())
				.attr('x', function(fraction)
					{
						return scaleFractionsX(fraction.name);
					})		
				.attr('y', function(fraction)
					{
						return scaleY(fraction.amount);
					})
				.attr('class',function(fraction)
					{
						var strName = fraction.name;
						if (strName == '*')
						{
							strName = 'default';	
						}
						else
						{
							strName = strName.toLowerCase();
						}
						return strCssClassPrefix+'bar-positive-fraction-'+strName;
					});

		var sNegativeFractions = sColumns.selectAll('.'+strCssClassPrefix+'negative-fraction')
			.data(function(row)	
				{
					return row.bar_negative_fractions;
				})
			.enter()
	  		.append('g')
	  			.attr('class', strCssClassPrefix+'negative-fraction');
	  			
		sNegativeFractions
			.append('rect')
				.attr('height', function(fraction)
					{
						return scaleY(0) - scaleY(fraction.amount) + ((fraction.amount != 0)?(1):(0));
					})
				.attr('width', scaleFractionsX.bandwidth())
				.attr('x', function(fraction)
					{
						return scaleFractionsX(fraction.name);
					})		
				.attr('y', function(fraction)
					{
						return scaleY(0);
					})
				.attr('class',function(fraction)
					{
						var strName = fraction.name;
						if (strName == '*')
						{
							strName = 'default';	
						}
						else
						{
							strName = strName.toLowerCase();
						}
						return strCssClassPrefix+'bar-negative-fraction-'+strName;
					});
	  
	  
	}
	
			
	sColumns
		.append('text')
			.attr('x', -10)
			.attr('y', function(row)
				{
					nY = scaleY(row.bar_positive_amount);
					if (row.bar_positive_count_text_position == 'outside')
					{
						nY -= 30;
					}
					return nY;
				})
			.attr('dy', '1.75em')
			.attr('dx', '1.0em')
			.attr('width', scaleX.bandwidth())
			.attr('class', function(row)
				{
					return strCssClassPrefix+'bar-positive-text-'+row.bar_positive_count_text_position;
				}) 
			.style('text-anchor', 'begin')
			.text(function(row)
				{	
					if (row.bar_positive_count > 0)
					{
						return row.bar_positive_count;
					}
					return '';
				});

	sColumns
		.append('text')
			.attr('x', -10)
			.attr('y', function(row)
				{
					if (row.bar_negative_count_text_position == 'inside')
					{
						return scaleY(0);
					}
					else
					{
						return scaleY(-row.bar_negative_amount) - 5;
					}
				})
			.attr('dy', '1.75em')
			.attr('dx', '1.0em')
			.attr('width', scaleX.bandwidth())
			.attr('class', function(row)
				{
					return strCssClassPrefix+'bar-negative-text-'+row.bar_negative_count_text_position;
				})
			.style('text-anchor', 'begin')
			.text(function(row)
				{	
					if (row.bar_negative_count > 0)
					{
						return row.bar_negative_count;
					}
					return '';
				});



	const fLineX1 = -(scaleX.step() * scaleX.padding() / 2); 
	const fLineX2 = scaleX.bandwidth() + (scaleX.step() * scaleX.padding() / 2); 

	var sLines = sColumns
		.append('line')
			.attr('class',strCssClassPrefix + 'line')
			.attr('x1', fLineX1)
			.attr('y1', function(row)
				{
					return scaleY(row.line_value_left);
				})
			.attr('x2', fLineX2)
			.attr('y2', function(row)
				{
					return scaleY(row.line_value_right);
				})
			;
		

	function FormatNumber(fNumber)
	{
	    //return Math.abs(fNumber) > 999999 ? Math.sign(fNumber)*((Math.abs(fNumber)/1000).toFixed(1)) + 'k' : Math.sign(fNumber)*Math.abs(fNumber);
	    //return Math.sign(fNumber)*((Math.abs(fNumber)/1000).toFixed(1)) + 'k';
	    return (Math.sign(fNumber)*((Math.abs(fNumber)/1000))).toLocaleString("en-GB") + 'k';
	}


}
