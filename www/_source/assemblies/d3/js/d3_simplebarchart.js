'use strict';

function D3_SimpleBarChart(selector, aProps)
{
	var svg = d3.select(selector);
	if (svg.empty() == true)
	{
		console.log('D3SimpleBarChart: Could not identify element based on selector: '+selector);
		return;	
	}

	var arrayData = aProps['data'];
	
	var strCssClassPrefix = GetStringValue(aProps['cssclassprefix']);
	if (strCssClassPrefix == '')
	{
		strCssClassPrefix = 'd3-simplebarchart-';
	}

	var nSvgWidth = svg.node().getBoundingClientRect().width;
	var nSvgHeight = svg.node().getBoundingClientRect().height;
	
	var nBarPadding = 5;
	
	var nBarWidth = (nSvgWidth / arrayData.length);		

	svg.attr('class', strCssClassPrefix+'svg');

	var yScale = d3.scaleLinear()
    .domain([0, d3.max(arrayData)])
    .range([0, nSvgHeight]);

	var barChart = svg.selectAll("rect")
	    .data(arrayData)
	    .enter()
	    .append("rect")
	    .attr("class", strCssClassPrefix+'bar')
	    .attr("y", function(nValue) {
	         return nSvgHeight - yScale(nValue)
	    })
	    .attr("height", function(d) { 
	        return yScale(d);
	    })
	    .attr("width", nBarWidth - nBarPadding)
	    .attr("transform", function (nValue, i) {
	        var translate = [nBarWidth * i, 0]; 
	        return "translate("+ translate +")";
	    });

	var text = svg.selectAll("text")
	    .data(arrayData)
	    .enter()
	    .append("text")
	    .text(function(d) {
	        return d;
	    })
	    .attr("y", function(d, i) {
	        return nSvgHeight - d - 2;
	    })
	    .attr("x", function(d, i) {
	        return nBarWidth * i;
	    })
	    .attr("class", strCssClassPrefix+'text');
}
