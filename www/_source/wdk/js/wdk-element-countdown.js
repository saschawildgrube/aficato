// WDK Element Countdown
'use strict';

if ( $.fn.countdown )
{
	$('.countdown').each( function(i,el)
	{
		var $el = $(el);
		var strDateTime = $el.data('countdown');
		var strHtml = $el.html();
		
		//$nTimeZoneOffsetMinutes = getTimezoneOffset();
			
		$el.countdown(strDateTime,function(e)
		{
			$(el).html(e.strftime(strHtml));
		});
		
		$el.show();
	});
};
