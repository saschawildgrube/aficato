function MakeTableOfContents(strTocElementId, nFirstLevel, bIgnorePrevious)
{
	if (typeof nFirstLevel !== 'number' || nFirstLevel < 1)
	{
	    nFirstLevel = 1;
	}
	
	var tocContainer = document.getElementById(strTocElementId);
	if (!tocContainer)
	{
	    return false;
	}
	
	// Get all H1–H6 elements
	var allHeadings = Array.prototype.slice.call(document.body.querySelectorAll('h1, h2, h3, h4, h5, h6'));
	
	// Optional: Ignore headings before the TOC container
	if (bIgnorePrevious)
	{
		allHeadings = allHeadings.filter(function(h)
		{
		  return tocContainer.compareDocumentPosition(h) & Node.DOCUMENT_POSITION_FOLLOWING;
		});
	}
	
	// Filter by min level
	var headings = allHeadings.filter(function(h)
	{
		var level = parseInt(h.tagName.substring(1));
		return level >= nFirstLevel;
	});
	
	if (headings.length === 0) return;
	
	// Remove subheadings under any heading with toc-ignore-children="true"
	var filteredHeadings = [];
	var ignoreLevel = null;
	
	for (var i = 0; i < headings.length; i++)
	{
		var h = headings[i];
		var level = parseInt(h.tagName.substring(1));
		
		if (ignoreLevel !== null && level > ignoreLevel)
		{
	    continue;
		}
		
		filteredHeadings.push(h);
		
		if (h.getAttribute('toc-ignore-children') === 'true')
		{
	    ignoreLevel = level;
		}
		else
		{
	    ignoreLevel = null;
		}
	}
	
	// Assign unique IDs if needed
	var usedIds = new Set();
	filteredHeadings.forEach(function(heading)
	{
	  if (!heading.id || usedIds.has(heading.id))
		{
			var strBaseId = heading.textContent;
			strBaseId = strBaseId.trim();
			strBaseId = strBaseId.toLowerCase();
			strBaseId = strBaseId.replace(/[^\w]+/g, '-');
			var strId = strBaseId;
			var counter = 1;
			while (!strId || usedIds.has(strId))
			{
		    strId = strBaseId + '-' + (counter++);
			}
			heading.id = strId;
		}
		usedIds.add(heading.id);
	});
	
	// Build nested TOC
	var tocRoot = document.createElement('ul');
	var stack = [{ level: nFirstLevel - 1, ul: tocRoot }];
	
	filteredHeadings.forEach(function(heading)
	{
		var level = parseInt(heading.tagName.substring(1));
		
		var li = document.createElement('li');
		var a = document.createElement('a');
		a.href = '#' + heading.id;
		a.textContent = heading.textContent;
		li.appendChild(a);
		
		while (stack.length && stack[stack.length - 1].level >= level)
		{
	    stack.pop();
		}
		
		var parentUl = stack[stack.length - 1].ul;
		parentUl.appendChild(li);
		
		var newUl = document.createElement('ul');
		li.appendChild(newUl);
		
		stack.push({ level: level, ul: newUl });
	});
	
	tocContainer.innerHTML = '';
	tocContainer.appendChild(tocRoot);
	return true;
}