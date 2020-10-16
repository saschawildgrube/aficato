<?php

	require_once(GetWDKDir()."wdk_error.inc");
	require_once(GetWDKDir()."wdk_mail.inc");


	function GetRequestURL()
	{
		$strURL =
			$_SERVER["SCRIPT_URI"].
			strstr($_SERVER["REQUEST_URI"],"?");
		return $strURL;
	}
	
	function MakeURLParametersEncoded($arrayParams)
	{
		return MakeURLParameters($arrayParams,"&amp;");
	}

	function MakeURLParameters($arrayParams, $strDelimiter = "&")
	{
		$strParams = "";
		if (is_array($arrayParams))
		{
			$nIndex = 0;
			foreach ($arrayParams as $key => $value)
			{
				if (!is_array($value))
				{
					if ($nIndex != 0)
					{
						$strParams .= $strDelimiter;
					}
					$strParams .= StringURLEncode($key)."=".StringURLEncode($value);
					$nIndex = $nIndex +1;
				}
			}
		}
		return $strParams;
	}
	
	function MakeURL($strRootURL,$arrayParams,$bEncoded=true)
	{
		$strURL = $strRootURL;
		if ($bEncoded == true)
		{
			$strParams = MakeURLParametersEncoded($arrayParams);
		}
		else
		{
			$strParams = MakeURLParameters($arrayParams);
		}
		if ($strParams != '')
		{
			if (FindString($strURL,'?') != -1)
			{
				if ($bEncoded == true)
				{
					$strURL .= '&amp;'.$strParams;
				}
				else
				{
					$strURL .= '&'.$strParams;
				}
			}
			else
			{
				$strURL .= '?'.$strParams;
			}
		}
		return $strURL;
	}
	
	function MakeURLRaw($strRootURL,$arrayParams)
	{
		return MakeURL($strRootURL,$arrayParams,false);
	}	
	


	function TweakURL($strURL)
	{
		if (IsValidURL($strURL))
		{
			return $strURL;	
		}	
		if (FindString($strURL,"www.") == 0)
		{
			$strURL = "http://".$strURL;
		}
		return $strURL;
	}

	function IsValidURL($strURL)
	{
		if ($strURL == "")	
		{
			return false;
		}
		if (StringCheckCharSet($strURL,CHARSET_URL) == false)
		{
			return false;	
		}
		if (
					(StringSection($strURL,0,7) != "http://")
			&& 	(StringSection($strURL,0,8) != "https://")
			&& 	(StringSection($strURL,0,6) != "ftp://") 
			)
		{
			return false;
		}

		$strIPregex = '\b(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\b';

		$strURLregex =
			'((https?|ftp)\:\/\/)?' // SCHEME 
    . '([a-z0-9+!*(),;?&=$_.-]+(\:[a-z0-9+!*(),;?&=$_.-]+)?@)?' // USER and PASSWORD
    . '(([a-z0-9-.]*)\.([a-z]{2,20})|'.$strIPregex.'|localhost)' // HOST with TLD or IP or "localhost"
    . '(\:[0-9]{2,5})?' // PORT
    . '(\/([a-z0-9:+$_=,~%()\/@-]\.?)+)*\/?' // PATH allowing double slashes and commas, equal signs...
    . '(\?[a-z\:+&$_.-][a-z0-9;:@&%=+\/$_.\?\{\}\-]*)?' // GET Query      
		. '(#([a-z!_.\-\/][a-z0-9:&!+\/=$_.\?\-]*)?)?' // ANCHOR 
		;
		
		$strURLregex = '/^'.$strURLregex.'$/';
		
		$strURL = StringLowerCase(u($strURL));

		if (RegExpMatch($strURL,$strURLregex) == false) 
		{ 
			return false; 
	  } 
		return true;
	}

	function ParseURL($strURL)
	{
		if (!IsValidURL($strURL))
		{
			return false;	
		}
		$arrayComponents = parse_url($strURL);
		if ($arrayComponents == false)
		{
			return false;	
		}
		if (!is_array($arrayComponents))
		{
			return false;	
		}
		$arrayResult = array();
		$arrayResult["PROTOCOL"] = 		ArrayGetValue($arrayComponents,"scheme");
		$arrayResult["HOST"] = 				ArrayGetValue($arrayComponents,"host");
		$arrayResult["PORT"] = 				ArrayGetValue($arrayComponents,"port");
		$arrayResult["USER"] =				ArrayGetValue($arrayComponents,"user");
		$arrayResult["PASSWORD"] =		ArrayGetValue($arrayComponents,"pass");
		$arrayResult["PATH"] = 				ArrayGetValue($arrayComponents,"path");
		$arrayResult["PARAMETERS"] =	ArrayGetValue($arrayComponents,"query");
		$arrayResult["ANCHOR"] = 			ArrayGetValue($arrayComponents,"fragment");
		return $arrayResult;
	}
	
	function ParseURLParameters($strURLParameters)
	{
		$strURLParameters = StringTrim($strURLParameters);
		$strURLParameters = ReplaceString($strURLParameters,"\r","");
		$strURLParameters = ReplaceString($strURLParameters,"\n","&");
		$strSpaceToken = MakeUniqueToken($strURLParameters,array());
		$strDotToken = MakeUniqueToken($strURLParameters,array($strSpaceToken));
		$strURLParameters = ReplaceString($strURLParameters," ",$strSpaceToken);
		$strURLParameters = ReplaceString($strURLParameters,"%20",$strSpaceToken);
		$strURLParameters = ReplaceString($strURLParameters,".",$strDotToken);
		$arrayParamsRaw = array();
		parse_str($strURLParameters,$arrayParamsRaw);
		if (is_array($arrayParamsRaw) == false)
		{
			return false;	
		}
		$arrayParams = array();
		foreach ($arrayParamsRaw as $strKey => $strValue)
		{
			$strNewKey = ReplaceString($strKey,$strSpaceToken," ");
			$strNewKey = ReplaceString($strNewKey,$strDotToken,".");
			$strNewValue = ReplaceString($strValue,$strSpaceToken," ");
			$strNewValue = ReplaceString($strNewValue,$strDotToken,".");
			$arrayParams[$strNewKey] = $strNewValue;
		}
		return $arrayParams;
	}

	function StringURLEncode($strString)
	{
		return urlencode($strString);
	}

	function StringURLDecode($strString)
	{
		return urldecode($strString);
	}
 

 	

