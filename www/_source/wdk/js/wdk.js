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
	function TraceError(variant)
	{
		console.error(variant);
	}
	
	function GetValue(vObject, vKey1, vKey2, vKey3, vKey4, vKey5, vKey6)
	{
		if (vObject == undefined)
		{
			return null;
		}
		if (vKey1 == undefined)
		{
			return null;
		}
		if (vKey2 == undefined)
		{
			if (vObject[vKey1] == undefined)
			{
				return null;
			}
			return vObject[vKey1];
		}
		return GetValue(vObject[vKey1],vKey2,vKey3,vKey4,vKey5,vKey6);
	}
	
	function RenderValue(vValue, nLevel)
	{
		nLevel = GetIntegerValue(nLevel);
		if (nLevel > 10)
		{
			return 'level too high';
		}
		var strLevel = '___';
		strLevel = strLevel.repeat(nLevel);
		
		var bIsObject = vValue instanceof Object;
		var strTypeOf = typeof vValue;
	
		if (vValue == undefined)
		{
			return 'undefined';
		}
		if (vValue == null)
		{
			return 'null';
		}
	
		var strOutput = '';
		
		if (Array.isArray(vValue))
		{
			strOutput += '\n';
			vValue.forEach( function(vElement,nKey)
			{
				strOutput += strLevel+'['+nKey+']: '+RenderValue(vElement,nLevel+1)+'\n';
			});
		}
		else if (bIsObject || (strTypeOf == 'object'))
		{
			strOutput += '\n';
			Object.keys(vValue).forEach( function(strKey)
			{
				strOutput += strLevel+'["'+strKey+'"]: '+RenderValue(vValue[strKey],nLevel+1)+'\n';
			});
		}
		else
		{
			strOutput += ''+vValue.toString();
		}
		return strOutput;
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

	function GetIntegerValue(value)
	{
		return parseInt(GetNumberValue(value));	
	}

	function HttpRequest(strURL)
	{
		var strResponse = '';
		function Listener()
		{
			strResponse = this.responseText;
		}
		var xmlhttprequest = new XMLHttpRequest();
		xmlhttprequest.addEventListener("load", Listener);
		xmlhttprequest.open("GET", strURL, false);
		xmlhttprequest.send();
		return strResponse;
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
	
	function GetRandomInteger(nMax = 1)
	{
		return Math.floor(Math.random() * (nMax+1));
	}

	function GetRandomToken(nLength)
	{
		Trace('GetRandomToken('+nLength+')');
		var strToken = '';
		for (var nCount = 0; nCount < nLength; nCount++)
		{
			strToken += GetStringValue(GetRandomInteger(9));
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
	

