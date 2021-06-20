<?php

	function HttpRequest(
		$strURL,
		$vParams = null,
		$strMode = "get",
		$nTimeOutSeconds = 60,
		$arrayCookies = array(),
		$arrayHeaders = array(),
		$bHeadersInOutput = false)
	{
		if (IsValidURL($strURL) == false)
		{
			return false;	
		}
		
		$bHeadersInOutput = GetBoolValue($bHeadersInOutput);
		$strMode = StringLowerCase($strMode);
		$bHttps = false;
		if (StringSection($strURL,0,8) == "https://")
		{
			$bHttps = true;
		}
		
		if ($strMode == "get")
		{
			$arrayParams = array();
			if (is_array($vParams))
			{
				$arrayParams = $vParams;
			}
			$strURL = MakeURL($strURL,$arrayParams,false);
		}
		else if ($strMode == "post")
		{
			if (is_array($vParams))
			{
				$strParams = MakeURLParameters($vParams);
			}
			else
			{
				$strParams = GetStringValue($vParams);
			}
		}
		else
		{
			return false;	
		}

		$ch = curl_init($strURL);
		curl_setopt($ch, CURLOPT_TIMEOUT, $nTimeOutSeconds);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
		curl_setopt($ch, CURLOPT_HEADER, $bHeadersInOutput);
		if ($strMode == "post")
		{
			curl_setopt($ch, CURLOPT_POSTFIELDS, $strParams);
			curl_setopt($ch, CURLOPT_POST, TRUE);
		}
		if ($bHttps == true)
		{
			curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
			curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, FALSE);
		}

		if (is_array($arrayCookies) == false)
		{
			$arrayCookies = array();	
		}
		if (ArrayCount($arrayCookies) > 0)
		{
			$strCookie = MakeURLParameters($arrayCookies,";");
			curl_setopt($ch, CURLOPT_COOKIE, $strCookie);
		}

		if (is_array($arrayHeaders) == false)
		{
			$arrayHeaders = array();	
		}
		if (ArrayCount($arrayHeaders) > 0)
		{
			if (ArrayKeyExists($arrayHeaders,"Accept-Encoding"))
			{
				$strAcceptEncoding = ArrayGetValue($arrayHeaders,"Accept-Encoding"); 
				ArrayRemoveKey($arrayHeaders,"Accept-Encoding");
				curl_setopt($ch, CURLOPT_ENCODING, $strAcceptEncoding);
			}
			
			$arrayHeadersList = array();
			foreach ($arrayHeaders as $strHeaderKey => $strHeaderValue)
			{
				$arrayHeadersList[] = $strHeaderKey.": ".$strHeaderValue;
			}
			
			curl_setopt($ch, CURLOPT_HTTPHEADER, $arrayHeadersList);
		}
	
		$strOutput = curl_exec($ch);
		$nError = curl_errno($ch);
		curl_close($ch);
		
		return $strOutput;
	}	

 
 	function ParseHttpResponse($strResponse)
 	{
 		$arrayResponse = array();
 		$nPos = FindString($strResponse,"\r\n\r\n");
 		if ($nPos  == -1)
 		{
 			$arrayResponse["headers"] = $strResponse;
 			$arrayResponse["content"] = "";
 			$arrayResponse["statuscode"] = "";
		}
		else
		{
 			$arrayResponse["headers"] = StringSection($strResponse,0,$nPos);
 			$arrayResponse["content"] = StringSection($strResponse,$nPos+4);
 			$nPosStatusCode = FindString($arrayResponse["headers"]," ");
 			$strStatusCode = StringSection($arrayResponse["headers"],$nPosStatusCode+1,3);
 			$arrayResponse["statuscode"] = $strStatusCode;
		}
 		return $arrayResponse;
 	}

 	function ParseHttpResponseEx($strResponse)
 	{
 		$arrayResponse = ParseHttpResponse($strResponse);

		// Parse headers (cookies and redirect location)
 		$strHeaders = ArrayGetValue($arrayResponse,"headers");
 		if ($strHeaders != "")
 		{
			$arrayHeaderLines = StringExplode($strHeaders,"\n");
			$arrayCookies = array();
			foreach ($arrayHeaderLines as $strLine) 
			{
				if (FindString($strLine,"Location: ") == 0)
				{ 
					$arrayResponse["redirect-location"] = StringTrim(StringSection($strLine,10));
				}
				else if (FindString($strLine,"Set-Cookie: ") == 0)
				{
					$strCookieLine = StringSection($strLine,12);	
					$arrayCookieAttributes = StringExplode($strCookieLine,";",true);
					$strCookieKeyValue = ArrayGetValue($arrayCookieAttributes,0);
					$arrayKeyValue = StringExplode($strCookieKeyValue,"=",true,2);
					$strCookieName = ArrayGetValue($arrayKeyValue,0);
					$strCookieValue = ArrayGetValue($arrayKeyValue,1);
					if ($strCookieName != "")
					{
						$arrayCookies[$strCookieName] = $strCookieValue;
					}
				}
			}
			$arrayResponse["cookies"]	= $arrayCookies;
 		}


 		$strContent = ArrayGetValue($arrayResponse,"content");
 		if ($strContent != "")
 		{
 			libxml_use_internal_errors(true);
 			
 			$domdoc = new DOMDocument();
			$domdoc->recover = true;
			$domdoc->strictErrorChecking = false;
 			
 			$bResult = $domdoc->loadHtml($strContent);

 			if ($bResult == true)
 			{
				// Links
	 			$arrayLinks = array();
	 			$arrayAElements = $domdoc->getElementsByTagName('a');
				foreach ($arrayAElements as $nodeA)
				{
					$arrayLinks[] = $nodeA->getAttribute( "href" );
				}
				$arrayResponse["links"] = $arrayLinks;

 				// Forms
	 			$arrayForms = array();
	 			$arrayFormElements = $domdoc->getElementsByTagName("form");
				foreach ($arrayFormElements as $nodeForm)
				{
					$arrayForm = array();
					//$arrayForm["html"] = $nodeForm->ownerDocument->saveHTML( $nodeForm );
					$arrayForm["action"] = $nodeForm->getAttribute("action");
					$arrayForm["method"] = $nodeForm->getAttribute("method");
					
					$arrayValues = array();
					
					$arrayElementsInput = $nodeForm->getElementsByTagName("input");
					foreach ($arrayElementsInput as $nodeInput)
					{
						$arrayValues[$nodeInput->getAttribute("name")] = $nodeInput->getAttribute("value");
					}
					$arrayElementsTextArea = $nodeForm->getElementsByTagName("textarea");
					foreach ($arrayElementsTextArea as $nodeTextArea)
					{
						$arrayValues[$nodeTextArea->getAttribute("name")] = $nodeTextArea->nodeValue;
					}
					
					$arrayElementsSelect = $nodeForm->getElementsByTagName("select");
					foreach ($arrayElementsSelect as $nodeSelect)
					{
						$strName = $nodeSelect->getAttribute("name");
						$strValue = "";
						$arrayElementsOption = $nodeForm->getElementsByTagName("option");
						foreach ($arrayElementsOption as $nodeOption)
						{
							if ($nodeOption->getAttribute("selected") == "selected")
							{
								$strValue = $nodeOption->getAttribute("value");
							}
						}
						$arrayValues[$strName] = $strValue;
					}
					
					$arrayForm["values"] = $arrayValues;

					$arrayForms[] = $arrayForm;
				}
				$arrayResponse["forms"] = $arrayForms;
 			}
 			
 			// identify meta tags
 			// Identify forms and create a structured array of their parameters 			// Create a link list
 		}
 		return $arrayResponse;
 	}

 	

	function RedirectAndExit($strURL,$strHeaders = '')
	{
		header('Location: '.$strURL);   
		print(
MakeDocType('html5').
'<html>
	<head>
		'.$strHeaders.'
		<meta http-equiv="refresh" content="0; URL='.$strURL.'"/>
		<title>Redirecting</title>
	</head>
	<body>
		<p>Redirecting to '.HtmlEncode($strURL).'</p>  
	</body>
</html>');
		exit();
	}