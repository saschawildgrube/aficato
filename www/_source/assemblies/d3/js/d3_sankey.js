'use strict';

// https://observablehq.com/@d3/sankey-diagram


function D3_Sankey(vSelector, aProps)
{
	var sSvg = d3.select(vSelector);
	if (sSvg.empty() == true)
	{
		console.log('D3_Sankey: Could not identify element based on selector: '+vSelector);
		return;	
	}

	var aData = aProps['data'];
	if (aData.length == 0)
	{
		console.log('D3_Sankey: No data to display.');
		return;	
	}

	var strSymbol = GetStringValue(aProps['symbol']);
	var nPrecision = GetNumberValue(aProps['precision']);
	if (strSymbol != '')
	{
			strSymbol = ' ' + strSymbol;
	}
	if (nPrecision < 0)
	{
		nPrecision = 0;
	}
	if (nPrecision > 3)
	{
			nPrecision = 3;
	}
	var D3_Sankey_FormatNumber = function(value)
		{
			return d3.format(',.'+GetStringValue(nPrecision)+'f')(value) + strSymbol;
		};

	var strCssClassPrefix = GetStringValue(aProps['cssclassprefix']);
	if (strCssClassPrefix == '')
	{
		strCssClassPrefix = 'd3-sankey-';
	}
	
	
	var nSvgWidth = sSvg.node().getBoundingClientRect().width;
	var nSvgHeight = sSvg.node().getBoundingClientRect().height;

	//var strEdgeColor = 'path'; // 'input', 'output', 'none'
	var strEdgeColor = 'input'; // 'input', 'output', 'none'
	

	const aNodes = Array.from(new Set(aData.flatMap(l => [l.source, l.target])), name => ({name}));

	var graph =
		{
			links: aData,
			nodes: aNodes
		};
	


	// Helper functions
	
	var pi = Math.PI,
	    tau = 2 * pi,
	    epsilon = 1e-6,
	    tauEpsilon = tau - epsilon;
	
	function Path()
	{
	  this._x0 = this._y0 = // start of current subpath
	  this._x1 = this._y1 = null; // end of current subpath
	  this._ = "";
	}
	
	function path()
	{
	  return new Path;
	}
	
	Path.prototype = path.prototype = {
	  constructor: Path,
	  moveTo: function(x, y)
	  {
	    this._ += "M" + (this._x0 = this._x1 = +x) + "," + (this._y0 = this._y1 = +y);
	  },
	  closePath: function()
	  {
	    if (this._x1 !== null) {
	      this._x1 = this._x0, this._y1 = this._y0;
	      this._ += "Z";
	    }
	  },
	  lineTo: function(x, y)
	  {
	    this._ += "L" + (this._x1 = +x) + "," + (this._y1 = +y);
	  },
	  quadraticCurveTo: function(x1, y1, x, y) {
	    this._ += "Q" + (+x1) + "," + (+y1) + "," + (this._x1 = +x) + "," + (this._y1 = +y);
	  },
	  bezierCurveTo: function(x1, y1, x2, y2, x, y) {
	    this._ += "C" + (+x1) + "," + (+y1) + "," + (+x2) + "," + (+y2) + "," + (this._x1 = +x) + "," + (this._y1 = +y);
	  },
	  arcTo: function(x1, y1, x2, y2, r) {
	    x1 = +x1, y1 = +y1, x2 = +x2, y2 = +y2, r = +r;
	    var x0 = this._x1,
	        y0 = this._y1,
	        x21 = x2 - x1,
	        y21 = y2 - y1,
	        x01 = x0 - x1,
	        y01 = y0 - y1,
	        l01_2 = x01 * x01 + y01 * y01;
	
	    // Is the radius negative? Error.
	    if (r < 0) throw new Error("negative radius: " + r);
	
	    // Is this path empty? Move to (x1,y1).
	    if (this._x1 === null) {
	      this._ += "M" + (this._x1 = x1) + "," + (this._y1 = y1);
	    }
	
	    // Or, is (x1,y1) coincident with (x0,y0)? Do nothing.
	    else if (!(l01_2 > epsilon));
	
	    // Or, are (x0,y0), (x1,y1) and (x2,y2) collinear?
	    // Equivalently, is (x1,y1) coincident with (x2,y2)?
	    // Or, is the radius zero? Line to (x1,y1).
	    else if (!(Math.abs(y01 * x21 - y21 * x01) > epsilon) || !r) {
	      this._ += "L" + (this._x1 = x1) + "," + (this._y1 = y1);
	    }
	
	    // Otherwise, draw an arc!
	    else {
	      var x20 = x2 - x0,
	          y20 = y2 - y0,
	          l21_2 = x21 * x21 + y21 * y21,
	          l20_2 = x20 * x20 + y20 * y20,
	          l21 = Math.sqrt(l21_2),
	          l01 = Math.sqrt(l01_2),
	          l = r * Math.tan((pi - Math.acos((l21_2 + l01_2 - l20_2) / (2 * l21 * l01))) / 2),
	          t01 = l / l01,
	          t21 = l / l21;
	
	      // If the start tangent is not coincident with (x0,y0), line to.
	      if (Math.abs(t01 - 1) > epsilon) {
	        this._ += "L" + (x1 + t01 * x01) + "," + (y1 + t01 * y01);
	      }
	
	      this._ += "A" + r + "," + r + ",0,0," + (+(y01 * x20 > x01 * y20)) + "," + (this._x1 = x1 + t21 * x21) + "," + (this._y1 = y1 + t21 * y21);
	    }
	  },
	  arc: function(x, y, r, a0, a1, ccw) {
	    x = +x, y = +y, r = +r, ccw = !!ccw;
	    var dx = r * Math.cos(a0),
	        dy = r * Math.sin(a0),
	        x0 = x + dx,
	        y0 = y + dy,
	        cw = 1 ^ ccw,
	        da = ccw ? a0 - a1 : a1 - a0;
	
	    // Is the radius negative? Error.
	    if (r < 0) throw new Error("negative radius: " + r);
	
	    // Is this path empty? Move to (x0,y0).
	    if (this._x1 === null) {
	      this._ += "M" + x0 + "," + y0;
	    }
	
	    // Or, is (x0,y0) not coincident with the previous point? Line to (x0,y0).
	    else if (Math.abs(this._x1 - x0) > epsilon || Math.abs(this._y1 - y0) > epsilon) {
	      this._ += "L" + x0 + "," + y0;
	    }
	
	    // Is this arc empty? We’re done.
	    if (!r) return;
	
	    // Does the angle go the wrong way? Flip the direction.
	    if (da < 0) da = da % tau + tau;
	
	    // Is this a complete circle? Draw two arcs to complete the circle.
	    if (da > tauEpsilon) {
	      this._ += "A" + r + "," + r + ",0,1," + cw + "," + (x - dx) + "," + (y - dy) + "A" + r + "," + r + ",0,1," + cw + "," + (this._x1 = x0) + "," + (this._y1 = y0);
	    }
	
	    // Is this arc non-empty? Draw an arc!
	    else if (da > epsilon) {
	      this._ += "A" + r + "," + r + ",0," + (+(da >= pi)) + "," + cw + "," + (this._x1 = x + r * Math.cos(a1)) + "," + (this._y1 = y + r * Math.sin(a1));
	    }
	  },
	  rect: function(x, y, w, h) {
	    this._ += "M" + (this._x0 = this._x1 = +x) + "," + (this._y0 = this._y1 = +y) + "h" + (+w) + "v" + (+h) + "h" + (-w) + "Z";
	  },
	  toString: function() {
	    return this._;
	  }
	};
	
	function linkSource(d)
	{
	  return d.source;
	}
	
	function linkTarget(d)
	{
	  return d.target;
	}
	
	
	function link(curve)
	{
	  var source = linkSource,
	      target = linkTarget,
	      x = p => p[0],
	      y = p => p[1],
	      context = null;
	
	  function link()
	  {
	    var buffer, argv = Array.prototype.slice.call(arguments), s = source.apply(this, argv), t = target.apply(this, argv);
	    if (!context) context = buffer = path();
	    curve(context, +x.apply(this, (argv[0] = s, argv)), +y.apply(this, argv), +x.apply(this, (argv[0] = t, argv)), +y.apply(this, argv));
	    if (buffer) return context = null, buffer + "" || null;
	  }
	
	  link.source = function(_)
	  {
	    return arguments.length ? (source = _, link) : source;
	  };
	
	  link.target = function(_)
	  {
	    return arguments.length ? (target = _, link) : target;
	  };
	
	  link.x = function(_)
	  {
	    return arguments.length ? (x = typeof _ === "function" ? _ : constant(+_), link) : x;
	  };
	
	  link.y = function(_)
	  {
	    return arguments.length ? (y = typeof _ === "function" ? _ : constant(+_), link) : y;
	  };
	
	  link.context = function(_)
	  {
	    return arguments.length ? ((context = _ == null ? null : _), link) : context;
	  };
	
	  return link;
	}
	
	function curveHorizontal(context, x0, y0, x1, y1)
	{
	  context.moveTo(x0, y0);
	  context.bezierCurveTo(x0 = (x0 + x1) / 2, y0, x0, y1, x1, y1);
	}
	
	function linkHorizontal()
	{
	  return link(curveHorizontal);
	}
	
	
	function horizontalSource(d)
	{
	  return [d.source.x1, d.y0];
	}
	
	function horizontalTarget(d)
	{
	  return [d.target.x0, d.y1];
	}
	
	function sankeyLinkHorizontal()
	{
	  return linkHorizontal()
	      .source(horizontalSource)
	      .target(horizontalTarget);
	}
	
	
	
	/*
	function sankey2()   
	{
	  const sankey3 = sankey()
	      .nodeId(d => d.name)
	      //.nodeAlign(d3[`sankey${align[0].toUpperCase()}${align.slice(1)}`])
	      .nodeAlign(justify)
	      .nodeWidth(15)
	      .nodePadding(10)
	      .extent([[1, 5], [width - 1, height - 5]]);
	  return ({nodes, links}) => sankey3(
	  {
	    nodes: nodes.map(d => Object.assign({}, d)),
	    links: links.map(d => Object.assign({}, d))
	  });
	}
	*/
	
	function color(strName)
	{
	  const colorScale = d3.scaleOrdinal(d3.schemeCategory10);
	  //return name => color(strName.replace(/ .*/, ""));
	  return colorScale(strName.replace(/ .*/, ""));
	}
	
	
	function ascendingSourceBreadth(a, b)
	{
	  return ascendingBreadth(a.source, b.source) || a.index - b.index;
	}
	function ascendingTargetBreadth(a, b)
	{
	  return ascendingBreadth(a.target, b.target) || a.index - b.index;
	}
	function ascendingBreadth(a, b)
	{
	  return a.y0 - b.y0;
	}
	
	function value(d)
	{
	  return d.value;
	}
	
	function defaultId(d)
	{
	  //return d.index;
	  return d.name;
	}
	
	function defaultNodes(graph)
	{
	  return graph.nodes;
	}
	
	function defaultLinks(graph)
	{
	  return graph.links;
	}
	
	function find(nodeById, id)
	{
	  const node = nodeById.get(id);
	  if (!node) throw new Error("missing: " + id);
	  return node;
	}
	
	function computeLinkBreadths({nodes})
	{
	  for (const node of nodes) {
	    let y0 = node.y0;
	    let y1 = y0;
	    for (const link of node.sourceLinks) {
	      link.y0 = y0 + link.width / 2;
	      y0 += link.width;
	    }
	    for (const link of node.targetLinks) {
	      link.y1 = y1 + link.width / 2;
	      y1 += link.width;
	    }
	  }
	}
	
	function targetDepth(d)
	{
	  return d.target.depth;
	}
	
	function left(node)
	{
	  return node.depth;
	}
	
	function right(node, n)
	{
	  return n - 1 - node.height;
	}
	
	function justify(node, n)
	{
	  return node.sourceLinks.length ? node.depth : n - 1;
	}
	
	function center(node)
	{
	  return node.targetLinks.length ? node.depth
	      : node.sourceLinks.length ? min(node.sourceLinks, targetDepth) - 1
	      : 0;
	}
	
	function constant(x)
	{
	  return function()
	  {
	    return x;
	  };
	}
	
	
	function max(values, valueof)
	{
	  let max;
	  if (valueof === undefined) {
	    for (const value of values) {
	      if (value != null
	          && (max < value || (max === undefined && value >= value))) {
	        max = value;
	      }
	    }
	  } else {
	    let index = -1;
	    for (let value of values) {
	      if ((value = valueof(value, ++index, values)) != null
	          && (max < value || (max === undefined && value >= value))) {
	        max = value;
	      }
	    }
	  }
	  return max;
	}
	
	function min(values, valueof)
	{
	  let min;
	  if (valueof === undefined) {
	    for (const value of values) {
	      if (value != null
	          && (min > value || (min === undefined && value >= value))) {
	        min = value;
	      }
	    }
	  } else {
	    let index = -1;
	    for (let value of values) {
	      if ((value = valueof(value, ++index, values)) != null
	          && (min > value || (min === undefined && value >= value))) {
	        min = value;
	      }
	    }
	  }
	  return min;
	}
	
	function sum(values, valueof)
	{
	  let sum = 0;
	  if (valueof === undefined)
	  {
	    for (let value of values)
	    {
	      if (value = +value) {
	        sum += value;
	      }
	    }
	  } else {
	    let index = -1;
	    for (let value of values)
	    {
	      if (value = +valueof(value, ++index, values))
	      {
	        sum += value;
	      }
	    }
	  }
	  return sum;
	}
	
	/*
	function Sankey()
	{
	*/
	  let x0 = 0, y0 = 0, x1 = 1, y1 = 1; // extent
	  //let dx = 24; // nodeWidth
	  //let dy = 8, py; // nodePadding
	  let dx = 15; // nodeWidth
	  let dy = 10, py; // nodePadding
	
	  let id = defaultId;
	  let align = justify;
	  let sort;
	  let linkSort;
	  let nodes = defaultNodes;
	  let links = defaultLinks;
	  let iterations = 6;
	
	  function sankey()
	  {
	    const graph =
	    	{
	    		nodes: nodes.apply(null, arguments),
	    		links: links.apply(null, arguments)
	    	};
	    computeNodeLinks(graph);
	    computeNodeValues(graph);
	    computeNodeDepths(graph);
	    computeNodeHeights(graph);
	    computeNodeBreadths(graph);
	    computeLinkBreadths(graph);
	    return graph;
	  }
	
	  sankey.update = function(graph)
	  {
	    computeLinkBreadths(graph);
	    return graph;
	  };
	
	  sankey.nodeId = function(_)
	  {
	    return arguments.length ? (id = typeof _ === "function" ? _ : constant(_), sankey) : id;
	  };
	
	  sankey.nodeAlign = function(_)
	  {
	    return arguments.length ? (align = typeof _ === "function" ? _ : constant(_), sankey) : align;
	  };
	
	  sankey.nodeSort = function(_)
	  {
	    return arguments.length ? (sort = _, sankey) : sort;
	  };
	
	  sankey.nodeWidth = function(_)
	  {
	    return arguments.length ? (dx = +_, sankey) : dx;
	  };
	
	  sankey.nodePadding = function(_)
	  {
	    return arguments.length ? (dy = py = +_, sankey) : dy;
	  };
	
	  sankey.nodes = function(_) {
	    return arguments.length ? (nodes = typeof _ === "function" ? _ : constant(_), sankey) : nodes;
	  };
	
	  sankey.links = function(_)
	   {
	    return arguments.length ? (links = typeof _ === "function" ? _ : constant(_), sankey) : links;
	  };
	
	  sankey.linkSort = function(_)
	  {
	    return arguments.length ? (linkSort = _, sankey) : linkSort;
	  };
	
	  sankey.size = function(_)
	  {
	    return arguments.length ? (x0 = y0 = 0, x1 = +_[0], y1 = +_[1], sankey) : [x1 - x0, y1 - y0];
	  };
	
	  sankey.extent = function(_)
	  {
	    return arguments.length ? (x0 = +_[0][0], x1 = +_[1][0], y0 = +_[0][1], y1 = +_[1][1], sankey) : [[x0, y0], [x1, y1]];
	  };
	
	  sankey.iterations = function(_)
	  {
	    return arguments.length ? (iterations = +_, sankey) : iterations;
	  };
	
	  function computeNodeLinks({nodes, links})
	  {
	    for (const [i, node] of nodes.entries())
	    {
	      node.index = i;
	      node.sourceLinks = [];
	      node.targetLinks = [];
	    }
	    const nodeById = new Map(nodes.map((d, i) => [id(d, i, nodes), d]));
	    for (const [i, link] of links.entries())
	    {
	      link.index = i;
	      let {source, target} = link;
	      if (typeof source !== "object") source = link.source = find(nodeById, source);
	      if (typeof target !== "object") target = link.target = find(nodeById, target);
	      source.sourceLinks.push(link);
	      target.targetLinks.push(link);
	    }
	    if (linkSort != null)
	    {
	      for (const {sourceLinks, targetLinks} of nodes) {
	        sourceLinks.sort(linkSort);
	        targetLinks.sort(linkSort);
	      }
	    }
	  }
	
	  function computeNodeValues({nodes})
	  {
	    for (const node of nodes)
	    {
	      node.value = node.fixedValue === undefined
	          ? Math.max(sum(node.sourceLinks, value), sum(node.targetLinks, value))
	          : node.fixedValue;
	    }
	  }
	
	  function computeNodeDepths({nodes})
	  {
	    const n = nodes.length;
	    let current = new Set(nodes);
	    let next = new Set;
	    let x = 0;
	    while (current.size) {
	      for (const node of current) {
	        node.depth = x;
	        for (const {target} of node.sourceLinks) {
	          next.add(target);
	        }
	      }
	      if (++x > n) throw new Error("circular link");
	      current = next;
	      next = new Set;
	    }
	  }
	
	  function computeNodeHeights({nodes})
	  {
	    const n = nodes.length;
	    let current = new Set(nodes);
	    let next = new Set;
	    let x = 0;
	    while (current.size) {
	      for (const node of current) {
	        node.height = x;
	        for (const {source} of node.targetLinks) {
	          next.add(source);
	        }
	      }
	      if (++x > n) throw new Error("circular link");
	      current = next;
	      next = new Set;
	    }
	  }
	
	  function computeNodeLayers({nodes})
	  {
	    const x = max(nodes, d => d.depth) + 1;
	    const kx = (x1 - x0 - dx) / (x - 1);
	    const columns = new Array(x);
	    for (const node of nodes)
	    {
	      const i = Math.max(0, Math.min(x - 1, Math.floor(align.call(null, node, x))));
	      node.layer = i;
	      node.x0 = x0 + i * kx;
	      node.x1 = node.x0 + dx;
	      if (columns[i]) columns[i].push(node);
	      else columns[i] = [node];
	    }
	    if (sort) for (const column of columns)
	    {
	      column.sort(sort);
	    }
	    return columns;
	  }
	
	  function initializeNodeBreadths(columns)
	  {
	    const ky = min(columns, c => (y1 - y0 - (c.length - 1) * py) / sum(c, value));
	    for (const nodes of columns)
	    {
	      let y = y0;
	      for (const node of nodes)
	      {
	        node.y0 = y;
	        node.y1 = y + node.value * ky;
	        y = node.y1 + py;
	        for (const link of node.sourceLinks)
	        {
	          link.width = link.value * ky;
	        }
	      }
	      y = (y1 - y + py) / (nodes.length + 1);
	      for (let i = 0; i < nodes.length; ++i)
	      {
	        const node = nodes[i];
	        node.y0 += y * (i + 1);
	        node.y1 += y * (i + 1);
	      }
	      reorderLinks(nodes);
	    }
	  }
	
	  function computeNodeBreadths(graph)
	  {
	    const columns = computeNodeLayers(graph);
	    py = Math.min(dy, (y1 - y0) / (max(columns, c => c.length) - 1));
	    initializeNodeBreadths(columns);
	    for (let i = 0; i < iterations; ++i)
	    {
	      const alpha = Math.pow(0.99, i);
	      const beta = Math.max(1 - alpha, (i + 1) / iterations);
	      relaxRightToLeft(columns, alpha, beta);
	      relaxLeftToRight(columns, alpha, beta);
	    }
	  }
	
	  // Reposition each node based on its incoming (target) links.
	  function relaxLeftToRight(columns, alpha, beta)
	  {
	    for (let i = 1, n = columns.length; i < n; ++i) {
	      const column = columns[i];
	      for (const target of column) {
	        let y = 0;
	        let w = 0;
	        for (const {source, value} of target.targetLinks) {
	          let v = value * (target.layer - source.layer);
	          y += targetTop(source, target) * v;
	          w += v;
	        }
	        if (!(w > 0)) continue;
	        let dy = (y / w - target.y0) * alpha;
	        target.y0 += dy;
	        target.y1 += dy;
	        reorderNodeLinks(target);
	      }
	      if (sort === undefined) column.sort(ascendingBreadth);
	      resolveCollisions(column, beta);
	    }
	  }
	
	  // Reposition each node based on its outgoing (source) links.
	  function relaxRightToLeft(columns, alpha, beta)
	  {
	    for (let n = columns.length, i = n - 2; i >= 0; --i) {
	      const column = columns[i];
	      for (const source of column) {
	        let y = 0;
	        let w = 0;
	        for (const {target, value} of source.sourceLinks) {
	          let v = value * (target.layer - source.layer);
	          y += sourceTop(source, target) * v;
	          w += v;
	        }
	        if (!(w > 0)) continue;
	        let dy = (y / w - source.y0) * alpha;
	        source.y0 += dy;
	        source.y1 += dy;
	        reorderNodeLinks(source);
	      }
	      if (sort === undefined) column.sort(ascendingBreadth);
	      resolveCollisions(column, beta);
	    }
	  }
	
	  function resolveCollisions(nodes, alpha)
	  {
	    const i = nodes.length >> 1;
	    const subject = nodes[i];
	    resolveCollisionsBottomToTop(nodes, subject.y0 - py, i - 1, alpha);
	    resolveCollisionsTopToBottom(nodes, subject.y1 + py, i + 1, alpha);
	    resolveCollisionsBottomToTop(nodes, y1, nodes.length - 1, alpha);
	    resolveCollisionsTopToBottom(nodes, y0, 0, alpha);
	  }
	
	  // Push any overlapping nodes down.
	  function resolveCollisionsTopToBottom(nodes, y, i, alpha)
	  {
	    for (; i < nodes.length; ++i) {
	      const node = nodes[i];
	      const dy = (y - node.y0) * alpha;
	      if (dy > 1e-6) node.y0 += dy, node.y1 += dy;
	      y = node.y1 + py;
	    }
	  }
	
	  // Push any overlapping nodes up.
	  function resolveCollisionsBottomToTop(nodes, y, i, alpha)
	  {
	    for (; i >= 0; --i) {
	      const node = nodes[i];
	      const dy = (node.y1 - y) * alpha;
	      if (dy > 1e-6) node.y0 -= dy, node.y1 -= dy;
	      y = node.y0 - py;
	    }
	  }
	
	  function reorderNodeLinks({sourceLinks, targetLinks})
	  {
	    if (linkSort === undefined) {
	      for (const {source: {sourceLinks}} of targetLinks) {
	        sourceLinks.sort(ascendingTargetBreadth);
	      }
	      for (const {target: {targetLinks}} of sourceLinks) {
	        targetLinks.sort(ascendingSourceBreadth);
	      }
	    }
	  }
	
	  function reorderLinks(nodes)
	  {
	    if (linkSort === undefined) {
	      for (const {sourceLinks, targetLinks} of nodes) {
	        sourceLinks.sort(ascendingTargetBreadth);
	        targetLinks.sort(ascendingSourceBreadth);
	      }
	    }
	  }
	
	  // Returns the target.y0 that would produce an ideal link from source to target.
	  function targetTop(source, target)
	  {
	    let y = source.y0 - (source.sourceLinks.length - 1) * py / 2;
	    for (const {target: node, width} of source.sourceLinks) {
	      if (node === target) break;
	      y += width + py;
	    }
	    for (const {source: node, width} of target.targetLinks) {
	      if (node === source) break;
	      y -= width;
	    }
	    return y;
	  }
	
	  // Returns the source.y0 that would produce an ideal link from source to target.
	  function sourceTop(source, target)
	  {
	    let y = target.y0 - (target.targetLinks.length - 1) * py / 2;
	    for (const {source: node, width} of target.targetLinks) {
	      if (node === source) break;
	      y += width + py;
	    }
	    for (const {target: node, width} of source.sourceLinks) {
	      if (node === target) break;
	      y -= width;
	    }
	    return y;
	  }
	/*
	  return sankey;
	}
	*/



	// The actual implementation


	sankey.extent([[1, 5], [nSvgWidth - 1, nSvgHeight - 5]]);

	graph = sankey(graph);

	sSvg.append('g')
		.attr("stroke", "#000")
		.selectAll("rect")
		.data(graph.nodes)
		//.join("rect")
		.enter()
			.append("rect")
				.attr("x", d => d.x0).attr("y", d => d.y0)
				.attr("height", d => d.y1 - d.y0)
				.attr("width", d => d.x1 - d.x0)
				.attr("fill", d => color(d.name))
			.append("title")
				.text(d => d.name+'\n'+D3_Sankey_FormatNumber(d.value));
 
  const sLink = sSvg.append("g")
		.attr("fill", "none")
		.attr("stroke-opacity", 0.5)
		.selectAll("g")
		.data(graph.links)
		.join("g")
			.style("mix-blend-mode", "multiply");

  if (strEdgeColor == 'path')
  {
    const sGradient = sLink.append("linearGradient")
        //.attr("id", d => (d.uid = DOM.uid("link")).id)
        .attr('id', d => d.uid = 'link'+d.index)
        .attr("gradientUnits", "userSpaceOnUse")
        .attr("x1", d => d.source.x1)
        .attr("x2", d => d.target.x0);

    sGradient.append("stop")
        .attr("offset", "0%")
        .attr("stop-color", d => color(d.source.name));

    sGradient.append("stop")
        .attr("offset", "100%")
        .attr("stop-color", d => color(d.target.name));
  }

  sLink.append("path")
      .attr("d", sankeyLinkHorizontal())
      .attr("stroke", d => strEdgeColor == "none" ? "#aaa"
          : strEdgeColor == "path" ? color(d.uid) 
          : strEdgeColor == "input" ? color(d.source.name) 
          : color(d.target.name))
      .attr("stroke-width", d => Math.max(1, d.width));

  sLink.append("title")
      .text(d => d.source.name+' / '+d.target.name+'\n'+D3_Sankey_FormatNumber(d.value));

  sSvg.append("g")
		.style("font", "10px sans-serif")
    .selectAll("text")
    .data(graph.nodes)
    .join("text")
      .attr("x", d => d.x0 < nSvgWidth / 2 ? d.x1 + 6 : d.x0 - 6)
      .attr("y", d => (d.y1 + d.y0) / 2)
      .attr("dy", "0.35em")
      .attr("text-anchor", d => d.x0 < nSvgWidth / 2 ? "start" : "end")
      .text(d => d.name);

}
