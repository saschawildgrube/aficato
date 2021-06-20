// WDK Scroll Position Restore
'use strict';

$(document).ready(function()
	{
		var nScrollPositionY = GetCookie("wdk-scrollposition-y");
		$(document).scrollTop(nScrollPositionY);
	}
);




