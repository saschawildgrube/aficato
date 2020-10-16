// WDK Helper functions
'use strict';

	var g_bTraceActive = false;
	function SetTraceActive(bTraceActive)
	{
		g_bTraceActive = bTraceActive;
	}
	function Trace(variant)
	{
		if (g_bTraceActive == true)
		{
			console.log(variant);
		}
	}
	function Error(variant)
	{
		console.error(variant);
	}
	
	function GetStringValue(value)
	{
		if (typeof value == 'string')
		{
			return value;
		}
		if (typeof value == 'undefined')
		{
			return '';	
		}
		return String(value);	
	}
	
	function GetNumberValue(value)
	{
		if (isNaN(value))
		{
			return 0;	
		}
		if (typeof value == 'number')
		{
			return value;
		}
		if (typeof value == 'undefined')
		{
			return 0;
		}
		if (typeof value == 'string')
		{
			value = parseFloat(value);
			if (isNaN(value))
			{
				return 0;	
			}
			return value;
		}
		return 0;	
	}

	function SetCookie(strName, strValue, nExpiryDays)
	{
		Trace('SetCookie("'+strName+'","'+strValue+'",'+nExpiryDays+')');
		var date = new Date();
		date.setTime(date.getTime() + (nExpiryDays*24*60*60*1000));
		var strExpires = "expires="+date.toUTCString();
		var strCookie = strName + "=" + strValue + "; " + strExpires + "; path={ROOTPATH}";
		document.cookie = strCookie;
	} 
	
	function GetCookie(strName)
	{
		Trace('GetCookie("'+strName+'")');
		var strNameEquals = strName + '=';
		var arrayCookies = document.cookie.split(';');
		for (var i=0; i < arrayCookies.length; i++)
		{
			var strCookie = arrayCookies[i];
			while (strCookie.charAt(0)==' ')
			{
				strCookie = strCookie.substring(1);
			}
			if (strCookie.indexOf(strNameEquals) != -1)
			{
				var strResult = strCookie.substring(strNameEquals.length,strCookie.length);
				Trace('returns "'+strResult+'"');
				return strResult;
			}
		}
		Trace('returns ""');
		return '';
	} 
	
	function GetAllCookies()
	{
		Trace('GetAllCookies()');
		var arrayCookies = document.cookie.split(';');
		for (var i=0; i < arrayCookies.length; i++)
		{
			var strCookie = arrayCookies[i];
			while (strCookie.charAt(0)==' ')
			{
				strCookie = strCookie.substring(1);
			}
			arrayCookies[i] = strCookie;
		}
		Trace('returns');
		Trace(arrayCookies);
		return arrayCookies;
	}
	
	function DeleteCookie(strName)
	{
		Trace('DeleteCookie("'+strName+'")');
		SetCookie(strName,'',-1);	
	}
	
	function Sleep(nMilliseconds)
	{
		Trace('Sleep('+nMilliseconds+')');
		nMilliseconds += new Date().getTime();
	 	while (new Date() < nMilliseconds) {}
	}
	
	function GetRandomToken(nLength)
	{
		Trace('GetRandomToken('+nLength+')');
		var strToken = "";
		for (var nCount = 0; nCount < nLength; nCount++)
		{
			strToken += ""+Math.floor((Math.random() * 10));
		}
		Trace('returns "'+strToken+'"');
		return strToken;
	}
	
	function InitProgressIndicator()
	{
		Trace('InitProgressIndicator()');
		var strCssClassContainer = 'wdk-progressindicator-container';
		$('a.'+strCssClassContainer+', button.'+strCssClassContainer).click(function()
		{
			StartProgressIndicator($(this));
		});			
	}
	
	function StartProgressIndicator(elementContainer)
	{
		Trace('StartProgressIndicator()');
		elementContainer.addClass('active');
	}

	function StopProgressIndicator(elementContainer)
	{
		Trace('StopProgressIndicator()');
		elementContainer.removeClass('active');
	}
	
	function InitProgressIndicatorDownload()
	{
		Trace('InitProgressIndicatorDownload()');
		var strCssClassContainer = 'wdk-progressindicator-container-download';
		$('a.'+strCssClassContainer+', button.'+strCssClassContainer).click(function()
		{
			ProgressIndicatorDownloadOnSubmit($(this));
		});
	}
	
  function ProgressIndicatorDownloadOnSubmit(elementContainer)
  {
    Trace('ProgressIndicatorDownloadOnSubmit()');
    Trace(elementContainer);

    var strToken = elementContainer.attr('data-downloadtoken'); 
    
    Trace('Token: '+strToken);
    var interval = window.setInterval(function()
    {
    	ProgressIndicatorDownloadCheckCookie(elementContainer,interval,strToken);
    }, 1000);
  }


	function ProgressIndicatorDownloadCheckCookie(elementContainer,interval,strToken)
	{
		Trace('ProgressIndicatorDownloadCheckCookie(Token='+strToken+')');
		var strCookieValue = GetCookie('downloadtoken');
		if (strCookieValue == strToken)
		{
			ProgressIndicatorDownloadOnDownload(elementContainer,interval);
		}
	} 

	function ProgressIndicatorDownloadOnDownload(elementContainer,interval)
	{
 		Trace('ProgressIndicatorDownloadOnDownload()');
 		Trace(elementContainer);

 		window.clearInterval(interval);
 		DeleteCookie('downloadtoken');
 		StopProgressIndicator(elementContainer);
	}
	

