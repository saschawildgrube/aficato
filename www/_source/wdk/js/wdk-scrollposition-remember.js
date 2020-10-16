// WDK Scroll Position Remember
'use strict';

$(window).scroll(function()
	{ 
		var nScrollPositionY = $(document).scrollTop().valueOf();
	 	SetCookie("wdk-scrollposition-y",nScrollPositionY,1);
	}
);