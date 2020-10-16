'use strict';

function D3_RadialTidyTree(selector, aProps)
{
	var svg = d3.select(selector);
	if (svg.empty() == true)
	{
		console.log('D3RadialTidyTree: Could not identify element based on selector: '+selector);
		return;	
	}

	svg.style("height","auto");
	
	var nSvgWidth = svg.node().getBoundingClientRect().width;
	var nSvgHeight = svg.node().getBoundingClientRect().height;
	
	var aData = aProps['data'];
	
	var aDataHierarchical = d3.hierarchy(aData);
	const fRadius = nSvgWidth / 2;	

	const root = d3.tree()
    .size([2 * Math.PI, fRadius])
    .separation((a, b) => (a.parent == b.parent ? 1 : 2) / a.depth)
    (aDataHierarchical)
    
  svg.attr("class", "d3-radialtidytree-svg");
	
  const link = svg.append("g")
	  .attr("fill", "none")
	  .attr("stroke", "#555")
	  .attr("stroke-opacity", 0.4)
	  .attr("stroke-width", 1.5)
		.selectAll("path")
		.data(root.links())
		.join("path")
	  .attr("d", d3.linkRadial()
			.angle(d => d.x)
			.radius(d => d.y)
			);
  
  const node = svg.append("g")
	  .attr("stroke-linejoin", "round")
	  .attr("stroke-width", 3)
		.selectAll("g")
		.data(root.descendants().reverse())
		.join("g")
	  .attr("transform", d => `
	    rotate(${d.x * 180 / Math.PI - 90})
	    translate(${d.y},0)
	  `);
  
  node.append("circle")
		//.attr("fill", d => d.children ? "#555" : "#999")
		.attr("class", d => d.children ? "d3-radialtidytree-path-end" : "d3-radialtidytree-path-start")
		.attr("r", 2.5);
  
  node.append("text")
	  .attr("dy", "0.31em")
	  .attr("x", d => d.x < Math.PI === !d.children ? 6 : -6)
	  .attr("text-anchor", d => d.x < Math.PI === !d.children ? "start" : "end")
	  .attr("transform", d => d.x >= Math.PI ? "rotate(180)" : null)
	  .text(d => d.data.name)
		.clone(true).lower()
	  .attr("stroke", "white");
	  
	  
	const svgnode = svg.node();
	const box = svgnode.getBBox();
	svgnode.setAttribute("viewBox", `${box.x} ${box.y} ${box.width} ${box.height}`);	  

}
