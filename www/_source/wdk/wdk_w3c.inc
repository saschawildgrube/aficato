<?php


	function w3cGetValidatorURL($strTargetURL)
	{
		$strTargetURL = ReplaceString($strTargetURL,"validate=1","");
		$strTargetURL = StringURLEncode($strTargetURL);
		//$strValidatorURL = "http://validator.w3.org/check?uri=$strTargetURL";
		$strValidatorURL = "https://validator.w3.org/nu/?doc=$strTargetURL";
		return $strValidatorURL;
		
	}

/*	
	// w3c checked the behaviour of the site on 2015-07-22
	
	function w3cInterpretResponse($strResponse)
	{
		if ($strResponse == false)
		{
			return "UNDEFINED";
		}
		if (FindString($strResponse,"Passed") == -1)
		{
			return "FAILED";
		}
		if (FindString($strResponse,"This document was successfully checked") == -1)
		{
			return "FAILED";
		}
		if (FindString($strResponse,"Errors found while checking this document") != -1)
		{
			return "FAILED";
		}
		return "PASSED";
	}
*/

	function w3cInterpretResponse($strResponse)
	{
		if ($strResponse == false)
		{
			return "UNDEFINED";
		}
		if (FindString($strResponse,"The document validates") == -1)
		{
			return "FAILED";
		}
		if (FindString($strResponse,"There were errors") != -1)
		{
			return "FAILED";
		}
		return "PASSED";
	}



	function w3cValidateURL($strURL)
	{
		$strValidatorURL = w3cGetValidatorURL($strURL);
		$strOutput = HttpRequest(
			$strValidatorURL,
			array(),
			"get",
			15,
			array(),
			array("User-Agent" => "Mozilla/5.0 (Windows NT 6.3; Win64; x64; Trident/7.0; rv:11.0) like Gecko"));
			
		//print($strOutput);
			
		return w3cInterpretResponse($strOutput);
	}
	

